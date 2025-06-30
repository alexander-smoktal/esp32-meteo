/**
 * @file ms_weather_gen.c
 *
 */

/*********************
 *      INCLUDES
 *********************/
#include "ms_weather_private_gen.h"
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

void ms_weather_constructor_hook(lv_obj_t * obj);
void ms_weather_destructor_hook(lv_obj_t * obj);
void ms_weather_event_hook(lv_event_t * e);

/**********************
 *  STATIC PROTOTYPES
 **********************/

static void ms_weather_constructor(const lv_obj_class_t * class_p, lv_obj_t * obj);
static void ms_weather_destructor(const lv_obj_class_t * class_p, lv_obj_t * obj);
static void ms_weather_event(const lv_obj_class_t * class_p, lv_event_t * e);

/**********************
 *  STATIC VARIABLES
 **********************/

const lv_obj_class_t ms_weather_class = {
    .base_class = &lv_obj_class,
    .constructor_cb = ms_weather_constructor,
    .destructor_cb = ms_weather_destructor,
    .event_cb = ms_weather_event,
    .instance_size = sizeof(ms_weather_t),
    .editable = 1,
    .name = "ms_weather"
};

/**********************
 *   GLOBAL FUNCTIONS
 **********************/


lv_obj_t * ms_weather_create(lv_obj_t * parent)
{
    LV_LOG_INFO("begin");
    lv_obj_t * obj = lv_obj_class_create_obj(&ms_weather_class, parent);
    lv_obj_class_init_obj(obj);
    return obj;
}

/**********************
 *   STATIC FUNCTIONS
 **********************/


static void ms_weather_constructor(const lv_obj_class_t * class_p, lv_obj_t * obj)
{
    LV_UNUSED(class_p);
    LV_TRACE_OBJ_CREATE("begin");

    ms_weather_t * widget = (ms_weather_t *)obj;
    static lv_style_t style_icon;
    static lv_style_t style_weather_icon;
    static lv_style_t style_container;
    static lv_style_t style_weather_label_style;
    static lv_style_t style_view_style;
    static bool style_inited = false;

    if(!style_inited) {
        
        lv_style_init(&style_icon);
        lv_style_set_margin_top(&style_icon, 8);

        lv_style_init(&style_weather_icon);
        lv_style_set_height(&style_weather_icon, 47);
        lv_style_set_width(&style_weather_icon, 50);

        lv_style_init(&style_container);
        lv_style_set_border_width(&style_container, 0);
        lv_style_set_bg_opa(&style_container, 0);
        lv_style_set_width(&style_container, 63);
        lv_style_set_height(&style_container, 54);
        lv_style_set_pad_all(&style_container, 0);
        lv_style_set_margin_top(&style_container, 1);

        lv_style_init(&style_weather_label_style);
        lv_style_set_text_font(&style_weather_label_style, weather_label_font);
        lv_style_set_text_color(&style_weather_label_style, lv_color_hex(0xdddddd));
        lv_style_set_width(&style_weather_label_style, lv_pct(100));
        lv_style_set_text_align(&style_weather_label_style, LV_TEXT_ALIGN_LEFT);
        lv_style_set_pad_ver(&style_weather_label_style, -1);

        lv_style_init(&style_view_style);
        lv_style_set_bg_opa(&style_view_style, 0);
        lv_style_set_border_width(&style_view_style, 0);
        lv_style_set_width(&style_view_style, 230);
        lv_style_set_height(&style_view_style, 54);
        lv_style_set_pad_all(&style_view_style, 0);
        lv_style_set_pad_column(&style_view_style, 3);

        style_inited = true;
    }
    lv_obj_add_style(obj, &style_view_style, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_flex_flow(obj, LV_FLEX_FLOW_ROW);
    lv_obj_set_flag(obj, LV_OBJ_FLAG_SCROLLABLE, false);

    lv_obj_t * lv_image_1 = lv_image_create(obj);
    lv_obj_add_style(lv_image_1, &style_icon, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_image_set_src(lv_image_1, house_image);
    widget->lv_image_1 = lv_image_1;
    lv_obj_t * lv_obj_1 = lv_obj_create(obj);
    lv_obj_add_style(lv_obj_1, &style_container, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_flex_flow(lv_obj_1, LV_FLEX_FLOW_COLUMN);
    lv_obj_set_style_pad_row(lv_obj_1, -3, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_flag(lv_obj_1, LV_OBJ_FLAG_SCROLLABLE, false);
    widget->lv_obj_1 = lv_obj_1;
    lv_obj_t * inhouse_temp = lv_label_create(lv_obj_1);
    lv_obj_add_style(inhouse_temp, &style_weather_label_style, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_name(inhouse_temp, "inhouse_temp");
    lv_label_set_text(inhouse_temp, "T: --°C");
    widget->inhouse_temp = inhouse_temp;
    lv_obj_t * inhouse_humidity = lv_label_create(lv_obj_1);
    lv_obj_add_style(inhouse_humidity, &style_weather_label_style, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_name(inhouse_humidity, "inhouse_humidity");
    lv_label_set_text(inhouse_humidity, "H: --%");
    widget->inhouse_humidity = inhouse_humidity;
    lv_obj_t * inhouse_pressure = lv_label_create(lv_obj_1);
    lv_obj_add_style(inhouse_pressure, &style_weather_label_style, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_name(inhouse_pressure, "inhouse_pressure");
    lv_label_set_text(inhouse_pressure, "P: ---m");
    widget->inhouse_pressure = inhouse_pressure;
    lv_obj_t * lv_image_2 = lv_image_create(obj);
    lv_obj_add_style(lv_image_2, &style_icon, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_image_set_src(lv_image_2, outdoor_image);
    widget->lv_image_2 = lv_image_2;
    lv_obj_t * lv_obj_2 = lv_obj_create(obj);
    lv_obj_add_style(lv_obj_2, &style_container, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_flex_flow(lv_obj_2, LV_FLEX_FLOW_COLUMN);
    lv_obj_set_style_pad_row(lv_obj_2, -3, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_flag(lv_obj_2, LV_OBJ_FLAG_SCROLLABLE, false);
    widget->lv_obj_2 = lv_obj_2;
    lv_obj_t * outdoor_temp = lv_label_create(lv_obj_2);
    lv_obj_add_style(outdoor_temp, &style_weather_label_style, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_name(outdoor_temp, "outdoor_temp");
    lv_label_set_text(outdoor_temp, "T: --°C");
    widget->outdoor_temp = outdoor_temp;
    lv_obj_t * outdoor_humidity = lv_label_create(lv_obj_2);
    lv_obj_add_style(outdoor_humidity, &style_weather_label_style, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_name(outdoor_humidity, "outdoor_humidity");
    lv_label_set_text(outdoor_humidity, "H: --%");
    widget->outdoor_humidity = outdoor_humidity;

    ms_weather_constructor_hook(obj);

    LV_TRACE_OBJ_CREATE("finished");
}

static void ms_weather_destructor(const lv_obj_class_t * class_p, lv_obj_t * obj)
{
    LV_UNUSED(class_p);

    ms_weather_destructor_hook(obj);
}

static void ms_weather_event(const lv_obj_class_t * class_p, lv_event_t * e)
{
    LV_UNUSED(class_p);

    lv_result_t res;

    /* Call the ancestor's event handler */
    res = lv_obj_event_base(&ms_weather_class, e);
    if(res != LV_RESULT_OK) return;

    ms_weather_event_hook(e);
}