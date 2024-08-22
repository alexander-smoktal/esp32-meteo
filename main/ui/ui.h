#pragma once

#include <memory>

#include <esp_lcd_types.h>
#include <esp_lvgl_port.h>
#include <esp_err.h>

#include "ui/current_weather.h"
#include "ui/time_label.h"
#include "ui/forecast.h"
#include "ui/logo.h"

class UI
{
public:
    static const uint32_t LCD_H_RES = 240;
    static const uint32_t LCD_V_RES = 280;

    UI();
    void start_task();

    void time_updated();
    void set_outdoor_weather(WeatherIconType icon, float temp, float humidity);
    void set_indoor_weather(float temp, float humidity);
    void set_forecast(std::vector<ForecastDay> data);
    void splash_fade_out();

    void toggle_prediction(const char *prediction);
    void toggle_light();

private:
    esp_err_t lcd_init(void);
    esp_err_t lvgl_init(void);
    void draw_ui();
    void splash();

    std::unique_ptr<TimeLabel> m_time_label = nullptr;
    std::unique_ptr<CurrentWeather> m_current_weather = nullptr;
    std::unique_ptr<Forecast> m_forecast = nullptr;
    std::unique_ptr<Logo> m_logo = nullptr;

    esp_lcd_panel_handle_t lcd_panel = nullptr;
    lv_display_t *lvgl_disp = nullptr;
    esp_lcd_panel_io_handle_t lcd_io = nullptr;
    lv_obj_t *m_grid = nullptr;
    lv_obj_t *m_prediction = nullptr;
    bool m_backlight_on = true;
};