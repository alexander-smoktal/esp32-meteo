/**
 * @file ms_date_gen.c
 *
 */

/*********************
 *      INCLUDES
 *********************/
#include "ms_date_private_gen.h"
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

void ms_date_constructor_hook(lv_obj_t * obj);
void ms_date_destructor_hook(lv_obj_t * obj);
void ms_date_event_hook(lv_event_t * e);

/**********************
 *  STATIC PROTOTYPES
 **********************/

static void ms_date_constructor(const lv_obj_class_t * class_p, lv_obj_t * obj);
static void ms_date_destructor(const lv_obj_class_t * class_p, lv_obj_t * obj);
static void ms_date_event(const lv_obj_class_t * class_p, lv_event_t * e);

/**********************
 *  STATIC VARIABLES
 **********************/

const lv_obj_class_t ms_date_class = {
    .base_class = &lv_obj_class,
    .constructor_cb = ms_date_constructor,
    .destructor_cb = ms_date_destructor,
    .event_cb = ms_date_event,
    .instance_size = sizeof(ms_date_t),
    .editable = 1,
    .name = "ms_date"
};

/**********************
 *   GLOBAL FUNCTIONS
 **********************/


lv_obj_t * ms_date_create(lv_obj_t * parent)
{
    LV_LOG_INFO("begin");
    lv_obj_t * obj = lv_obj_class_create_obj(&ms_date_class, parent);
    lv_obj_class_init_obj(obj);
    return obj;
}

/**********************
 *   STATIC FUNCTIONS
 **********************/


static void ms_date_constructor(const lv_obj_class_t * class_p, lv_obj_t * obj)
{
    LV_UNUSED(class_p);
    LV_TRACE_OBJ_CREATE("begin");

    ms_date_t * widget = (ms_date_t *)obj;
    static lv_style_t style_date_label;
    static lv_style_t style_view_style;
    static bool style_inited = false;

    if(!style_inited) {
        
        lv_style_init(&style_date_label);
        lv_style_set_text_font(&style_date_label, weather_label_font);
        lv_style_set_text_color(&style_date_label, lv_color_hex(0xdddddd));
        lv_style_set_width(&style_date_label, lv_pct(100));
        lv_style_set_text_align(&style_date_label, LV_TEXT_ALIGN_CENTER);
        lv_style_set_pad_ver(&style_date_label, -1);

        lv_style_init(&style_view_style);
        lv_style_set_bg_opa(&style_view_style, 0);
        lv_style_set_border_width(&style_view_style, 0);
        lv_style_set_width(&style_view_style, 92);
        lv_style_set_height(&style_view_style, 60);
        lv_style_set_pad_all(&style_view_style, 0);
        lv_style_set_pad_row(&style_view_style, 0);

        style_inited = true;
    }
    lv_obj_add_style(obj, &style_view_style, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_flex_flow(obj, LV_FLEX_FLOW_COLUMN);
    lv_obj_set_flag(obj, LV_OBJ_FLAG_SCROLLABLE, false);

    lv_obj_t * day_of_week_label = lv_label_create(obj);
    lv_obj_add_style(day_of_week_label, &style_date_label, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_name(day_of_week_label, "day_of_week_label");
    lv_label_set_text(day_of_week_label, "Понеділок");
    lv_obj_set_style_margin_top(day_of_week_label, 8, LV_PART_MAIN | LV_STATE_DEFAULT);
    widget->day_of_week_label = day_of_week_label;
    lv_obj_t * date_label = lv_label_create(obj);
    lv_obj_add_style(date_label, &style_date_label, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_name(date_label, "date_label");
    lv_label_set_text(date_label, "30.12.2025");
    widget->date_label = date_label;

    ms_date_constructor_hook(obj);

    LV_TRACE_OBJ_CREATE("finished");
}

static void ms_date_destructor(const lv_obj_class_t * class_p, lv_obj_t * obj)
{
    LV_UNUSED(class_p);

    ms_date_destructor_hook(obj);
}

static void ms_date_event(const lv_obj_class_t * class_p, lv_event_t * e)
{
    LV_UNUSED(class_p);

    lv_result_t res;

    /* Call the ancestor's event handler */
    res = lv_obj_event_base(&ms_date_class, e);
    if(res != LV_RESULT_OK) return;

    ms_date_event_hook(e);
}