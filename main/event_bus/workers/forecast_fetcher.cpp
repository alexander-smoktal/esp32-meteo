#include "forecast_fetcher.h"

#include <algorithm>
#include <ctime>

#include "freertos/FreeRTOS.h"
#include "freertos/event_groups.h"

#include "esp_log.h"
#include "esp_wifi.h"
#include "esp_event.h"
#include "esp_tls.h"

#include "json_parser.h"

#include "../event_bus.h"
#include "common.h"

static constexpr auto TAG = "Forecast Fetcher";

static const uint32_t MAX_HTTP_OUTPUT_BUFFER = 10000;
static char sg_local_response_buffer[MAX_HTTP_OUTPUT_BUFFER + 1] = {0};

// Only fetch when connected to WiFi
static EventGroupHandle_t s_wifi_event_group = xEventGroupCreate();
static constexpr auto WIFI_CONNECTED_BIT = BIT0;

#define CHECK_PARSE(func, ctx)                                      \
    do                                                              \
    {                                                               \
        auto res = func;                                            \
        if (res != OS_SUCCESS)                                      \
        {                                                           \
            ESP_LOGE(TAG, "Failed to parse %s JSON: %d", ctx, res); \
            return false;                                          \
        }                                                           \
    } while (0)

static void event_handler(void *arg, esp_event_base_t event_base,
                          int32_t event_id, void *event_data) {
    if (event_base == IP_EVENT && event_id == IP_EVENT_STA_GOT_IP) {
        xEventGroupSetBits(s_wifi_event_group, WIFI_CONNECTED_BIT);
    }
}

namespace
{
    WeatherIconType icon_to_type(int icon) {
        switch (icon) {
        case 1:
        case 2:
        case 3:
        case 4:
            return WeatherIconType::Sunny;
        case 33:
        case 34:
            return WeatherIconType::ClearMoon;
        case 5:
        case 6:
        case 20:
        case 21:
            return WeatherIconType::MostlyCloudy;
        case 35:
        case 36:
        case 37:
            return WeatherIconType::CloudyMoon;
        case 7:
        case 8:
        case 19:
        case 38:
            return WeatherIconType::Cloudy;
        case 11:
            return WeatherIconType::Fog;
        case 12:
            return WeatherIconType::HeavyRain;
        case 18:
        case 40:
            return WeatherIconType::Rain;
        case 13:
        case 14:
        case 16:
        case 17:
        case 39:
            return WeatherIconType::MostlyRain;
        case 15:
        case 41:
        case 42:
            return WeatherIconType::Storm;
        case 22:
        case 23:
        case 24:
        case 43:
        case 44:
            return WeatherIconType::Snow;
        case 25:
        case 26:
        case 29:
            return WeatherIconType::Sleet;
        case 32:
            return WeatherIconType::Wind;
        case 30:
            return WeatherIconType::Hot;
        case 31:
            return WeatherIconType::Cold;
        default:
            ESP_LOGE(TAG, "Failed to find icon for type: %d", icon);
            return WeatherIconType::Sunny;
        }
    }
}

bool ForecastFetcher::execute() {
    ESP_ERROR_CHECK(esp_event_handler_instance_register(IP_EVENT, IP_EVENT_STA_GOT_IP, &event_handler, nullptr, nullptr));

    wifi_ap_record_t ap_info;
    if (esp_wifi_sta_get_ap_info(&ap_info) != ESP_OK)
    {
        ESP_LOGI(TAG, "WiFi is not connected. Waiting for connection before fetching forecast.");
        // Note: We don't clear the bits for further invokation
        xEventGroupWaitBits(s_wifi_event_group, WIFI_CONNECTED_BIT, pdFALSE, pdFALSE, portMAX_DELAY);
    }

    ESP_LOGI(TAG, "Fetching forecast");

    auto path = "/forecasts/v1/daily/5day/" + m_storage->get_string(s_weather_city_code);
    std::string query = "apikey=" + m_storage->get_string(s_weather_api_token) + "&metric=true";

    esp_http_client_config_t config = {};
    config.host = "dataservice.accuweather.com";
    config.path = path.c_str();
    config.query = query.c_str();
    config.method = HTTP_METHOD_GET;
    config.event_handler = http_event_handler,
    config.user_data = sg_local_response_buffer;
    config.disable_auto_redirect = true;

    esp_http_client_handle_t client = esp_http_client_init(&config);

    esp_err_t err = esp_http_client_perform(client);
    if (err != ESP_OK) {
        ESP_LOGE(TAG, "HTTP GET request failed: %s", esp_err_to_name(err));
        esp_http_client_cleanup(client);
        return true;
    }

    ESP_LOGI(TAG, "Query '%s' result. HTTP GET Status = %d, content_length = %" PRId64 " data: %s",
             query.c_str(),
             esp_http_client_get_status_code(client),
             esp_http_client_get_content_length(client),
             sg_local_response_buffer);

    if (esp_http_client_get_status_code(client) != 200) {
        ESP_LOGW(TAG, "Failed to update forecast");
        esp_http_client_cleanup(client);

        return true;
    }

    WeatherForecast forecast{};
    if (parse_forecast_json(sg_local_response_buffer,
                            esp_http_client_get_content_length(client),
                            forecast)) {
        EventBus::get_instance().pubish(EventBusEvent::WeatherForecastEvent, forecast, sizeof(forecast));
    }

    ESP_LOGI(TAG, "Online forecast updated");
    esp_http_client_cleanup(client);

    return true;
}

bool ForecastFetcher::parse_forecast_json(const char *json, int len, WeatherForecast result) {
    time_t now{};
    time(&now);

    jparse_ctx_t jctx;

    CHECK_PARSE(json_parse_start(&jctx, json, len), "root");

    int num_elem;
    CHECK_PARSE(json_obj_get_array(&jctx, "DailyForecasts", &num_elem), "DailyForecasts");

    size_t element_counter = 0;
    for (uint32_t i = 0; i < 5; ++i)
    {
        CHECK_PARSE(json_arr_get_object(&jctx, i), "PredArray");

        int64_t timestamp = 0;
        CHECK_PARSE(json_obj_get_int64(&jctx, "EpochDate", &timestamp), "EpochDate");

        // The API returns 4 A.M., so we need to adjust the timestamp to the start of the day
        // (Because user always think about the next day as the one starting at 0:00).
        // Plus, we add out timezone shift.
        timestamp -= 4 * 3600;
        timestamp += _timezone;

        if (timestamp < now)
        {
            ESP_LOGI(TAG, "Skipping forecast entry: Now: %lld. Ts: %lld", now, timestamp);
            json_arr_leave_object(&jctx);
            continue;
        }
        else
        {
            ESP_LOGI(TAG, "Adding forecast entry: Now: %lld. Ts: %lld", now, timestamp);
        }

        CHECK_PARSE(json_obj_get_object(&jctx, "Temperature"), "Temperature");

        CHECK_PARSE(json_obj_get_object(&jctx, "Minimum"), "Minimum");
        float min_temp = 0;
        CHECK_PARSE(json_obj_get_float(&jctx, "Value", &min_temp), "Value");

        json_obj_leave_object(&jctx);
        CHECK_PARSE(json_obj_get_object(&jctx, "Maximum"), "Maximum");
        float max_temp = 0;
        CHECK_PARSE(json_obj_get_float(&jctx, "Value", &max_temp), "Value");

        json_obj_leave_object(&jctx);
        json_obj_leave_object(&jctx);
        CHECK_PARSE(json_obj_get_object(&jctx, "Day"), "Day");

        int icon_type = 0;
        CHECK_PARSE(json_obj_get_int(&jctx, "Icon", &icon_type), "Icon");
        json_obj_leave_object(&jctx);
        auto icon = icon_to_type(icon_type);

        result[element_counter].temp_low = min_temp;
        result[element_counter].temp_high = max_temp;
        result[element_counter].icon_type = icon;
        element_counter++;

        json_arr_leave_object(&jctx);

        if (element_counter >= 3)
        {
            break;
        }
    }

    json_parse_end(&jctx);

    return true;
}

esp_err_t ForecastFetcher::http_event_handler(esp_http_client_event_t *evt)
{
    static char *output_buffer; // Buffer to store response of http request from event handler
    static int output_len;      // Stores number of bytes read
    int mbedtls_err = 0;
    esp_err_t err = ESP_OK;

    switch (evt->event_id)
    {
    case HTTP_EVENT_ERROR:
        ESP_LOGD(TAG, "HTTP_EVENT_ERROR");
        break;
    case HTTP_EVENT_ON_CONNECTED:
        ESP_LOGD(TAG, "HTTP_EVENT_ON_CONNECTED");
        break;
    case HTTP_EVENT_HEADER_SENT:
        ESP_LOGD(TAG, "HTTP_EVENT_HEADER_SENT");
        break;
    case HTTP_EVENT_ON_HEADER:
        ESP_LOGD(TAG, "HTTP_EVENT_ON_HEADER, key=%s, value=%s", evt->header_key, evt->header_value);
        break;
    case HTTP_EVENT_ON_DATA:
        ESP_LOGD(TAG, "HTTP_EVENT_ON_DATA, len=%d", evt->data_len);
        // Clean the buffer in case of a new request
        if (output_len == 0 && evt->user_data)
        {
            // we are just starting to copy the output data into the use
            memset(evt->user_data, 0, MAX_HTTP_OUTPUT_BUFFER);
        }
        /*
         *  Check for chunked encoding is added as the URL for chunked encoding used in this example returns binary data.
         *  However, event handler can also be used in case chunked encoding is used.
         */
        if (!esp_http_client_is_chunked_response(evt->client))
        {
            // If user_data buffer is configured, copy the response into the buffer
            int copy_len = 0;
            if (evt->user_data)
            {
                // The last byte in evt->user_data is kept for the NULL character in case of out-of-bound access.
                copy_len = std::min<int>(evt->data_len, (MAX_HTTP_OUTPUT_BUFFER - output_len));
                if (copy_len)
                {
                    memcpy((char *)evt->user_data + output_len, evt->data, copy_len);
                }
            }
            else
            {
                int content_len = esp_http_client_get_content_length(evt->client);
                if (output_buffer == NULL)
                {
                    // We initialize output_buffer with 0 because it is used by strlen() and similar functions therefore should be null terminated.
                    output_buffer = (char *)calloc(content_len + 1, sizeof(char));
                    output_len = 0;
                    if (output_buffer == NULL)
                    {
                        ESP_LOGE(TAG, "Failed to allocate memory for output buffer");
                        return ESP_FAIL;
                    }
                }
                copy_len = std::min(evt->data_len, (content_len - output_len));
                if (copy_len)
                {
                    memcpy(output_buffer + output_len, evt->data, copy_len);
                }
            }
            output_len += copy_len;
        }

        break;
    case HTTP_EVENT_ON_FINISH:
        ESP_LOGD(TAG, "HTTP_EVENT_ON_FINISH");
        if (output_buffer != NULL)
        {
            // Response is accumulated in output_buffer. Uncomment the below line to print the accumulated response
            // ESP_LOG_BUFFER_HEX(TAG, output_buffer, output_len);
            free(output_buffer);
            output_buffer = NULL;
        }
        output_len = 0;
        break;
    case HTTP_EVENT_DISCONNECTED:
        ESP_LOGI(TAG, "HTTP_EVENT_DISCONNECTED");
        err = esp_tls_get_and_clear_last_error((esp_tls_error_handle_t)evt->data, &mbedtls_err, NULL);
        if (err != 0)
        {
            ESP_LOGI(TAG, "Last esp error code: 0x%x", err);
            ESP_LOGI(TAG, "Last mbedtls failure: 0x%x", mbedtls_err);
        }
        if (output_buffer != NULL)
        {
            free(output_buffer);
            output_buffer = NULL;
        }
        output_len = 0;
        break;
    case HTTP_EVENT_REDIRECT:
        ESP_LOGD(TAG, "HTTP_EVENT_REDIRECT");
        esp_http_client_set_header(evt->client, "From", "user@example.com");
        esp_http_client_set_header(evt->client, "Accept", "text/html");
        esp_http_client_set_redirection(evt->client);
        break;
    }
    return ESP_OK;
}
