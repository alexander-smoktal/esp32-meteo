/**
 * @file ms_date_time_private_gen.h
 *
 */

#ifndef MS_DATE_TIME_PRIVATE_H
#define MS_DATE_TIME_PRIVATE_H

#ifdef __cplusplus
extern "C" {
#endif

/*********************
 *      INCLUDES
 *********************/
#include "lvgl/src/core/lv_obj_private.h"
#include "ms_date_time.h"

/*********************
 *      DEFINES
 *********************/

/**********************
 *      TYPEDEFS
 **********************/

typedef struct {
    lv_obj_t obj;  /* Base widget to extend */
    lv_obj_t * ms_time_1;
    lv_obj_t * ms_date_1;
} ms_date_time_t;

/**********************
 * GLOBAL PROTOTYPES
 **********************/

#if LV_USE_XML
    void ms_date_time_register(void);
#endif

/**********************
 *      MACROS
 **********************/

#ifdef __cplusplus
} /*extern "C"*/
#endif

#endif /*MS_DATE_TIME_PRIVATE_H*/