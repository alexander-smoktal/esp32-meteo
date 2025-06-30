/**
 * @file ms_current_weather_gen.h
 *
 */

#ifndef MS_CURRENT_WEATHER_GEN_H
#define MS_CURRENT_WEATHER_GEN_H

#ifdef __cplusplus
extern "C" {
#endif

/*********************
 *      INCLUDES
 *********************/
#include "lvgl/lvgl.h"

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
 * Create a ms_current_weather object
 * @param parent pointer to an object, it will be the parent of the new ms_current_weather
 * @return pointer to the created ms_current_weather
 */
lv_obj_t * ms_current_weather_create(lv_obj_t * parent);


/**
 * Set the weather_image
 * @param obj   pointer to a ms_current_weather
 * @param weather_image  weather_image
 */
void ms_current_weather_set_weather_image(lv_obj_t * obj, void * weather_image);

/**********************
 *      MACROS
 **********************/

#ifdef __cplusplus
} /*extern "C"*/
#endif

#endif /*MS_CURRENT_WEATHER_GEN_H*/