#include "ui_provisioning_input_state.h"

#include "esp_lvgl_port.h"

#include "ui/globals.h"

bool UiProvisioningInputState::enter()
{
    if (s_provisioning_screen == nullptr)
    {
        s_provisioning_screen = provisioning_screen_create();
    }

    auto container = lv_obj_get_child(s_provisioning_screen, 0);
    auto input_image = lv_obj_get_child(container, 0);
    auto conn_test_image = lv_obj_get_child(container, 1);
    auto completed_image = lv_obj_get_child(container, 2);

    lv_obj_set_style_opa(input_image, LV_OPA_100, 0);
    lv_obj_set_style_opa(conn_test_image, LV_OPA_40, 0);
    lv_obj_set_style_opa(completed_image, LV_OPA_40, 0);

    // This is a magic to delete previous screen without LVGL crashing
    lv_screen_load_anim(s_provisioning_screen, LV_SCR_LOAD_ANIM_NONE, 1, 1, true);

    return true;
}

void UiProvisioningInputState::exit()
{
}