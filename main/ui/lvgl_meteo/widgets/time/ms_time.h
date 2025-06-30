/**
 * @file ms_time.h
 *
 */

#ifndef MS_TIME_H
#define MS_TIME_H

#ifdef __cplusplus
extern "C" {
#endif

/*********************
 *      INCLUDES
 *********************/
#include "lvgl/lvgl.h"
#include "ms_time_gen.h"

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
 * Get the time
 * @param obj   pointer to a ms_time
 * @return      the time value
 */
void * ms_time_get_time(lv_obj_t * obj);

/**********************
 *      MACROS
 **********************/

#ifdef __cplusplus
} /*extern "C"*/
#endif

#endif /*MS_TIME_H*/