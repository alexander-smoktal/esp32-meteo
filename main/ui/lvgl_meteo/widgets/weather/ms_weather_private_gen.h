/**
 * @file ms_weather_private_gen.h
 *
 */

#ifndef MS_WEATHER_PRIVATE_H
#define MS_WEATHER_PRIVATE_H

#ifdef __cplusplus
extern "C" {
#endif

/*********************
 *      INCLUDES
 *********************/
#include "lvgl/src/core/lv_obj_private.h"
#include "ms_weather.h"

/*********************
 *      DEFINES
 *********************/

/**********************
 *      TYPEDEFS
 **********************/

typedef struct {
    lv_obj_t obj;  /* Base widget to extend */
    void * inhouse;
    void * outdoor;
    void * icon;
    lv_obj_t * lv_image_1;
    lv_obj_t * lv_obj_1;
    lv_obj_t * inhouse_temp;
    lv_obj_t * inhouse_humidity;
    lv_obj_t * inhouse_pressure;
    lv_obj_t * lv_image_2;
    lv_obj_t * lv_obj_2;
    lv_obj_t * outdoor_temp;
    lv_obj_t * outdoor_humidity;
} ms_weather_t;

/**********************
 * GLOBAL PROTOTYPES
 **********************/

#if LV_USE_XML
    void ms_weather_register(void);
#endif

/**********************
 *      MACROS
 **********************/

#ifdef __cplusplus
} /*extern "C"*/
#endif

#endif /*MS_WEATHER_PRIVATE_H*/