/**
 * @file ms_date_time_xml_parser.c
 *
 */

/*********************
 *      INCLUDES
 *********************/
#include "lvgl/lvgl.h"
#include "ms_date_time_gen.h"
#include "lvgl/src/others/xml/parsers/lv_xml_obj_parser.h"
#include "lvgl/src/others/xml/lv_xml_widget.h"
#include "lvgl/src/others/xml/lv_xml_parser.h"

/*********************
 *      DEFINES
 *********************/

/**********************
 *      TYPEDEFS
 **********************/

/**********************
 *  STATIC PROTOTYPES
 **********************/

/**********************
 *  STATIC VARIABLES
 **********************/

/**********************
 *      MACROS
 **********************/

/**********************
 *   GLOBAL FUNCTIONS
 **********************/

void * ms_date_time_xml_create(lv_xml_parser_state_t * state, const char ** attrs)
{
    LV_UNUSED(attrs);
    void * item = ms_date_time_create(lv_xml_state_get_parent(state));

    if(item == NULL) {
        LV_LOG_ERROR("Failed to create ms_date_time");
        return NULL;
    }

    return item;
}

void ms_date_time_xml_apply(lv_xml_parser_state_t * state, const char ** attrs)
{

    void * item = lv_xml_state_get_item(state);

    lv_xml_obj_apply(state, attrs);

    for(int i = 0; attrs[i]; i += 2) {
        const char * name = attrs[i];
        const char * value = attrs[i + 1];
    }
}

void ms_date_time_register(void)
{
    lv_xml_widget_register("ms_date_time", ms_date_time_xml_create, ms_date_time_xml_apply);
}

/**********************
 *   STATIC FUNCTIONS
 **********************/