/**
 * @file ms_forecast_row_gen.c
 *
 */

/*********************
 *      INCLUDES
 *********************/
#include "ms_forecast_row_private_gen.h"
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

void ms_forecast_row_constructor_hook(lv_obj_t * obj);
void ms_forecast_row_destructor_hook(lv_obj_t * obj);
void ms_forecast_row_event_hook(lv_event_t * e);

/**********************
 *  STATIC PROTOTYPES
 **********************/

static void ms_forecast_row_constructor(const lv_obj_class_t * class_p, lv_obj_t * obj);
static void ms_forecast_row_destructor(const lv_obj_class_t * class_p, lv_obj_t * obj);
static void ms_forecast_row_event(const lv_obj_class_t * class_p, lv_event_t * e);

/**********************
 *  STATIC VARIABLES
 **********************/

const lv_obj_class_t ms_forecast_row_class = {
    .base_class = &lv_obj_class,
    .constructor_cb = ms_forecast_row_constructor,
    .destructor_cb = ms_forecast_row_destructor,
    .event_cb = ms_forecast_row_event,
    .instance_size = sizeof(ms_forecast_row_t),
    .editable = 1,
    .name = "ms_forecast_row"
};

/**********************
 *   GLOBAL FUNCTIONS
 **********************/


lv_obj_t * ms_forecast_row_create(lv_obj_t * parent)
{
    LV_LOG_INFO("begin");
    lv_obj_t * obj = lv_obj_class_create_obj(&ms_forecast_row_class, parent);
    lv_obj_class_init_obj(obj);
    return obj;
}

/**********************
 *   STATIC FUNCTIONS
 **********************/


static void ms_forecast_row_constructor(const lv_obj_class_t * class_p, lv_obj_t * obj)
{
    LV_UNUSED(class_p);
    LV_TRACE_OBJ_CREATE("begin");

    ms_forecast_row_t * widget = (ms_forecast_row_t *)obj;
    static lv_style_t style_view_style;
    static bool style_inited = false;

    if(!style_inited) {
        
        lv_style_init(&style_view_style);
        lv_style_set_bg_opa(&style_view_style, 0);
        lv_style_set_border_width(&style_view_style, 0);
        lv_style_set_width(&style_view_style, 150);
        lv_style_set_height(&style_view_style, 60);
        lv_style_set_pad_all(&style_view_style, 0);
        lv_style_set_pad_row(&style_view_style, 0);

        style_inited = true;
    }
    lv_obj_add_style(obj, &style_view_style, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_flex_flow(obj, LV_FLEX_FLOW_ROW);
    lv_obj_set_flag(obj, LV_OBJ_FLAG_SCROLLABLE, false);

    lv_obj_t * forecast_1_widget = ms_forecast_create(obj);
    lv_obj_set_name(forecast_1_widget, "forecast_1_widget");
    ms_forecast_set_temp(forecast_1_widget, 17, 7);
    ms_forecast_set_icon(forecast_1_widget, weather_sunny_cloud_icon);
    widget->forecast_1_widget = forecast_1_widget;
    lv_obj_t * forecast_2_widget = ms_forecast_create(obj);
    lv_obj_set_name(forecast_2_widget, "forecast_2_widget");
    ms_forecast_set_temp(forecast_2_widget, 0, 1);
    ms_forecast_set_icon(forecast_2_widget, weather_rain_cloud_icon);
    widget->forecast_2_widget = forecast_2_widget;
    lv_obj_t * forecast_3_widget = ms_forecast_create(obj);
    lv_obj_set_name(forecast_3_widget, "forecast_3_widget");
    ms_forecast_set_temp(forecast_3_widget, -3, -17);
    ms_forecast_set_icon(forecast_3_widget, weather_snow_icon);
    widget->forecast_3_widget = forecast_3_widget;

    ms_forecast_row_constructor_hook(obj);

    LV_TRACE_OBJ_CREATE("finished");
}

static void ms_forecast_row_destructor(const lv_obj_class_t * class_p, lv_obj_t * obj)
{
    LV_UNUSED(class_p);

    ms_forecast_row_destructor_hook(obj);
}

static void ms_forecast_row_event(const lv_obj_class_t * class_p, lv_event_t * e)
{
    LV_UNUSED(class_p);

    lv_result_t res;

    /* Call the ancestor's event handler */
    res = lv_obj_event_base(&ms_forecast_row_class, e);
    if(res != LV_RESULT_OK) return;

    ms_forecast_row_event_hook(e);
}