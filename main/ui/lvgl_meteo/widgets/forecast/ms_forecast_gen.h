/**
 * @file ms_forecast_gen.h
 *
 */

#ifndef MS_FORECAST_GEN_H
#define MS_FORECAST_GEN_H

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
 * Create a ms_forecast object
 * @param parent pointer to an object, it will be the parent of the new ms_forecast
 * @return pointer to the created ms_forecast
 */
lv_obj_t * ms_forecast_create(lv_obj_t * parent);


/**
 * Set the temp
 * @param obj   pointer to a ms_forecast
 * @param temp_low  temp_low
 * @param temp_high  temp_high
 */
void ms_forecast_set_temp(lv_obj_t * obj, int32_t temp_low, int32_t temp_high);

/**
 * Set the icon
 * @param obj   pointer to a ms_forecast
 * @param icon  icon
 */
void ms_forecast_set_icon(lv_obj_t * obj, void * icon);

/**********************
 *      MACROS
 **********************/

#ifdef __cplusplus
} /*extern "C"*/
#endif

#endif /*MS_FORECAST_GEN_H*/