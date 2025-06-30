/**
 * @file ms_forecast_private_gen.h
 *
 */

#ifndef MS_FORECAST_PRIVATE_H
#define MS_FORECAST_PRIVATE_H

#ifdef __cplusplus
extern "C" {
#endif

/*********************
 *      INCLUDES
 *********************/
#include "lvgl/src/core/lv_obj_private.h"
#include "ms_forecast.h"

/*********************
 *      DEFINES
 *********************/

/**********************
 *      TYPEDEFS
 **********************/

typedef struct {
    lv_obj_t obj;  /* Base widget to extend */
    void * temp;
    void * icon;
    lv_obj_t * weather_icon;
    lv_obj_t * temp_label;
} ms_forecast_t;

/**********************
 * GLOBAL PROTOTYPES
 **********************/

#if LV_USE_XML
    void ms_forecast_register(void);
#endif

/**********************
 *      MACROS
 **********************/

#ifdef __cplusplus
} /*extern "C"*/
#endif

#endif /*MS_FORECAST_PRIVATE_H*/