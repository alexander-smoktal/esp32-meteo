/**
 * @file main_screen_gen.c
 * @description Template source file for LVGL objects
 */

/*********************
 *      INCLUDES
 *********************/
#include "main_screen_gen.h"
#include "ui.h"

/*********************
 *      DEFINES
 *********************/

/**********************
 *      TYPEDEFS
 **********************/

/***********************
 *  STATIC VARIABLES
 **********************/

/***********************
 *  STATIC PROTOTYPES
 **********************/

/**********************
 *   GLOBAL FUNCTIONS
 **********************/

lv_obj_t * main_screen_create(void)
{
    LV_TRACE_OBJ_CREATE("begin");

    static lv_style_t style_view_style;
    static lv_style_t style_container;

    static bool style_inited = false;

    if(!style_inited) {
        
        lv_style_init(&style_view_style);
        lv_style_set_bg_color(&style_view_style, lv_color_hex(0x000));
        lv_style_set_pad_all(&style_view_style, 0);
        lv_style_set_border_width(&style_view_style, 0);
        lv_style_set_radius(&style_view_style, 0);

        lv_style_init(&style_container);
        lv_style_set_pad_all(&style_container, 0);
        lv_style_set_pad_row(&style_container, 0);
        lv_style_set_bg_opa(&style_container, 0);
        lv_style_set_border_width(&style_container, 0);
        lv_style_set_width(&style_container, lv_pct(100));
        lv_style_set_height(&style_container, lv_pct(100));

        style_inited = true;
    }

    lv_obj_t * lv_obj_1 = lv_obj_create(NULL);
        lv_obj_add_style(lv_obj_1, &style_view_style, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_width(lv_obj_1, 320);
    lv_obj_set_height(lv_obj_1, 172);
    lv_obj_set_flag(lv_obj_1, LV_OBJ_FLAG_SCROLLABLE, false);

    lv_obj_t * lv_obj_2 = lv_obj_create(lv_obj_1);
    lv_obj_add_style(lv_obj_2, &style_container, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_flex_flow(lv_obj_2, LV_FLEX_FLOW_COLUMN);
    lv_obj_set_flag(lv_obj_2, LV_OBJ_FLAG_SCROLLABLE, false);

    lv_obj_t * ms_date_time_1 = ms_date_time_create(lv_obj_2);
    lv_obj_set_style_margin_left(ms_date_time_1, -1, LV_PART_MAIN | LV_STATE_DEFAULT);

    lv_obj_t * ms_weather_1 = ms_weather_create(lv_obj_2);
    lv_obj_set_style_margin_left(ms_weather_1, 88, LV_PART_MAIN | LV_STATE_DEFAULT);

    lv_obj_t * ms_forecast_row_1 = ms_forecast_row_create(lv_obj_2);
    lv_obj_set_style_margin_left(ms_forecast_row_1, 88, LV_PART_MAIN | LV_STATE_DEFAULT);

    lv_obj_t * ms_current_weather_1 = ms_current_weather_create(lv_obj_1);
    lv_obj_set_x(ms_current_weather_1, 6);
    lv_obj_set_y(ms_current_weather_1, 75);

    lv_obj_t * ms_moon_phase_1 = ms_moon_phase_create(lv_obj_1);
    lv_obj_set_x(ms_moon_phase_1, 247);
    lv_obj_set_y(ms_moon_phase_1, 101);


    LV_TRACE_OBJ_CREATE("finished");

    return lv_obj_1;
}

/**********************
 *   STATIC FUNCTIONS
 **********************/