/**
 * @file ms_current_weather_private_gen.h
 *
 */

#ifndef MS_CURRENT_WEATHER_PRIVATE_H
#define MS_CURRENT_WEATHER_PRIVATE_H

#ifdef __cplusplus
extern "C" {
#endif

/*********************
 *      INCLUDES
 *********************/
#include "lvgl/src/core/lv_obj_private.h"
#include "ms_current_weather.h"

/*********************
 *      DEFINES
 *********************/

/**********************
 *      TYPEDEFS
 **********************/

typedef struct {
    lv_obj_t obj;  /* Base widget to extend */
    void * weather_image;
    lv_obj_t * weather_icon;
} ms_current_weather_t;

/**********************
 * GLOBAL PROTOTYPES
 **********************/

#if LV_USE_XML
    void ms_current_weather_register(void);
#endif

/**********************
 *      MACROS
 **********************/

#ifdef __cplusplus
} /*extern "C"*/
#endif

#endif /*MS_CURRENT_WEATHER_PRIVATE_H*/