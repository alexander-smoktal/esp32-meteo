/**
 * @file prediction_screen_gen.c
 * @description Template source file for LVGL objects
 */

/*********************
 *      INCLUDES
 *********************/
#include "prediction_screen_gen.h"
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

lv_obj_t * prediction_screen_create(void)
{
    LV_TRACE_OBJ_CREATE("begin");

    static lv_style_t style_view_style;
    static lv_style_t style_label_style;

    static bool style_inited = false;

    if(!style_inited) {
        
        lv_style_init(&style_view_style);
        lv_style_set_bg_color(&style_view_style, lv_color_hex(0x000000));
        lv_style_set_pad_all(&style_view_style, 0);
        lv_style_set_border_width(&style_view_style, 0);
        lv_style_set_radius(&style_view_style, 0);

        lv_style_init(&style_label_style);
        lv_style_set_text_font(&style_label_style, prediction_label_font);
        lv_style_set_text_color(&style_label_style, lv_color_hex(0xfdfdfd));
        lv_style_set_width(&style_label_style, lv_pct(95));
        lv_style_set_text_align(&style_label_style, LV_TEXT_ALIGN_CENTER);

        style_inited = true;
    }

    lv_obj_t * lv_obj_1 = lv_obj_create(NULL);
        lv_obj_add_style(lv_obj_1, &style_view_style, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_width(lv_obj_1, 320);
    lv_obj_set_height(lv_obj_1, 172);
    lv_obj_set_flag(lv_obj_1, LV_OBJ_FLAG_SCROLLABLE, false);

    lv_obj_t * lv_label_1 = lv_label_create(lv_obj_1);
    lv_obj_add_style(lv_label_1, &style_label_style, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_label_set_text(lv_label_1, "Тонюсік, увага! Це пророцтво містить вірус любові! Він знаходить самого добру і хорошу людину!");
    lv_obj_set_align(lv_label_1, LV_ALIGN_CENTER);
    lv_obj_set_flag(lv_label_1, LV_OBJ_FLAG_SCROLLABLE, false);


    LV_TRACE_OBJ_CREATE("finished");

    return lv_obj_1;
}

/**********************
 *   STATIC FUNCTIONS
 **********************/