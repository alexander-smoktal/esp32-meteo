/**
 * @file ms_date_private_gen.h
 *
 */

#ifndef MS_DATE_PRIVATE_H
#define MS_DATE_PRIVATE_H

#ifdef __cplusplus
extern "C" {
#endif

/*********************
 *      INCLUDES
 *********************/
#include "lvgl/src/core/lv_obj_private.h"
#include "ms_date.h"

/*********************
 *      DEFINES
 *********************/

/**********************
 *      TYPEDEFS
 **********************/

typedef struct {
    lv_obj_t obj;  /* Base widget to extend */
    void * date;
    lv_obj_t * day_of_week_label;
    lv_obj_t * date_label;
} ms_date_t;

/**********************
 * GLOBAL PROTOTYPES
 **********************/

#if LV_USE_XML
    void ms_date_register(void);
#endif

/**********************
 *      MACROS
 **********************/

#ifdef __cplusplus
} /*extern "C"*/
#endif

#endif /*MS_DATE_PRIVATE_H*/