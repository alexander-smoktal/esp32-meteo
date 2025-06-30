/**
 * @file ms_current_weather.h
 *
 */

#ifndef MS_CURRENT_WEATHER_H
#define MS_CURRENT_WEATHER_H

#ifdef __cplusplus
extern "C" {
#endif

/*********************
 *      INCLUDES
 *********************/
#include "lvgl/lvgl.h"
#include "ms_current_weather_gen.h"

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
 * Get the weather_image
 * @param obj   pointer to a ms_current_weather
 * @return      the weather_image value
 */
void * ms_current_weather_get_weather_image(lv_obj_t * obj);

/**********************
 *      MACROS
 **********************/

#ifdef __cplusplus
} /*extern "C"*/
#endif

#endif /*MS_CURRENT_WEATHER_H*/