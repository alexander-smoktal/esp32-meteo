#include "current_weather.h"

#include <esp_log.h>

#include "fonts.h"
#include "ui.h"

static const char *TAG = "weather";

void CurrentWeather::set_outdoor(WeatherIconType icon, float temp, float humidity)
{
    ESP_LOGI("Outdoor", "%2.2f°C  %2.2f%%", temp, humidity);
    lvgl_port_lock(0);

    lv_label_set_text_fmt(m_out_temp_label, "%2.0f°", temp);
    lv_label_set_text_fmt(m_out_hum_label, "%2.0f%%h", humidity);

    lv_image_set_src(m_weather_icon, icon_to_name(icon));
    lv_image_set_scale(m_weather_icon, 220);

    lvgl_port_unlock();
}

void CurrentWeather::set_indoor(float temp, float humidity)
{
    ESP_LOGI("Indoor", "%2.2f°C  %2.2f%%", temp, humidity);

    lvgl_port_lock(0);

    lv_label_set_text_fmt(m_in_temp_label, "%2.0f°", temp);
    lv_label_set_text_fmt(m_in_hum_label, "%2.0f%%h", humidity);

    lvgl_port_unlock();
}

const char *CurrentWeather::icon_to_name(int icon)
{
    switch (icon)
    {
    case Sunny:
        return "F:/flash/icons/icons8-sunny-96.png";
    case MostlyCloudy:
        return "F:/flash/icons/icons8-cloudy-96.png";
    case Cloudy:
        return "F:/flash/icons/icons8-cloudy-f-96.png";
    case ClearMoon:
        return "F:/flash/icons/icons8-full-moon-96.png";
    case CloudyMoon:
        return "F:/flash/icons/icons8-cloudy-moon-96.png";
    case Rain:
        return "F:/flash/icons/icons8-rain-96.png";
    case MostlyRain:
        return "F:/flash/icons/icons8-rain-cloud-96.png";
    case HeavyRain:
        return "F:/flash/icons/icons8-heavy-rain-96.png";
    case Snow:
        return "F:/flash/icons/icons8-snow-96.png";
    case Wind:
        return "F:/flash/icons/icons8-windy-weather-96.png";
    case Storm:
        return "F:/flash/icons/icons8-storm-with-heavy-rain-96.png";
    case Fog:
        return "F:/flash/icons/icons8-fog-96.png";
    case Sleet:
        return "F:/flash/icons/icons8-sleet-96.png";
    case Hot:
        return "F:/flash/icons/icons8-hot-96.png";
    case Cold:
        return "F:/flash/icons/icons8-cold-96.png";
    }

    ESP_LOGE(TAG, "Unknown icon");
    return "F:/flash/icons/icons8-sunny-96.png";
}

lv_obj_t *CurrentWeather::root(lv_obj_t *parent)
{
    if (m_grid)
    {
        return m_grid;
    }

    lv_color_t text_color = {200, 200, 200};

    m_grid = lv_obj_create(parent);
    lv_obj_set_layout(m_grid, LV_LAYOUT_GRID);
    lv_obj_set_style_bg_opa(m_grid, 0, 0);
    lv_obj_set_style_border_opa(m_grid, 0, 0);
    lv_obj_set_scrollbar_mode(m_grid, LV_SCROLLBAR_MODE_OFF);
    lv_obj_set_style_pad_row(m_grid, 0, 0);
    lv_obj_set_style_pad_column(m_grid, 0, 0);

    static int32_t column_dsc[] = {95, 35, 55, 85, LV_GRID_TEMPLATE_LAST};
    static int32_t row_dsc[] = {10, 28, 48, LV_GRID_TEMPLATE_LAST};
    lv_obj_set_grid_dsc_array(m_grid, column_dsc, row_dsc);

    m_weather_icon = lv_image_create(m_grid);
    lv_image_set_align(m_weather_icon, LV_ALIGN_CENTER);
    lv_obj_set_grid_cell(m_weather_icon, LV_GRID_ALIGN_START, 0, 1, LV_GRID_ALIGN_START, 0, 3);

    auto nature_icon = lv_image_create(m_grid);
    lv_image_set_src(nature_icon, "F:/flash/icons/nature-25.png");
    lv_obj_set_grid_cell(nature_icon, LV_GRID_ALIGN_START, 1, 1, LV_GRID_ALIGN_CENTER, 1, 1);
    auto home_icon = lv_image_create(m_grid);
    lv_image_set_src(home_icon, "F:/flash/icons/house-25.png");
    lv_obj_set_grid_cell(home_icon, LV_GRID_ALIGN_START, 1, 1, LV_GRID_ALIGN_START, 2, 1);

    m_out_temp_label = lv_label_create(m_grid);
    lv_obj_set_style_text_align(m_out_temp_label, LV_TEXT_ALIGN_LEFT, 0);
    lv_obj_set_style_text_font(m_out_temp_label, &noto_24, 0);
    lv_obj_set_style_text_color(m_out_temp_label, text_color, 0);
    lv_label_set_text(m_out_temp_label, "--°");
    lv_obj_set_grid_cell(m_out_temp_label, LV_GRID_ALIGN_START, 2, 1, LV_GRID_ALIGN_CENTER, 1, 1);

    m_out_hum_label = lv_label_create(m_grid);
    lv_obj_set_style_text_align(m_out_hum_label, LV_TEXT_ALIGN_LEFT, 0);
    lv_obj_set_style_text_font(m_out_hum_label, &noto_24, 0);
    lv_obj_set_style_text_color(m_out_hum_label, text_color, 0);
    lv_label_set_text(m_out_hum_label, "--%h");
    lv_obj_set_grid_cell(m_out_hum_label, LV_GRID_ALIGN_START, 3, 1, LV_GRID_ALIGN_CENTER, 1, 1);

    m_in_temp_label = lv_label_create(m_grid);
    lv_obj_set_style_text_align(m_in_temp_label, LV_TEXT_ALIGN_LEFT, 0);
    lv_obj_set_style_text_font(m_in_temp_label, &noto_24, 0);
    lv_obj_set_style_text_color(m_in_temp_label, text_color, 0);
    lv_label_set_text(m_in_temp_label, "--°");
    lv_obj_set_grid_cell(m_in_temp_label, LV_GRID_ALIGN_START, 2, 1, LV_GRID_ALIGN_START, 2, 1);

    m_in_hum_label = lv_label_create(m_grid);
    lv_obj_set_style_text_align(m_in_hum_label, LV_TEXT_ALIGN_LEFT, 0);
    lv_obj_set_style_text_font(m_in_hum_label, &noto_24, 0);
    lv_obj_set_style_text_color(m_in_hum_label, text_color, 0);
    lv_label_set_text(m_in_hum_label, "--%h");
    lv_obj_set_grid_cell(m_in_hum_label, LV_GRID_ALIGN_START, 3, 1, LV_GRID_ALIGN_START, 2, 1);

    return m_grid;
}
