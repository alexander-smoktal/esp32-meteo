#pragma once

#include <esp_lvgl_port.h>

enum WeatherIconType
{
    Sunny,
    MostlyCloudy,
    Cloudy,
    ClearMoon,
    CloudyMoon,
    Rain,
    MostlyRain,
    HeavyRain,
    Snow,
    Wind,
    Storm,
    Fog,
    Sleet,
    Hot,
    Cold
};

class CurrentWeather
{
public:
    void set_outdoor(WeatherIconType icon, float temp, float humidity);
    void set_indoor(float temp, float humidity);
    static const char *icon_to_name(int icon);

    lv_obj_t *root(lv_obj_t *parent);

private:
    lv_obj_t *m_grid = nullptr;
    lv_obj_t *m_weather_icon = nullptr;
    lv_obj_t *m_in_temp_label = nullptr;
    lv_obj_t *m_in_hum_label = nullptr;
    lv_obj_t *m_out_temp_label = nullptr;
    lv_obj_t *m_out_hum_label = nullptr;
};