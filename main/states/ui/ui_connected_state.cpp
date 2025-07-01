#include "ui_connected_state.h"

#include "esp_lvgl_port.h"

#include "ui/globals.h"

bool UiConnectedState::enter() {
    assert(s_provisioning_screen != nullptr);

    auto container = lv_obj_get_child(s_provisioning_screen, 0);

    auto input_image = lv_obj_get_child(container, 0);
    auto conn_test_image = lv_obj_get_child(container, 1);
    auto completed_image = lv_obj_get_child(container, 2);

    if (lvgl_port_lock(-1)) {
        lv_obj_set_style_opa(input_image, LV_OPA_40, 0);
        lv_obj_set_style_opa(conn_test_image, LV_OPA_40, 0);
        lv_obj_set_style_opa(completed_image, LV_OPA_100, 0);

        lvgl_port_unlock();
    }

    // The main screen will delete previous screen which is the provisioning screen now
    s_provisioning_screen = nullptr;
    switch_to(StateId::MainScreen);

    return true;
}

void UiConnectedState::exit() {
    s_provisioning_screen = nullptr;
}