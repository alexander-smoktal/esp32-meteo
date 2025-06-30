#pragma once

#include "lvgl/lvgl.h"

#include "../state.h"
#include "io/button.h"

class UiMainScreenState : public State {
public:
    UiMainScreenState(std::shared_ptr<Button> button) : m_button(button) {}
    ~UiMainScreenState() override = default;

    bool enter() override;
    void exit() override;

private:
    lv_obj_t* m_main_screen = nullptr;
    std::shared_ptr<Button> m_button;
};