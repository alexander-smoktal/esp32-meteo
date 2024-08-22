#pragma once

#include <esp_lvgl_port.h>

class Logo
{
public:
    Logo();
    ~Logo();

    void attach(lv_obj_t *parent);
    void detach();

    void fade_in(uint32_t time);
    void fade_out(uint32_t time);

private:
    lv_obj_t *m_logo_image = nullptr;
};