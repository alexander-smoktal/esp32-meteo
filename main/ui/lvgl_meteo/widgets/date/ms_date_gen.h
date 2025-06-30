/**
 * @file ms_date_gen.h
 *
 */

#ifndef MS_DATE_GEN_H
#define MS_DATE_GEN_H

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
 * Create a ms_date object
 * @param parent pointer to an object, it will be the parent of the new ms_date
 * @return pointer to the created ms_date
 */
lv_obj_t * ms_date_create(lv_obj_t * parent);


/**
 * Set the date
 * @param obj   pointer to a ms_date
 * @param week_day  week_day
 * @param date  date
 * @param month  month
 * @param year  year
 */
void ms_date_set_date(lv_obj_t * obj, const char * week_day, int32_t date, int32_t month, int32_t year);

/**********************
 *      MACROS
 **********************/

#ifdef __cplusplus
} /*extern "C"*/
#endif

#endif /*MS_DATE_GEN_H*/