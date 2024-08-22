#pragma once

#include <ctime>

#include <esp_lvgl_port.h>
#include <esp_timer.h>

class TimeLabel
{
public:
    TimeLabel();
    ~TimeLabel();

    lv_obj_t *root(lv_obj_t *parent);
    void time_updated();

private:
    void start_timer();
    static void timer_callback(void *arg);
    static const char *day_of_week(int id);
    static const char *month(int id);

    time_t m_time = 0;
    tm m_timeinfo = {};

    lv_obj_t *m_grid = nullptr;
    lv_obj_t *m_time_label = nullptr;
    lv_obj_t *m_date_label = nullptr;
    esp_timer_handle_t m_timer_handle = nullptr;
};