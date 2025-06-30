/**
 * @file ms_forecast_row_private_gen.h
 *
 */

#ifndef MS_FORECAST_ROW_PRIVATE_H
#define MS_FORECAST_ROW_PRIVATE_H

#ifdef __cplusplus
extern "C" {
#endif

/*********************
 *      INCLUDES
 *********************/
#include "lvgl/src/core/lv_obj_private.h"
#include "ms_forecast_row.h"

/*********************
 *      DEFINES
 *********************/

/**********************
 *      TYPEDEFS
 **********************/

typedef struct {
    lv_obj_t obj;  /* Base widget to extend */
    void * forecast_1;
    void * forecast_2;
    void * forecast_3;
    lv_obj_t * forecast_1_widget;
    lv_obj_t * forecast_2_widget;
    lv_obj_t * forecast_3_widget;
} ms_forecast_row_t;

/**********************
 * GLOBAL PROTOTYPES
 **********************/

#if LV_USE_XML
    void ms_forecast_row_register(void);
#endif

/**********************
 *      MACROS
 **********************/

#ifdef __cplusplus
} /*extern "C"*/
#endif

#endif /*MS_FORECAST_ROW_PRIVATE_H*/