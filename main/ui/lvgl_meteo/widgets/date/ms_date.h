/**
 * @file ms_date.h
 *
 */

#ifndef MS_DATE_H
#define MS_DATE_H

#ifdef __cplusplus
extern "C" {
#endif

/*********************
 *      INCLUDES
 *********************/
#include "lvgl/lvgl.h"
#include "ms_date_gen.h"

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
 * Get the temp
 * @param obj   pointer to a ms_date
 * @return      the temp value
 */
void * ms_date_get_temp(lv_obj_t * obj);

/**
 * Get the icon
 * @param obj   pointer to a ms_date
 * @return      the icon value
 */
void * ms_date_get_icon(lv_obj_t * obj);

/**********************
 *      MACROS
 **********************/

#ifdef __cplusplus
} /*extern "C"*/
#endif

#endif /*MS_DATE_H*/