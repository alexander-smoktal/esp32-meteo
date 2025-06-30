/**
 * @file splash_screen_gen.c
 * @description Template source file for LVGL objects
 */

/*********************
 *      INCLUDES
 *********************/
#include "splash_screen_gen.h"
#include "ui.h"

/*********************
 *      DEFINES
 *********************/

/**********************
 *      TYPEDEFS
 **********************/

/***********************
 *  STATIC VARIABLES
 **********************/

/***********************
 *  STATIC PROTOTYPES
 **********************/

/**********************
 *   GLOBAL FUNCTIONS
 **********************/

lv_obj_t * splash_screen_create(void)
{
    LV_TRACE_OBJ_CREATE("begin");

    static lv_style_t style_view_style;

    static bool style_inited = false;

    if(!style_inited) {
        
        lv_style_init(&style_view_style);
        lv_style_set_bg_color(&style_view_style, lv_color_hex(0x000));
        lv_style_set_pad_all(&style_view_style, 0);
        lv_style_set_border_width(&style_view_style, 0);
        lv_style_set_radius(&style_view_style, 0);

        style_inited = true;
    }

    lv_obj_t * lv_obj_1 = lv_obj_create(NULL);
        lv_obj_add_style(lv_obj_1, &style_view_style, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_width(lv_obj_1, 320);
    lv_obj_set_height(lv_obj_1, 172);

    lv_obj_t * lv_image_1 = lv_image_create(lv_obj_1);
    lv_image_set_src(lv_image_1, the_one_image);
    lv_obj_set_width(lv_image_1, 310);
    lv_obj_set_height(lv_image_1, lv_pct(100));
    lv_obj_set_x(lv_image_1, 10);


    LV_TRACE_OBJ_CREATE("finished");

    return lv_obj_1;
}

/**********************
 *   STATIC FUNCTIONS
 **********************/