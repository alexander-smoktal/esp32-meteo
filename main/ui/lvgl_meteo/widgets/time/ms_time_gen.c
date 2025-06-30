/**
 * @file ms_time_gen.c
 *
 */

/*********************
 *      INCLUDES
 *********************/
#include "ms_time_private_gen.h"
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

void ms_time_constructor_hook(lv_obj_t * obj);
void ms_time_destructor_hook(lv_obj_t * obj);
void ms_time_event_hook(lv_event_t * e);

/**********************
 *  STATIC PROTOTYPES
 **********************/

static void ms_time_constructor(const lv_obj_class_t * class_p, lv_obj_t * obj);
static void ms_time_destructor(const lv_obj_class_t * class_p, lv_obj_t * obj);
static void ms_time_event(const lv_obj_class_t * class_p, lv_event_t * e);

/**********************
 *  STATIC VARIABLES
 **********************/

const lv_obj_class_t ms_time_class = {
    .base_class = &lv_obj_class,
    .constructor_cb = ms_time_constructor,
    .destructor_cb = ms_time_destructor,
    .event_cb = ms_time_event,
    .instance_size = sizeof(ms_time_t),
    .editable = 1,
    .name = "ms_time"
};

/**********************
 *   GLOBAL FUNCTIONS
 **********************/


lv_obj_t * ms_time_create(lv_obj_t * parent)
{
    LV_LOG_INFO("begin");
    lv_obj_t * obj = lv_obj_class_create_obj(&ms_time_class, parent);
    lv_obj_class_init_obj(obj);
    return obj;
}

/**********************
 *   STATIC FUNCTIONS
 **********************/


static void ms_time_constructor(const lv_obj_class_t * class_p, lv_obj_t * obj)
{
    LV_UNUSED(class_p);
    LV_TRACE_OBJ_CREATE("begin");

    ms_time_t * widget = (ms_time_t *)obj;
    static lv_style_t style_time_label_style;
    static lv_style_t style_view_style;
    static bool style_inited = false;

    if(!style_inited) {
        
        lv_style_init(&style_time_label_style);
        lv_style_set_text_font(&style_time_label_style, time_label_font);
        lv_style_set_text_color(&style_time_label_style, lv_color_hex(0xdddddd));
        lv_style_set_width(&style_time_label_style, lv_pct(100));
        lv_style_set_height(&style_time_label_style, 60);
        lv_style_set_text_align(&style_time_label_style, LV_TEXT_ALIGN_CENTER);
        lv_style_set_pad_top(&style_time_label_style, 8);

        lv_style_init(&style_view_style);
        lv_style_set_bg_opa(&style_view_style, 0);
        lv_style_set_border_width(&style_view_style, 0);
        lv_style_set_width(&style_view_style, 225);
        lv_style_set_height(&style_view_style, 60);
        lv_style_set_pad_all(&style_view_style, 0);

        style_inited = true;
    }
    lv_obj_add_style(obj, &style_view_style, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_flag(obj, LV_OBJ_FLAG_SCROLLABLE, false);

    lv_obj_t * time_label = lv_label_create(obj);
    lv_obj_add_style(time_label, &style_time_label_style, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_name(time_label, "time_label");
    lv_label_set_text(time_label, "00:00:00");
    widget->time_label = time_label;

    ms_time_constructor_hook(obj);

    LV_TRACE_OBJ_CREATE("finished");
}

static void ms_time_destructor(const lv_obj_class_t * class_p, lv_obj_t * obj)
{
    LV_UNUSED(class_p);

    ms_time_destructor_hook(obj);
}

static void ms_time_event(const lv_obj_class_t * class_p, lv_event_t * e)
{
    LV_UNUSED(class_p);

    lv_result_t res;

    /* Call the ancestor's event handler */
    res = lv_obj_event_base(&ms_time_class, e);
    if(res != LV_RESULT_OK) return;

    ms_time_event_hook(e);
}