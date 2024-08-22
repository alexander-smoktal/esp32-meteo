#include "time_label.h"

#include "ui.h"

#include "fonts.h"

#include <string>
#include <esp_log.h>

// static const char *TAG = "time label";

TimeLabel::TimeLabel()
{
}

TimeLabel::~TimeLabel()
{
    esp_timer_stop(m_timer_handle);
}

lv_obj_t *TimeLabel::root(lv_obj_t *parent)
{
    if (m_grid)
    {
        return m_grid;
    }

    m_grid = lv_obj_create(parent);
    lv_obj_set_height(m_grid, 120);
    lv_obj_set_width(m_grid, UI::LCD_V_RES);
    lv_obj_set_style_bg_opa(m_grid, 0, 0);
    lv_obj_set_style_border_opa(m_grid, 0, 0);
    lv_obj_set_scrollbar_mode(m_grid, LV_SCROLLBAR_MODE_OFF);

    lv_color_t time_color = {235, 235, 235};
    m_time_label = lv_label_create(m_grid);
    lv_obj_set_style_text_align(m_time_label, LV_TEXT_ALIGN_CENTER, 0);
    lv_obj_set_style_text_font(m_time_label, &noto_60, 0);
    lv_obj_set_style_text_color(m_time_label, time_color, 0);
    lv_obj_align(m_time_label, LV_ALIGN_TOP_MID, 0, -20);
    lv_obj_set_width(m_time_label, UI::LCD_V_RES);
    lv_label_set_text(m_time_label, "");

    lv_color_t date_color = {200, 200, 200};
    m_date_label = lv_label_create(m_grid);
    lv_obj_set_style_text_align(m_date_label, LV_TEXT_ALIGN_CENTER, 0);
    lv_obj_set_style_text_font(m_date_label, &noto_24, 0);
    lv_obj_set_style_text_color(m_date_label, date_color, 0);
    lv_obj_align(m_date_label, LV_ALIGN_TOP_MID, 5, 40);
    lv_obj_set_width(m_date_label, UI::LCD_V_RES);
    lv_label_set_text(m_date_label, "");

    start_timer();

    return m_grid;
}

void TimeLabel::start_timer()
{
    esp_timer_create_args_t timer_args = {};
    timer_args.callback = &TimeLabel::timer_callback;
    timer_args.arg = this;
    timer_args.name = "time tick";

    ESP_ERROR_CHECK(esp_timer_create(&timer_args, &m_timer_handle));
    ESP_ERROR_CHECK(esp_timer_start_periodic(m_timer_handle, 1000000));
}

void TimeLabel::time_updated()
{
    time(&m_time);
    esp_timer_restart(m_timer_handle, 1000000);
}

void TimeLabel::timer_callback(void *arg)
{
    auto self = (TimeLabel *)arg;
    if (self->m_time == 0 && !self->m_time_label)
        return;

    time(&self->m_time);
    localtime_r(&self->m_time, &self->m_timeinfo);

    lvgl_port_lock(0);

    lv_label_set_text_fmt(self->m_time_label, "%02d:%02d:%02d",
                          self->m_timeinfo.tm_hour,
                          self->m_timeinfo.tm_min,
                          self->m_timeinfo.tm_sec);

    lv_label_set_text_fmt(self->m_date_label, "%s, %02d %s ",
                          day_of_week(self->m_timeinfo.tm_wday),
                          self->m_timeinfo.tm_mday,
                          month(self->m_timeinfo.tm_mon));

    lvgl_port_unlock();

    self->m_time++;
}

const char *TimeLabel::day_of_week(int id)
{
    static const char *days[] = {
        "Неділя", "Понеділок", "Вівторок", "Середа", "Четвер", "Пʼятниця", "Субота"};

    return days[id];
}

const char *TimeLabel::month(int id)
{
    static const char *months[] = {
        "Січня",
        "Лютого",
        "Березня",
        "Квітня",
        "Травня",
        "Червня",
        "Липня",
        "Серпня",
        "Вересня",
        "Жовтня",
        "Листопада",
        "Грудня"};

    return months[id];
}