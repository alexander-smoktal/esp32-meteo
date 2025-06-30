/**
 * @file ms_forecast.h
 *
 */

#ifndef MS_FORECAST_H
#define MS_FORECAST_H

#ifdef __cplusplus
extern "C" {
#endif

/*********************
 *      INCLUDES
 *********************/
#include "lvgl/lvgl.h"
#include "ms_forecast_gen.h"

/*********************
 *      DEFINES
 *********************/

/**********************
 *      TYPEDEFS
 **********************/

/**********************
 * GLOBAL PROTOTYPES
 **********************/

/**
 * Get the weather_icon
 * @param obj   pointer to a ms_forecast
 * @return      the weather_icon value
 */
void * ms_forecast_get_weather_icon(lv_obj_t * obj);

/**
 * Get the temp
 * @param obj   pointer to a ms_forecast
 * @return      the temp value
 */
const char * ms_forecast_get_temp(lv_obj_t * obj);

/**
 * Get the icon
 * @param obj   pointer to a ms_forecast
 * @return      the icon value
 */
const char * ms_forecast_get_icon(lv_obj_t * obj);

/**********************
 *      MACROS
 **********************/

#ifdef __cplusplus
} /*extern "C"*/
#endif

#endif /*MS_FORECAST_H*/