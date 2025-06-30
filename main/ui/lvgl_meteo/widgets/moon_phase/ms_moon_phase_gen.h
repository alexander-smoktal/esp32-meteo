/**
 * @file ms_moon_phase_gen.h
 *
 */

#ifndef MS_MOON_PHASE_GEN_H
#define MS_MOON_PHASE_GEN_H

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
 * Create a ms_moon_phase object
 * @param parent pointer to an object, it will be the parent of the new ms_moon_phase
 * @return pointer to the created ms_moon_phase
 */
lv_obj_t * ms_moon_phase_create(lv_obj_t * parent);


/**
 * Set the phase_image
 * @param obj   pointer to a ms_moon_phase
 * @param phase_image  phase_image
 */
void ms_moon_phase_set_phase_image(lv_obj_t * obj, void * phase_image);

/**********************
 *      MACROS
 **********************/

#ifdef __cplusplus
} /*extern "C"*/
#endif

#endif /*MS_MOON_PHASE_GEN_H*/