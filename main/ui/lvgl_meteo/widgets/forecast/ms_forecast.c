/**
 * @file ms_forecast.c
 *
 */

/*********************
 *      INCLUDES
 *********************/
#include "ms_forecast_private_gen.h"
#include "ui.h"

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

void ms_forecast_constructor_hook(lv_obj_t * obj)
{

}

void ms_forecast_destructor_hook(lv_obj_t * obj)
{

}

void ms_forecast_event_hook(lv_event_t * e)
{

}

void ms_forecast_set_temp(lv_obj_t * obj, int32_t temp_low, int32_t temp_high)
{
    ms_forecast_t * widget = (ms_forecast_t *)obj;

    lv_label_set_text_fmt(widget->temp_label, "%ld°/%ld°", temp_high, temp_low);
}


void ms_forecast_set_icon(lv_obj_t * obj, void * icon)
{
    ms_forecast_t * widget = (ms_forecast_t *)obj;

    widget->icon = icon;
    lv_image_set_src(widget->weather_icon, icon);
}

/**********************
 *   STATIC FUNCTIONS
 **********************/