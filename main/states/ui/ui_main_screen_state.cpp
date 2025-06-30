#include "ui_main_screen_state.h"

#include "esp_lvgl_port.h"

#include "ui/lvgl_meteo/screens/main/main_screen_gen.h"

bool UiMainScreenState::enter()
{
    if (m_main_screen == nullptr)
    {
        m_main_screen = main_screen_create();
    }

    m_button->register_callback(BUTTON_PRESS_DOWN, [this]() {
        switch_to(StateId::Prediction);
    });

    m_button->register_callback(BUTTON_LONG_PRESS_START, [this]() {
        switch_to(StateId::Provisioning);
    });

    // This is a magic to delete previous screen without LVGL crashing
    lv_screen_load_anim(m_main_screen, LV_SCR_LOAD_ANIM_NONE, 1, 1, true);

    // // auto active_screen = lv_screen_active();
    // // lv_screen_load(m_main_screen);
    // if (lvgl_port_lock(-1))
    // {
    //     lv_screen_load_anim(m_main_screen, LV_SCR_LOAD_ANIM_NONE, 1, 1, true);
    //     // lv_screen_load(m_main_screen);
    //     lvgl_port_unlock();
    // }

    // if (active_screen) {
    //     lv_obj_delete(active_screen);
    // }

    return true;
}

void UiMainScreenState::exit()
{
}