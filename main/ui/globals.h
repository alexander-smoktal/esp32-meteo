#pragma once

#include "lvgl_meteo/screens/provisioning/provisioning_screen_gen.h"

// Because we have three provisioning states that use the same screen,
// we need a static single screen
extern lv_obj_t *s_provisioning_screen;
