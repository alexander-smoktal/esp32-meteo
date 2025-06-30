#pragma once

#if MS_DEVICE_BUILD
    #define MAYBE_LOCK_LVGL(body) \
        do { \
            if (lvgl_port_lock(-1)) { \
                    body \
                lvgl_port_unlock(); \
            } \
        } while (0);
#else
    #define MAYBE_LOCK_LVGL(body) \
        do { \
            body \
        } while (0);
#endif
