#include "updater.h"

#include <fstream>

#include <esp_log.h>
#include <esp_err.h>
#include <esp_tls.h>
#include <esp_netif_sntp.h>
#include <esp_wifi.h>
#include <json_parser.h>

#include "flash.h"
#include "sensor.h"

static const uint32_t MAX_HTTP_OUTPUT_BUFFER = 20000;
static char sg_local_response_buffer[MAX_HTTP_OUTPUT_BUFFER + 1] = {0};

static const bool USE_CACHED_DATA = false;

static const char *TAG = "updater";
static const char *WEATHER_UPDATED_FILE = "last_weather_hour.txt";
static const char *FORECAST_UPDATED_FILE = "last_forecast_date.txt";
static const char *WEATHER_CACHE_FILE = "weather.json";
static const char *FORECAST_CACHE_FILE = "forecast.json";

static const uint64_t ONE_SEC_IN_mSEC = 1000000;               // One second
static const uint64_t ONE_MIN_IN_mSEC = 60 * ONE_SEC_IN_mSEC;  // One minute
static const uint64_t ONE_HOUR_IN_mSEC = 60 * ONE_MIN_IN_mSEC; // One hour
static const uint64_t TIMER_EXEC_NOW = 20000;                  // Init delay

Updater::Updater(UI *ui) : m_ui(ui)
{
    auto weather_update_file = Flash::open_file(WEATHER_UPDATED_FILE, std::fstream::in);
    if (weather_update_file.good())
    {
        weather_update_file >> m_last_weather_updated_hour;
        ESP_LOGI(TAG, "Weather cache hour: %d", (int)m_last_weather_updated_hour);
    }

    auto forecast_update_file = Flash::open_file(FORECAST_UPDATED_FILE, std::fstream::in);
    if (forecast_update_file.good())
    {
        forecast_update_file >> m_last_forecast_updated_date;
        ESP_LOGI(TAG, "Forecast cache date: %d", (int)m_last_forecast_updated_date);
    }
}

void Updater::on_connected(bool connected)
{
    ESP_LOGI(TAG, "WiFi connected: %d", connected);

    if (connected)
    {
        m_ui->splash_fade_out();

        if (m_time_timer != nullptr)
        {
            ESP_LOGE(TAG, "Active timer while not connected");
            return;
        }

        esp_timer_create_args_t timer_args = {};
        timer_args.callback = &update_time_cb;
        timer_args.arg = this;
        timer_args.name = "time update";

        ESP_ERROR_CHECK(esp_timer_create(&timer_args, &m_time_timer));
        ESP_ERROR_CHECK(esp_timer_start_periodic(m_time_timer, TIMER_EXEC_NOW));

        timer_args.callback = &update_weather_cb;
        timer_args.arg = this;
        timer_args.name = "weather update";

        ESP_ERROR_CHECK(esp_timer_create(&timer_args, &m_weather_timer));
        ESP_ERROR_CHECK(esp_timer_start_periodic(m_weather_timer, TIMER_EXEC_NOW - 1));

        timer_args.callback = &update_in_house_cb;
        timer_args.arg = this;
        timer_args.name = "in house update";

        ESP_ERROR_CHECK(esp_timer_create(&timer_args, &m_in_house_timer));
        ESP_ERROR_CHECK(esp_timer_start_periodic(m_in_house_timer, TIMER_EXEC_NOW - 3));

        timer_args.callback = &update_forecast_cb;
        timer_args.arg = this;
        timer_args.name = "forecast update";

        ESP_ERROR_CHECK(esp_timer_create(&timer_args, &m_forecast_timer));
        ESP_ERROR_CHECK(esp_timer_start_periodic(m_forecast_timer, TIMER_EXEC_NOW - 2));
    }
    else
    {
        if (m_time_timer == nullptr)
        {
            ESP_LOGE(TAG, "No timer running when disconnected");
            return;
        }

        esp_timer_stop(m_time_timer);
        esp_timer_delete(m_time_timer);
        m_time_timer = nullptr;

        esp_timer_stop(m_weather_timer);
        esp_timer_delete(m_weather_timer);
        m_weather_timer = nullptr;

        esp_timer_stop(m_forecast_timer);
        esp_timer_delete(m_forecast_timer);
        m_forecast_timer = nullptr;
    }
}

void Updater::update_time_cb(void *ctx)
{
    auto self = (Updater *)ctx;

    ESP_LOGI(TAG, "Updating time");
    ESP_LOGI(TAG, "Time start mem: %d", heap_caps_get_free_size(MALLOC_CAP_8BIT));

    if (esp_netif_sntp_sync_wait(pdMS_TO_TICKS(10000)) == ESP_OK)
    {
        ESP_LOGI(TAG, "NTP time updated successfully");
        self->m_ui->time_updated();
        ESP_ERROR_CHECK(esp_timer_restart(self->m_time_timer, ONE_HOUR_IN_mSEC));
    }
    else
    {
        ESP_LOGW(TAG, "Failed to update NTP time. Retrying");
        ESP_ERROR_CHECK(esp_timer_restart(self->m_time_timer, ONE_SEC_IN_mSEC * 5));
    }

    ESP_LOGI(TAG, "Time end mem: %d", heap_caps_get_free_size(MALLOC_CAP_8BIT));
}

void Updater::update_in_house_cb(void *ctx)
{
    ESP_LOGI(TAG, "In house start mem: %d", heap_caps_get_free_size(MALLOC_CAP_8BIT));

    auto self = (Updater *)ctx;

    ESP_LOGI(TAG, "Updating inhouse condidions");

    auto [temp, humid] = self->m_sensor.get_values();
    self->m_ui->set_indoor_weather(temp, humid);

    ESP_ERROR_CHECK(esp_timer_restart(self->m_in_house_timer, ONE_MIN_IN_mSEC));

    ESP_LOGI(TAG, "In house end mem: %d", heap_caps_get_free_size(MALLOC_CAP_8BIT));
}

void Updater::update_weather_cb(void *ctx)
{
    auto self = (Updater *)ctx;
    ESP_LOGI(TAG, "Updating weather");
    ESP_LOGI(TAG, "Weather start mem: %d", heap_caps_get_free_size(MALLOC_CAP_8BIT));

    ESP_ERROR_CHECK(esp_timer_restart(self->m_weather_timer, ONE_MIN_IN_mSEC * 10));

    time_t now{};
    time(&now);

    tm timeinfo{};
    localtime_r(&now, &timeinfo);

    if (USE_CACHED_DATA || timeinfo.tm_hour == self->m_last_weather_updated_hour)
    {
        ESP_LOGI(TAG, "Using weather cache");

        if (!read_cache(WEATHER_CACHE_FILE))
        {
            ESP_LOGW(TAG, "Failed to update weather");
            return;
        }

        const auto [icon, temp, humid] = parse_weather_json(sg_local_response_buffer, MAX_HTTP_OUTPUT_BUFFER);

        self->m_ui->set_outdoor_weather(icon, temp, humid);

        ESP_LOGI(TAG, "Weather updated");
    }
    else
    {
        ESP_LOGI(TAG, "Fetching weather");

        esp_http_client_config_t config = {};
        config.host = "dataservice.accuweather.com";
        config.path = "/currentconditions/v1/324505";
        config.query = "apikey=uPX9ugSLpwqa4ASlqua03xeFVko417nQ&details=true";
        config.method = HTTP_METHOD_GET;
        config.event_handler = http_event_handler,
        config.user_data = sg_local_response_buffer;
        config.disable_auto_redirect = true;

        esp_http_client_handle_t client = esp_http_client_init(&config);

        esp_err_t err = esp_http_client_perform(client);
        if (err != ESP_OK)
        {
            ESP_LOGE(TAG, "HTTP GET request failed: %s", esp_err_to_name(err));
            esp_http_client_cleanup(client);
            return;
        }

        ESP_LOGI(TAG, "HTTP GET Status = %d, content_length = %" PRId64 " data: %s",
                 esp_http_client_get_status_code(client),
                 esp_http_client_get_content_length(client),
                 sg_local_response_buffer);

        if (esp_http_client_get_status_code(client) != 200)
        {
            ESP_LOGW(TAG, "Failed to update weather");
            esp_http_client_cleanup(client);
            return;
        }

        self->m_last_weather_updated_hour = timeinfo.tm_hour;

        // Skip outer list
        write_cache(WEATHER_CACHE_FILE, sg_local_response_buffer + 1,
                    esp_http_client_get_content_length(client) - 2,
                    WEATHER_UPDATED_FILE, self->m_last_weather_updated_hour);

        const auto [icon, temp, humid] = parse_weather_json(sg_local_response_buffer + 1,
                                                            esp_http_client_get_content_length(client) - 2);

        self->m_ui->set_outdoor_weather(icon, temp, humid);

        ESP_LOGI(TAG, "Online weather updated");
        esp_http_client_cleanup(client);
    }

    ESP_LOGI(TAG, "Weather end mem: %d", heap_caps_get_free_size(MALLOC_CAP_8BIT));
}

void Updater::update_forecast_cb(void *ctx)
{
    auto self = (Updater *)ctx;
    ESP_LOGI(TAG, "Updating forecast");
    ESP_LOGI(TAG, "Pred start mem: %d", heap_caps_get_free_size(MALLOC_CAP_8BIT));

    ESP_ERROR_CHECK(esp_timer_restart(self->m_forecast_timer, ONE_MIN_IN_mSEC * 10));

    time_t now{};
    time(&now);

    tm timeinfo{};
    localtime_r(&now, &timeinfo);

    if (USE_CACHED_DATA || timeinfo.tm_mday == self->m_last_forecast_updated_date)
    {
        ESP_LOGI(TAG, "Using forecast cache");

        if (!read_cache(FORECAST_CACHE_FILE))
        {
            ESP_LOGW(TAG, "Failed to update forecast");
            return;
        }

        const auto data = parse_forecast_json(sg_local_response_buffer, MAX_HTTP_OUTPUT_BUFFER);

        self->m_ui->set_forecast(data);

        ESP_LOGI(TAG, "Forecast updated");
    }
    else
    {
        ESP_LOGI(TAG, "Fetching forecast");

        esp_http_client_config_t config = {};
        config.host = "dataservice.accuweather.com";
        config.path = "/forecasts/v1/daily/5day/324505";
        config.query = "apikey=uPX9ugSLpwqa4ASlqua03xeFVko417nQ&metric=true";
        config.method = HTTP_METHOD_GET;
        config.event_handler = http_event_handler,
        config.user_data = sg_local_response_buffer;
        config.disable_auto_redirect = true;

        esp_http_client_handle_t client = esp_http_client_init(&config);

        esp_err_t err = esp_http_client_perform(client);
        if (err != ESP_OK)
        {
            ESP_LOGE(TAG, "HTTP GET request failed: %s", esp_err_to_name(err));
            esp_http_client_cleanup(client);
            return;
        }

        ESP_LOGI(TAG, "HTTP GET Status = %d, content_length = %" PRId64 " data: %s",
                 esp_http_client_get_status_code(client),
                 esp_http_client_get_content_length(client),
                 sg_local_response_buffer);

        if (esp_http_client_get_status_code(client) != 200)
        {
            ESP_LOGW(TAG, "Failed to update forecast");
            esp_http_client_cleanup(client);
            return;
        }

        self->m_last_forecast_updated_date = timeinfo.tm_mday;
        write_cache(FORECAST_CACHE_FILE, sg_local_response_buffer,
                    esp_http_client_get_content_length(client), FORECAST_UPDATED_FILE, self->m_last_forecast_updated_date);

        const auto data = parse_forecast_json(sg_local_response_buffer,
                                              esp_http_client_get_content_length(client));

        self->m_ui->set_forecast(data);

        ESP_LOGI(TAG, "Online forecast updated");
        esp_http_client_cleanup(client);
    }

    ESP_LOGI(TAG, "Pred end mem: %d", heap_caps_get_free_size(MALLOC_CAP_8BIT));
}

bool Updater::read_cache(const char *path)
{
    auto file = Flash::open_file(path, std::fstream::in);

    if (!file.good())
    {
        ESP_LOGW(TAG, "Failed to read cache at: %s", path);
        return false;
    }

    file.get(sg_local_response_buffer, MAX_HTTP_OUTPUT_BUFFER, '\0');

    if (!file.eof())
    {
        ESP_LOGW(TAG, "Failed to read full cache file into buffer");
        // return false;
    }

    // ESP_LOGI(TAG, "Cache data: %s", sg_local_response_buffer);

    return true;
}

void Updater::write_cache(const char *path, const char *cache, size_t cache_len,
                          const char *cache_tm_file, uint32_t upd_time)
{
    auto file = Flash::open_file(path, std::fstream::out | std::fstream::trunc);

    if (!file.good())
    {
        ESP_LOGW(TAG, "Failed to open cache at: %s", path);
        return;
    }

    file.write(cache, cache_len);
    file.close();

    auto tm_file = Flash::open_file(cache_tm_file, std::fstream::out | std::fstream::trunc);

    if (!tm_file.good())
    {
        ESP_LOGW(TAG, "Failed to open tm file at: %s", cache_tm_file);
        return;
    }

    tm_file << upd_time;

    ESP_LOGI(TAG, "Succesfully updated %s cache at %d", path, (int)upd_time);
}

#define CHECK_PARSE(func, ctx)                                                        \
    do                                                                                \
    {                                                                                 \
        auto res = func;                                                              \
        if (res != OS_SUCCESS)                                                        \
        {                                                                             \
            ESP_LOGE("weather JSON parser", "Failed to parse %s JSON: %d", ctx, res); \
            return result;                                                            \
        }                                                                             \
    } while (0)

std::tuple<WeatherIconType, float, float>
Updater::parse_weather_json(const char *json, int len)
{
    auto result = std::tuple<WeatherIconType, float, float>{WeatherIconType::Sunny, 0, 0};
    jparse_ctx_t jctx;

    CHECK_PARSE(json_parse_start(&jctx, json, len), "root");

    int icon_type = 0;
    CHECK_PARSE(json_obj_get_int(&jctx, "WeatherIcon", &icon_type), "WeatherIcon");

    float humidity = 0;
    CHECK_PARSE(json_obj_get_float(&jctx, "RelativeHumidity", &humidity), "RelativeHumidity");

    CHECK_PARSE(json_obj_get_object(&jctx, "Temperature"), "Temperature");
    CHECK_PARSE(json_obj_get_object(&jctx, "Metric"), "Metric");

    float temp = 0;
    CHECK_PARSE(json_obj_get_float(&jctx, "Value", &temp), "Value");

    json_parse_end(&jctx);

    return {icon_to_type(icon_type), temp, humidity};
}

std::vector<ForecastDay> Updater::parse_forecast_json(const char *json, int len)
{
    time_t now{};
    time(&now);

    std::vector<ForecastDay> result{};
    jparse_ctx_t jctx;

    CHECK_PARSE(json_parse_start(&jctx, json, len), "root");

    int num_elem;
    CHECK_PARSE(json_obj_get_array(&jctx, "DailyForecasts", &num_elem), "DailyForecasts");

    for (uint32_t i = 0; i < 5; ++i)
    {
        CHECK_PARSE(json_arr_get_object(&jctx, i), "PredArray");

        int64_t timestamp = 0;
        CHECK_PARSE(json_obj_get_int64(&jctx, "EpochDate", &timestamp), "EpochDate");

        // For some reason API returns +4 hours timestamp.
        // Also, we have out timezone, so we adjust
        timestamp -= 25200;
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

        result.push_back(ForecastDay{icon, min_temp, max_temp});
        json_arr_leave_object(&jctx);

        if (result.size() >= 3)
        {
            break;
        }
    }

    json_parse_end(&jctx);

    return result;
}

WeatherIconType Updater::icon_to_type(int icon)
{
    switch (icon)
    {
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

esp_err_t
Updater::http_event_handler(esp_http_client_event_t *evt)
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
                copy_len = MIN(evt->data_len, (MAX_HTTP_OUTPUT_BUFFER - output_len));
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
                copy_len = MIN(evt->data_len, (content_len - output_len));
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