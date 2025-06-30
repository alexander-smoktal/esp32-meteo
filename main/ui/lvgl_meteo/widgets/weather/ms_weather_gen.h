/**
 * @file ms_weather_gen.h
 *
 */

#ifndef MS_WEATHER_GEN_H
#define MS_WEATHER_GEN_H

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
 * Create a ms_weather object
 * @param parent pointer to an object, it will be the parent of the new ms_weather
 * @return pointer to the created ms_weather
 */
lv_obj_t * ms_weather_create(lv_obj_t * parent);


/**
 * Set the inhouse
 * @param obj   pointer to a ms_weather
 * @param temp  temp
 * @param humid  humid
 * @param pressure  pressure
 */
void ms_weather_set_inhouse(lv_obj_t * obj, int32_t temp, int32_t humid, int32_t pressure);

/**
 * Set the outdoor
 * @param obj   pointer to a ms_weather
 * @param temp  temp
 * @param humid  humid
 */
void ms_weather_set_outdoor(lv_obj_t * obj, int32_t temp, int32_t humid);

/**
 * Set the icon
 * @param obj   pointer to a ms_weather
 * @param icon  icon
 */
void ms_weather_set_icon(lv_obj_t * obj, void * icon);

/**********************
 *      MACROS
 **********************/

#ifdef __cplusplus
} /*extern "C"*/
#endif

#endif /*MS_WEATHER_GEN_H*/