#pragma once

#include <vector>

#include "current_weather.h"

struct ForecastDay
{
    WeatherIconType icon;
    float min;
    float max;
};

class Forecast
{
public:
    void set_forecast(std::vector<ForecastDay> data);

    lv_obj_t *root(lv_obj_t *parent);

private:
    void calculate_moon_phase();

    lv_obj_t *m_grid = nullptr;
    lv_obj_t *m_weather_1 = nullptr;
    lv_obj_t *m_weather_label_1 = nullptr;
    lv_obj_t *m_weather_2 = nullptr;
    lv_obj_t *m_weather_label_2 = nullptr;
    lv_obj_t *m_weather_3 = nullptr;
    lv_obj_t *m_weather_label_3 = nullptr;
    lv_obj_t *m_moon_phase = nullptr;
};