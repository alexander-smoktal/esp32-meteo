/**
 * @file ms_forecast_row_gen.h
 *
 */

#ifndef MS_FORECAST_ROW_GEN_H
#define MS_FORECAST_ROW_GEN_H

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
 * Create a ms_forecast_row object
 * @param parent pointer to an object, it will be the parent of the new ms_forecast_row
 * @return pointer to the created ms_forecast_row
 */
lv_obj_t * ms_forecast_row_create(lv_obj_t * parent);


/**
 * Set the forecast_1
 * @param obj   pointer to a ms_forecast_row
 * @param temp_low  temp_low
 * @param temp_high  temp_high
 * @param icon  icon
 */
void ms_forecast_row_set_forecast_1(lv_obj_t * obj, int32_t temp_low, int32_t temp_high, void * icon);

/**
 * Set the forecast_2
 * @param obj   pointer to a ms_forecast_row
 * @param temp_low  temp_low
 * @param temp_high  temp_high
 * @param icon  icon
 */
void ms_forecast_row_set_forecast_2(lv_obj_t * obj, int32_t temp_low, int32_t temp_high, void * icon);

/**
 * Set the forecast_3
 * @param obj   pointer to a ms_forecast_row
 * @param temp_low  temp_low
 * @param temp_high  temp_high
 * @param icon  icon
 */
void ms_forecast_row_set_forecast_3(lv_obj_t * obj, int32_t temp_low, int32_t temp_high, void * icon);

/**********************
 *      MACROS
 **********************/

#ifdef __cplusplus
} /*extern "C"*/
#endif

#endif /*MS_FORECAST_ROW_GEN_H*/