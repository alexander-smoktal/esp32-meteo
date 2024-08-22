#pragma once

#include <tuple>

#include <esp_timer.h>
#include <esp_http_client.h>

#include "ui/ui.h"
#include "ui/current_weather.h"
#include "ui/forecast.h"
#include "sensor.h"

class Updater
{
public:
    Updater(UI *ui);

    void on_connected(bool connected);

private:
    static WeatherIconType icon_to_type(int icon);
    static esp_err_t http_event_handler(esp_http_client_event_t *evt);
    static std::tuple<WeatherIconType, float, float> parse_weather_json(const char *json, int len);
    static std::vector<ForecastDay> parse_forecast_json(const char *json, int len);

    void task_cb(void *ctc);
    static void update_time_cb(void *ctx);
    static void update_weather_cb(void *ctx);
    static void update_in_house_cb(void *ctx);
    static void update_forecast_cb(void *ctx);

    static bool read_cache(const char *path);
    static void write_cache(const char *path, const char *cache, size_t cache_len,
                            const char *cache_tm_file, uint32_t upd_time);

    UI *m_ui;
    Sensor m_sensor = {};

    esp_timer_handle_t m_time_timer = nullptr;
    esp_timer_handle_t m_weather_timer = nullptr;
    esp_timer_handle_t m_in_house_timer = nullptr;
    esp_timer_handle_t m_forecast_timer = nullptr;

    uint32_t m_last_forecast_updated_date = 0;
    uint32_t m_last_weather_updated_hour = 0;
};