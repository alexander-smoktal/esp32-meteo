/**
 * @file ms_weather.h
 *
 */

#ifndef MS_WEATHER_H
#define MS_WEATHER_H

#ifdef __cplusplus
extern "C" {
#endif

/*********************
 *      INCLUDES
 *********************/
#include "lvgl/lvgl.h"
#include "ms_weather_gen.h"

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
 * Get the inhouse
 * @param obj   pointer to a ms_weather
 * @return      the inhouse value
 */
void * ms_weather_get_inhouse(lv_obj_t * obj);

/**
 * Get the outdoor
 * @param obj   pointer to a ms_weather
 * @return      the outdoor value
 */
void * ms_weather_get_outdoor(lv_obj_t * obj);

/**********************
 *      MACROS
 **********************/

#ifdef __cplusplus
} /*extern "C"*/
#endif

#endif /*MS_WEATHER_H*/