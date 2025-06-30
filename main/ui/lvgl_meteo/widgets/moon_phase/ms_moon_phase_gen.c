/**
 * @file ms_moon_phase_gen.c
 *
 */

/*********************
 *      INCLUDES
 *********************/
#include "ms_moon_phase_private_gen.h"
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

void ms_moon_phase_constructor_hook(lv_obj_t * obj);
void ms_moon_phase_destructor_hook(lv_obj_t * obj);
void ms_moon_phase_event_hook(lv_event_t * e);

/**********************
 *  STATIC PROTOTYPES
 **********************/

static void ms_moon_phase_constructor(const lv_obj_class_t * class_p, lv_obj_t * obj);
static void ms_moon_phase_destructor(const lv_obj_class_t * class_p, lv_obj_t * obj);
static void ms_moon_phase_event(const lv_obj_class_t * class_p, lv_event_t * e);

/**********************
 *  STATIC VARIABLES
 **********************/

const lv_obj_class_t ms_moon_phase_class = {
    .base_class = &lv_obj_class,
    .constructor_cb = ms_moon_phase_constructor,
    .destructor_cb = ms_moon_phase_destructor,
    .event_cb = ms_moon_phase_event,
    .instance_size = sizeof(ms_moon_phase_t),
    .editable = 1,
    .name = "ms_moon_phase"
};

/**********************
 *   GLOBAL FUNCTIONS
 **********************/


lv_obj_t * ms_moon_phase_create(lv_obj_t * parent)
{
    LV_LOG_INFO("begin");
    lv_obj_t * obj = lv_obj_class_create_obj(&ms_moon_phase_class, parent);
    lv_obj_class_init_obj(obj);
    return obj;
}

/**********************
 *   STATIC FUNCTIONS
 **********************/


static void ms_moon_phase_constructor(const lv_obj_class_t * class_p, lv_obj_t * obj)
{
    LV_UNUSED(class_p);
    LV_TRACE_OBJ_CREATE("begin");

    ms_moon_phase_t * widget = (ms_moon_phase_t *)obj;
    
    static bool style_inited = false;

    if(!style_inited) {
        
        style_inited = true;
    }
    lv_obj_set_flag(obj, LV_OBJ_FLAG_SCROLLABLE, false);

    lv_obj_t * moon_image = lv_image_create(obj);
    lv_obj_set_name(moon_image, "moon_image");
    lv_image_set_src(moon_image, moon_laxing_icon);
    widget->moon_image = moon_image;

    ms_moon_phase_constructor_hook(obj);

    LV_TRACE_OBJ_CREATE("finished");
}

static void ms_moon_phase_destructor(const lv_obj_class_t * class_p, lv_obj_t * obj)
{
    LV_UNUSED(class_p);

    ms_moon_phase_destructor_hook(obj);
}

static void ms_moon_phase_event(const lv_obj_class_t * class_p, lv_event_t * e)
{
    LV_UNUSED(class_p);

    lv_result_t res;

    /* Call the ancestor's event handler */
    res = lv_obj_event_base(&ms_moon_phase_class, e);
    if(res != LV_RESULT_OK) return;

    ms_moon_phase_event_hook(e);
}