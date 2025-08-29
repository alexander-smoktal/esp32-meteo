#include "weather_fetcher.h"

#include <algorithm>

#include "freertos/FreeRTOS.h"
#include "freertos/event_groups.h"

#include "esp_log.h"
#include "esp_wifi.h"
#include "esp_event.h"
#include "esp_tls.h"

#include "json_parser.h"

#include "../event_bus.h"
#include "common.h"

static constexpr auto TAG = "Weather Fetcher";

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
            return result;                                          \
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
    WeatherIconType icon_to_condition(const std::string &icon) {
        const auto code = icon.substr(0, 2);

        if (icon == "01d" || icon == "01n"){
            // Check if it's night (ends with 'n') for moon
            return (icon.back() == 'n') ? WeatherIconType::Moon : WeatherIconType::Sunny;
        } else if (code == "02") {
            return WeatherIconType::MostlyCloudy;
        } else if (code == "03") {
            return WeatherIconType::Wind;
        } else if (code == "04") {
            return WeatherIconType::Cloudy;
        } else if (code == "09") {
            return WeatherIconType::HeavyRain;
        } else if (code == "10") {
            return WeatherIconType::MostlyRain;
        } else if (code == "11") {
            return WeatherIconType::Storm;
        } else if (code == "13") {
            return WeatherIconType::Snow;
        } else if (code == "50") {
            return WeatherIconType::Fog;
        }

        // Default fallback
        return WeatherIconType::Sunny;
    }
}

WeatherFetcher::WeatherFetcher(std::shared_ptr<NVStorage> storage)
    : m_storage(std::move(storage)) {}

bool WeatherFetcher::execute() {
    ESP_ERROR_CHECK(esp_event_handler_instance_register(IP_EVENT, IP_EVENT_STA_GOT_IP, &event_handler, nullptr, nullptr));

    wifi_ap_record_t ap_info;
    if (esp_wifi_sta_get_ap_info(&ap_info) != ESP_OK) {
        ESP_LOGI(TAG, "WiFi is not connected. Waiting for connection before fetching weather.");
        // Note: We don't clear the bits for further invokation
        xEventGroupWaitBits(s_wifi_event_group, WIFI_CONNECTED_BIT, pdFALSE, pdFALSE, portMAX_DELAY);
    }

    ESP_LOGI(TAG, "Fetching weather");

    std::string path = "/data/2.5/weather";
    auto query = "units=metric&q=" + m_storage->get_string(NVS_WEATHER_CITY_KEY) + "&appid=" + m_storage->get_string(NVS_WEATHER_API_TOKEN_KEY);

    esp_http_client_config_t config = {};
    config.host = "api.openweathermap.org";
    config.path = path.c_str();
    config.query = query.c_str();
    config.method = HTTP_METHOD_GET;
    config.event_handler = &WeatherFetcher::http_event_handler,
    config.user_data = sg_local_response_buffer;
    config.disable_auto_redirect = true;

    esp_http_client_handle_t client = esp_http_client_init(&config);

    esp_err_t err = esp_http_client_perform(client);
    if (err != ESP_OK) {
        ESP_LOGE(TAG, "HTTP GET request failed: %s", esp_err_to_name(err));
        esp_http_client_cleanup(client);
        return false;
    }

    ESP_LOGI(TAG, "Query '%s' result. HTTP GET Status = %d, content_length = %" PRId64 " data: %s",
             query.c_str(),
             esp_http_client_get_status_code(client),
             esp_http_client_get_content_length(client),
             sg_local_response_buffer);

    if (esp_http_client_get_status_code(client) != 200) {
        ESP_LOGW(TAG, "Failed to update weather");
        esp_http_client_cleanup(client);

        return false;
    }


    const auto current_weather = parse_weather_json(sg_local_response_buffer,
                                                    esp_http_client_get_content_length(client));

    EventBus::get_instance().publish(EventBusEvent::CurrentWeatherEvent, &current_weather, sizeof(current_weather));

    ESP_LOGI(TAG, "Online weather updated to: Icon %s, Temp: %f, Humidity: %f",
             to_string(current_weather.icon_type).c_str(), current_weather.temperature, current_weather.humidity);
    esp_http_client_cleanup(client);

    return true;
}

WeatherData WeatherFetcher::parse_weather_json(const char *json, int len) {
    auto result = WeatherData{WeatherIconType::Sunny, 0, 0};
    jparse_ctx_t jctx;

    CHECK_PARSE(json_parse_start(&jctx, json, len), "root");

    std::string icon_type{};
    icon_type.resize(10);
    int num_elem;
    CHECK_PARSE(json_obj_get_array(&jctx, "weather", &num_elem), "weather");
    CHECK_PARSE(json_arr_get_object(&jctx, 0), "weather_icon_obj");
    CHECK_PARSE(json_obj_get_string(&jctx, "icon", icon_type.data(), icon_type.size()), "icon");
    json_arr_leave_object(&jctx);
    json_obj_leave_array(&jctx);

    CHECK_PARSE(json_obj_get_object(&jctx, "main"), "main");
    float humidity = 0;
    CHECK_PARSE(json_obj_get_float(&jctx, "humidity", &humidity), "humidity");
    float temp = 0;
    CHECK_PARSE(json_obj_get_float(&jctx, "temp", &temp), "temp");
    json_obj_leave_object(&jctx);

    json_parse_end(&jctx);

    auto icon = icon_to_condition(icon_type);

    if (icon == WeatherIconType::Sunny) {
        if (temp >= 35) {
            icon = WeatherIconType::Hot;
        } else if (temp <= -10) {
            icon = WeatherIconType::Cold;
        }
    }

    return {icon, temp, humidity};
}

esp_err_t WeatherFetcher::http_event_handler(esp_http_client_event_t *evt) {
    static char *output_buffer; // Buffer to store response of http request from event handler
    static int output_len;      // Stores number of bytes read
    int mbedtls_err = 0;
    esp_err_t err = ESP_OK;

    switch (evt->event_id) {
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
        if (output_len == 0 && evt->user_data) {
            // we are just starting to copy the output data into the use
            memset(evt->user_data, 0, MAX_HTTP_OUTPUT_BUFFER);
        }
        /*
         *  Check for chunked encoding is added as the URL for chunked encoding used in this example returns binary data.
         *  However, event handler can also be used in case chunked encoding is used.
         */
        if (!esp_http_client_is_chunked_response(evt->client)) {
            // If user_data buffer is configured, copy the response into the buffer
            int copy_len = 0;
            if (evt->user_data) {
                // The last byte in evt->user_data is kept for the NULL character in case of out-of-bound access.
                copy_len = std::min<int>(evt->data_len, (MAX_HTTP_OUTPUT_BUFFER - output_len));
                if (copy_len) {
                    memcpy((char *)evt->user_data + output_len, evt->data, copy_len);
                }
            } else {
                int content_len = esp_http_client_get_content_length(evt->client);
                if (output_buffer == NULL) {
                    // We initialize output_buffer with 0 because it is used by strlen() and similar functions therefore should be null terminated.
                    output_buffer = (char *)calloc(content_len + 1, sizeof(char));
                    output_len = 0;
                    if (output_buffer == NULL) {
                        ESP_LOGE(TAG, "Failed to allocate memory for output buffer");
                        return ESP_FAIL;
                    }
                }
                copy_len = std::min(evt->data_len, (content_len - output_len));
                if (copy_len) {
                    memcpy(output_buffer + output_len, evt->data, copy_len);
                }
            }
            output_len += copy_len;
        }

        break;
    case HTTP_EVENT_ON_FINISH:
        ESP_LOGD(TAG, "HTTP_EVENT_ON_FINISH");
        if (output_buffer != NULL) {
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
        if (err != 0) {
            ESP_LOGI(TAG, "Last esp error code: 0x%x", err);
            ESP_LOGI(TAG, "Last mbedtls failure: 0x%x", mbedtls_err);
        }

        if (output_buffer != NULL) {
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