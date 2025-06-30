#pragma once

#include "lvgl.h"

class Display {
public:
    Display();

private:
    void start_splash();

    lv_display_t *m_display = nullptr;
};
