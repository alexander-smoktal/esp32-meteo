/**
 * @file ms_time_gen.h
 *
 */

#ifndef MS_TIME_GEN_H
#define MS_TIME_GEN_H

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
 * Create a ms_time object
 * @param parent pointer to an object, it will be the parent of the new ms_time
 * @return pointer to the created ms_time
 */
lv_obj_t * ms_time_create(lv_obj_t * parent);


/**
 * Set the time
 * @param obj   pointer to a ms_time
 * @param hours  hours
 * @param minutes  minutes
 * @param seconds  seconds
 */
void ms_time_set_time(lv_obj_t * obj, int32_t hours, int32_t minutes, int32_t seconds);

/**********************
 *      MACROS
 **********************/

#ifdef __cplusplus
} /*extern "C"*/
#endif

#endif /*MS_TIME_GEN_H*/