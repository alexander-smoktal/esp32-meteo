/**
 * @file ms_forecast_gen.c
 *
 */

/*********************
 *      INCLUDES
 *********************/
#include "ms_forecast_private_gen.h"
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

void ms_forecast_constructor_hook(lv_obj_t * obj);
void ms_forecast_destructor_hook(lv_obj_t * obj);
void ms_forecast_event_hook(lv_event_t * e);

/**********************
 *  STATIC PROTOTYPES
 **********************/

static void ms_forecast_constructor(const lv_obj_class_t * class_p, lv_obj_t * obj);
static void ms_forecast_destructor(const lv_obj_class_t * class_p, lv_obj_t * obj);
static void ms_forecast_event(const lv_obj_class_t * class_p, lv_event_t * e);

/**********************
 *  STATIC VARIABLES
 **********************/

const lv_obj_class_t ms_forecast_class = {
    .base_class = &lv_obj_class,
    .constructor_cb = ms_forecast_constructor,
    .destructor_cb = ms_forecast_destructor,
    .event_cb = ms_forecast_event,
    .instance_size = sizeof(ms_forecast_t),
    .editable = 1,
    .name = "ms_forecast"
};

/**********************
 *   GLOBAL FUNCTIONS
 **********************/


lv_obj_t * ms_forecast_create(lv_obj_t * parent)
{
    LV_LOG_INFO("begin");
    lv_obj_t * obj = lv_obj_class_create_obj(&ms_forecast_class, parent);
    lv_obj_class_init_obj(obj);
    return obj;
}

/**********************
 *   STATIC FUNCTIONS
 **********************/


static void ms_forecast_constructor(const lv_obj_class_t * class_p, lv_obj_t * obj)
{
    LV_UNUSED(class_p);
    LV_TRACE_OBJ_CREATE("begin");

    ms_forecast_t * widget = (ms_forecast_t *)obj;
    static lv_style_t style_temp_label;
    static lv_style_t style_icon_style;
    static lv_style_t style_view_style;
    static bool style_inited = false;

    if(!style_inited) {
        
        lv_style_init(&style_temp_label);
        lv_style_set_text_font(&style_temp_label, forecast_label_font);
        lv_style_set_text_color(&style_temp_label, lv_color_hex(0xdddddd));
        lv_style_set_width(&style_temp_label, lv_pct(100));
        lv_style_set_text_align(&style_temp_label, LV_TEXT_ALIGN_CENTER);
        lv_style_set_margin_top(&style_temp_label, -3);

        lv_style_init(&style_icon_style);
        lv_style_set_width(&style_icon_style, 50);
        lv_style_set_height(&style_icon_style, 50);

        lv_style_init(&style_view_style);
        lv_style_set_bg_opa(&style_view_style, 0);
        lv_style_set_border_width(&style_view_style, 0);
        lv_style_set_width(&style_view_style, 50);
        lv_style_set_height(&style_view_style, 60);
        lv_style_set_pad_all(&style_view_style, 0);
        lv_style_set_pad_row(&style_view_style, 0);

        style_inited = true;
    }
    lv_obj_add_style(obj, &style_view_style, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_flex_flow(obj, LV_FLEX_FLOW_COLUMN);
    lv_obj_set_flag(obj, LV_OBJ_FLAG_SCROLLABLE, false);

    lv_obj_t * weather_icon = lv_image_create(obj);
    lv_obj_add_style(weather_icon, &style_icon_style, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_name(weather_icon, "weather_icon");
    lv_image_set_src(weather_icon, weather_sleet_icon);
    lv_image_set_scale_x(weather_icon, 140);
    lv_image_set_scale_y(weather_icon, 140);
    widget->weather_icon = weather_icon;
    lv_obj_t * temp_label = lv_label_create(obj);
    lv_obj_add_style(temp_label, &style_temp_label, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_name(temp_label, "temp_label");
    lv_label_set_text(temp_label, "-20°/-30°");
    widget->temp_label = temp_label;

    ms_forecast_constructor_hook(obj);

    LV_TRACE_OBJ_CREATE("finished");
}

static void ms_forecast_destructor(const lv_obj_class_t * class_p, lv_obj_t * obj)
{
    LV_UNUSED(class_p);

    ms_forecast_destructor_hook(obj);
}

static void ms_forecast_event(const lv_obj_class_t * class_p, lv_event_t * e)
{
    LV_UNUSED(class_p);

    lv_result_t res;

    /* Call the ancestor's event handler */
    res = lv_obj_event_base(&ms_forecast_class, e);
    if(res != LV_RESULT_OK) return;

    ms_forecast_event_hook(e);
}