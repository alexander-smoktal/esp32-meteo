/**
 * @file ms_moon_phase.h
 *
 */

#ifndef MS_MOON_PHASE_H
#define MS_MOON_PHASE_H

#ifdef __cplusplus
extern "C" {
#endif

/*********************
 *      INCLUDES
 *********************/
#include "lvgl/lvgl.h"
#include "ms_moon_phase_gen.h"

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
 * Get the phase_image
 * @param obj   pointer to a ms_moon_phase
 * @return      the phase_image value
 */
void * ms_moon_phase_get_phase_image(lv_obj_t * obj);

/**********************
 *      MACROS
 **********************/

#ifdef __cplusplus
} /*extern "C"*/
#endif

#endif /*MS_MOON_PHASE_H*/