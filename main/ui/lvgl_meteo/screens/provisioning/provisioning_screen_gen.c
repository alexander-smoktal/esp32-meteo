/**
 * @file provisioning_screen_gen.c
 * @description Template source file for LVGL objects
 */

/*********************
 *      INCLUDES
 *********************/
#include "provisioning_screen_gen.h"
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

lv_obj_t * provisioning_screen_create(void)
{
    LV_TRACE_OBJ_CREATE("begin");

    static lv_style_t style_view_style;
    static lv_style_t style_container_style;
    static lv_style_t style_label_style;

    static bool style_inited = false;

    if(!style_inited) {
        
        lv_style_init(&style_view_style);
        lv_style_set_bg_color(&style_view_style, lv_color_hex(0x000));
        lv_style_set_pad_all(&style_view_style, 0);
        lv_style_set_border_side(&style_view_style, LV_BORDER_SIDE_NONE);
        lv_style_set_radius(&style_view_style, 0);

        lv_style_init(&style_container_style);
        lv_style_set_bg_opa(&style_container_style, 0);
        lv_style_set_border_side(&style_container_style, LV_BORDER_SIDE_NONE);
        lv_style_set_pad_column(&style_container_style, 0);

        lv_style_init(&style_label_style);
        lv_style_set_text_font(&style_label_style, weather_label_font);
        lv_style_set_text_color(&style_label_style, lv_color_hex(0xdddddd));
        lv_style_set_width(&style_label_style, lv_pct(100));
        lv_style_set_text_align(&style_label_style, LV_TEXT_ALIGN_LEFT);

        style_inited = true;
    }

    lv_obj_t * lv_obj_1 = lv_obj_create(NULL);
        lv_obj_add_style(lv_obj_1, &style_view_style, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_width(lv_obj_1, 320);
    lv_obj_set_height(lv_obj_1, 172);
    lv_obj_set_flag(lv_obj_1, LV_OBJ_FLAG_SCROLLABLE, false);

    lv_obj_t * lv_obj_2 = lv_obj_create(lv_obj_1);
    lv_obj_add_style(lv_obj_2, &style_container_style, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_flex_flow(lv_obj_2, LV_FLEX_FLOW_ROW);
    lv_obj_set_width(lv_obj_2, lv_pct(100));
    lv_obj_set_height(lv_obj_2, lv_pct(100));
    lv_obj_set_x(lv_obj_2, 5);
    lv_obj_set_y(lv_obj_2, 60);

    lv_obj_t * lv_image_1 = lv_image_create(lv_obj_2);
    lv_image_set_src(lv_image_1, prov_site);

    lv_obj_t * lv_image_2 = lv_image_create(lv_obj_2);
    lv_image_set_src(lv_image_2, prov_wifi);
    lv_obj_set_style_margin_left(lv_image_2, 5, LV_PART_MAIN | LV_STATE_DEFAULT);

    lv_obj_t * lv_image_3 = lv_image_create(lv_obj_2);
    lv_image_set_src(lv_image_3, prov_complete);

    lv_obj_t * lv_label_1 = lv_label_create(lv_obj_1);
    lv_obj_add_style(lv_label_1, &style_label_style, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_label_set_text(lv_label_1, "Що б налаштувати пристрій:\n  - Підключіться до 'MeteoProvisioning'\n  - Відкрийте 192.168.4.1\n  - Заповніть поля");
    lv_obj_set_x(lv_label_1, 10);
    lv_obj_set_y(lv_label_1, 3);


    LV_TRACE_OBJ_CREATE("finished");

    return lv_obj_1;
}

/**********************
 *   STATIC FUNCTIONS
 **********************/