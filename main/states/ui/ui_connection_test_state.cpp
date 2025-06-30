#include "ui_connection_test_state.h"

#include "esp_lvgl_port.h"

#include "ui/globals.h"

bool UiConnectionTestState::enter()
{
    assert(s_provisioning_screen != nullptr);

    auto container = lv_obj_get_child(s_provisioning_screen, 0);

    auto input_image = lv_obj_get_child(container, 0);
    auto conn_test_image = lv_obj_get_child(container, 1);
    auto completed_image = lv_obj_get_child(container, 2);

    if (lvgl_port_lock(-1))
    {
        lv_obj_set_style_opa(input_image, LV_OPA_40, 0);
        lv_obj_set_style_opa(conn_test_image, LV_OPA_100, 0);
        lv_obj_set_style_opa(completed_image, LV_OPA_40, 0);

        lvgl_port_unlock();
    }

    return true;
}

void UiConnectionTestState::exit()
{
}