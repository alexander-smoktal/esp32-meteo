/**
 * @file ms_date_time_gen.h
 *
 */

#ifndef MS_DATE_TIME_GEN_H
#define MS_DATE_TIME_GEN_H

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
 * Create a ms_date_time object
 * @param parent pointer to an object, it will be the parent of the new ms_date_time
 * @return pointer to the created ms_date_time
 */
lv_obj_t * ms_date_time_create(lv_obj_t * parent);




/**********************
 *      MACROS
 **********************/

#ifdef __cplusplus
} /*extern "C"*/
#endif

#endif /*MS_DATE_TIME_GEN_H*/