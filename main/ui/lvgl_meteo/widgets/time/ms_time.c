/**
 * @file ms_time.c
 *
 */

/*********************
 *      INCLUDES
 *********************/
#include <time.h>

#if MS_DEVICE_BUILD
#include "esp_lvgl_port.h"
#include "event_bus/event_bus_c.h"
#endif

#include "ms_time_private_gen.h"
#include "ui.h"
#include "macros.h"

/*********************
 *      DEFINES
 *********************/

/**********************
 *      TYPEDEFS
 **********************/

/**********************
 *  STATIC PROTOTYPES
 **********************/

/**********************
 *  STATIC VARIABLES
 **********************/

/**********************
 *      MACROS
 **********************/

/**********************
 *   GLOBAL FUNCTIONS
 **********************/

static void event_callback(void* data, void* context) {
    struct tm* timeinfo = (struct tm*)(data);
    lv_obj_t *widget = (lv_obj_t*)(context);

    ms_time_set_time(widget, timeinfo->tm_hour, timeinfo->tm_min, timeinfo->tm_sec);
}

void ms_time_constructor_hook(lv_obj_t * obj)
{
#if MS_DEVICE_BUILD
    event_bus_subscribe(TimeEvent, event_callback, obj);
#endif
}

void ms_time_destructor_hook(lv_obj_t * obj)
{

}

void ms_time_event_hook(lv_event_t * e)
{

}

void ms_time_set_time(lv_obj_t * obj, int32_t hours, int32_t minutes, int32_t seconds)
{
    ms_time_t * widget = (ms_time_t *)obj;

    MAYBE_LOCK_LVGL({
        lv_label_set_text_fmt(widget->time_label, "%02ld:%02ld:%02ld", hours, minutes, seconds);
    })
}


/**********************
 *   STATIC FUNCTIONS
 **********************/