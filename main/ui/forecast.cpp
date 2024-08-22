#include "forecast.h"

#include <ctime>
#include <esp_log.h>

#include "fonts.h"

static const uint64_t LAST_FULL_MOON = 1719004080;
static const uint64_t MOON_PERIOD = 2551443;

static const char *TAG = "forecast";

void Forecast::set_forecast(std::vector<ForecastDay> data)
{
    calculate_moon_phase();

    for (auto &pred : data)
    {
        ESP_LOGI(TAG, "Forecast: %d, %f, %f", pred.icon, pred.min, pred.max);
    }

    if (data.size() < 3)
    {
        ESP_LOGW(TAG, "Data size less than 3");
        return;
    }

    lvgl_port_lock(0);

    auto &data1 = data[0];
    lv_image_set_src(m_weather_1, CurrentWeather::icon_to_name(data1.icon));
    lv_image_set_scale(m_weather_1, 120);
    lv_label_set_text_fmt(m_weather_label_1, "%2.0f/%2.0f°", data1.max, data1.min);

    auto &data2 = data[1];
    lv_image_set_src(m_weather_2, CurrentWeather::icon_to_name(data2.icon));
    lv_image_set_scale(m_weather_2, 120);
    lv_label_set_text_fmt(m_weather_label_2, "%2.0f/%2.0f°", data2.max, data2.min);

    auto &data3 = data[2];
    lv_image_set_src(m_weather_3, CurrentWeather::icon_to_name(data3.icon));
    lv_image_set_scale(m_weather_3, 120);
    lv_label_set_text_fmt(m_weather_label_3, "%2.0f/%2.0f°", data3.max, data3.min);

    lvgl_port_unlock();
}

lv_obj_t *Forecast::root(lv_obj_t *parent)
{
    if (m_grid)
    {
        return m_grid;
    }

    lv_color_t text_color = {200, 200, 200};

    m_grid = lv_obj_create(parent);
    lv_obj_set_height(m_grid, 80);
    lv_obj_set_layout(m_grid, LV_LAYOUT_GRID);
    lv_obj_set_style_bg_opa(m_grid, 0, 0);
    lv_obj_set_style_border_opa(m_grid, 0, 0);
    lv_obj_set_scrollbar_mode(m_grid, LV_SCROLLBAR_MODE_OFF);
    lv_obj_set_style_pad_row(m_grid, 0, 0);
    lv_obj_set_style_pad_column(m_grid, 0, 0);

    static int32_t column_dsc[] = {55, 55, 55, 5, 100, LV_GRID_TEMPLATE_LAST};
    static int32_t row_dsc[] = {10, 60, LV_GRID_TEMPLATE_LAST};
    lv_obj_set_grid_dsc_array(m_grid, column_dsc, row_dsc);

    m_moon_phase = lv_image_create(m_grid);
    lv_image_set_align(m_moon_phase, LV_ALIGN_CENTER);
    lv_obj_set_grid_cell(m_moon_phase, LV_GRID_ALIGN_START, 4, 1, LV_GRID_ALIGN_START, 0, 2);
    lv_obj_set_height(m_moon_phase, 62);

    m_weather_1 = lv_image_create(m_grid);
    lv_image_set_align(m_weather_1, LV_ALIGN_CENTER);
    lv_obj_set_grid_cell(m_weather_1, LV_GRID_ALIGN_START, 0, 1, LV_GRID_ALIGN_CENTER, 1, 1);
    lv_obj_set_size(m_weather_1, 50, 100);

    m_weather_label_1 = lv_label_create(m_grid);
    lv_obj_set_style_text_align(m_weather_label_1, LV_TEXT_ALIGN_CENTER, 0);
    lv_obj_set_style_text_font(m_weather_label_1, &noto_15, 0);
    lv_obj_set_style_text_color(m_weather_label_1, text_color, 0);
    lv_label_set_text(m_weather_label_1, "");
    lv_obj_set_grid_cell(m_weather_label_1, LV_GRID_ALIGN_CENTER, 0, 1, LV_GRID_ALIGN_START, 0, 1);

    m_weather_2 = lv_image_create(m_grid);
    lv_image_set_align(m_weather_2, LV_ALIGN_CENTER);
    lv_obj_set_grid_cell(m_weather_2, LV_GRID_ALIGN_START, 1, 1, LV_GRID_ALIGN_CENTER, 1, 1);
    lv_obj_set_size(m_weather_2, 50, 100);

    m_weather_label_2 = lv_label_create(m_grid);
    lv_obj_set_style_text_align(m_weather_label_2, LV_TEXT_ALIGN_CENTER, 0);
    lv_obj_set_style_text_font(m_weather_label_2, &noto_15, 0);
    lv_obj_set_style_text_color(m_weather_label_2, text_color, 0);
    lv_label_set_text(m_weather_label_2, "");
    lv_obj_set_grid_cell(m_weather_label_2, LV_GRID_ALIGN_CENTER, 1, 1, LV_GRID_ALIGN_START, 0, 1);

    m_weather_3 = lv_image_create(m_grid);
    lv_image_set_align(m_weather_3, LV_ALIGN_CENTER);
    lv_obj_set_grid_cell(m_weather_3, LV_GRID_ALIGN_START, 2, 1, LV_GRID_ALIGN_CENTER, 1, 1);
    lv_obj_set_size(m_weather_3, 50, 100);

    m_weather_label_3 = lv_label_create(m_grid);
    lv_obj_set_style_text_align(m_weather_label_3, LV_TEXT_ALIGN_CENTER, 0);
    lv_obj_set_style_text_font(m_weather_label_3, &noto_15, 0);
    lv_obj_set_style_text_color(m_weather_label_3, text_color, 0);
    lv_label_set_text(m_weather_label_3, "");
    lv_obj_set_grid_cell(m_weather_label_3, LV_GRID_ALIGN_CENTER, 2, 1, LV_GRID_ALIGN_START, 0, 1);

    return m_grid;
}

void Forecast::calculate_moon_phase()
{
    ESP_LOGI(TAG, "Calculatin moon phase");

    time_t now;
    time(&now);

    if (now < LAST_FULL_MOON)
    {
        ESP_LOGW(TAG, "Time is less than start");
        return;
    }

    float diff = now - LAST_FULL_MOON;
    auto percentage = uint64_t(diff / MOON_PERIOD * 100.0) % 100;
    ESP_LOGI(TAG, "Moon phase: %lld", percentage);

    lvgl_port_lock(0);
    if (percentage < 7 || percentage >= 94)
    {
        lv_image_set_src(m_moon_phase, "F:/flash/icons/icons8-full-moon-96.png");
    }
    else if (percentage < 19)
    {
        lv_image_set_src(m_moon_phase, "F:/flash/icons/icons8-waxing-gibbous-96-flip.png");
    }
    else if (percentage < 32)
    {
        lv_image_set_src(m_moon_phase, "F:/flash/icons/icons8-first-quarter-96-flip.png");
    }
    else if (percentage < 44)
    {
        lv_image_set_src(m_moon_phase, "F:/flash/icons/icons8-waxing-crescent-96-flip.png");
    }
    else if (percentage < 56)
    {
        lv_image_set_src(m_moon_phase, "F:/flash/icons/icons8-new-moon-96.png");
    }
    else if (percentage < 69)
    {
        lv_image_set_src(m_moon_phase, "F:/flash/icons/icons8-waxing-crescent-96.png");
    }
    else if (percentage < 82)
    {
        lv_image_set_src(m_moon_phase, "F:/flash/icons/icons8-first-quarter-96.png");
    }
    else
    {
        lv_image_set_src(m_moon_phase, "F:/flash/icons/icons8-waxing-gibbous-96.png");
    }

    lv_image_set_scale(m_moon_phase, 200);
    lvgl_port_unlock();
}