/**
 * @file ms_current_weather_gen.c
 *
 */

/*********************
 *      INCLUDES
 *********************/
#include "ms_current_weather_private_gen.h"
#include "lvgl/src/core/lv_obj_class_private.h"
#include "ui.h"

/*********************
 *      DEFINES
 *********************/

/**********************
 *      TYPEDEFS
 **********************/

/**********************
 *  GLOBAL PROTOTYPES
 **********************/

void ms_current_weather_constructor_hook(lv_obj_t * obj);
void ms_current_weather_destructor_hook(lv_obj_t * obj);
void ms_current_weather_event_hook(lv_event_t * e);

/**********************
 *  STATIC PROTOTYPES
 **********************/

static void ms_current_weather_constructor(const lv_obj_class_t * class_p, lv_obj_t * obj);
static void ms_current_weather_destructor(const lv_obj_class_t * class_p, lv_obj_t * obj);
static void ms_current_weather_event(const lv_obj_class_t * class_p, lv_event_t * e);

/**********************
 *  STATIC VARIABLES
 **********************/

const lv_obj_class_t ms_current_weather_class = {
    .base_class = &lv_obj_class,
    .constructor_cb = ms_current_weather_constructor,
    .destructor_cb = ms_current_weather_destructor,
    .event_cb = ms_current_weather_event,
    .instance_size = sizeof(ms_current_weather_t),
    .editable = 1,
    .name = "ms_current_weather"
};

/**********************
 *   GLOBAL FUNCTIONS
 **********************/


lv_obj_t * ms_current_weather_create(lv_obj_t * parent)
{
    LV_LOG_INFO("begin");
    lv_obj_t * obj = lv_obj_class_create_obj(&ms_current_weather_class, parent);
    lv_obj_class_init_obj(obj);
    return obj;
}

/**********************
 *   STATIC FUNCTIONS
 **********************/


static void ms_current_weather_constructor(const lv_obj_class_t * class_p, lv_obj_t * obj)
{
    LV_UNUSED(class_p);
    LV_TRACE_OBJ_CREATE("begin");

    ms_current_weather_t * widget = (ms_current_weather_t *)obj;
    
    static bool style_inited = false;

    if(!style_inited) {
        
        style_inited = true;
    }
    lv_obj_set_flag(obj, LV_OBJ_FLAG_SCROLLABLE, false);

    lv_obj_t * weather_icon = lv_image_create(obj);
    lv_obj_set_name(weather_icon, "weather_icon");
    lv_image_set_src(weather_icon, weather_rain_cloud_icon);
    widget->weather_icon = weather_icon;

    ms_current_weather_constructor_hook(obj);

    LV_TRACE_OBJ_CREATE("finished");
}

static void ms_current_weather_destructor(const lv_obj_class_t * class_p, lv_obj_t * obj)
{
    LV_UNUSED(class_p);

    ms_current_weather_destructor_hook(obj);
}

static void ms_current_weather_event(const lv_obj_class_t * class_p, lv_event_t * e)
{
    LV_UNUSED(class_p);

    lv_result_t res;

    /* Call the ancestor's event handler */
    res = lv_obj_event_base(&ms_current_weather_class, e);
    if(res != LV_RESULT_OK) return;

    ms_current_weather_event_hook(e);
}