/**
 * @file ms_date.c
 *
 */

/*********************
 *      INCLUDES
 *********************/
#include <time.h>

#if MS_DEVICE_BUILD
#include "esp_lvgl_port.h"
#include "event_bus/event_bus_c.h"
#endif

#include "ms_date_private_gen.h"
#include "ui.h"
#include "macros.h"


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

static int32_t last_hour = -1;

static const char *week_days[] = {
    "Неділя",
    "Понеділок",
    "Вівторок",
    "Середа",
    "Четвер",
    "П'ятниця",
    "Субота",
    };

static void event_callback(void *data, void *context)
{
    struct tm *timeinfo = (struct tm *)(data);

    if (last_hour == timeinfo->tm_mday)
    {
        // No need to update the date if it hasn't changed
        return;
    }
    last_hour = timeinfo->tm_mday;

    lv_obj_t *widget = (lv_obj_t *)(context);

    ms_date_set_date(widget, week_days[timeinfo->tm_wday], timeinfo->tm_mday,
                     timeinfo->tm_mon + 1, timeinfo->tm_year + 1900);
}

void ms_date_constructor_hook(lv_obj_t *obj)
{
#if MS_DEVICE_BUILD
    event_bus_subscribe(TimeEvent, event_callback, obj);
#endif
}

void ms_date_destructor_hook(lv_obj_t *obj)
{
}

void ms_date_event_hook(lv_event_t *e)
{
}

void ms_date_set_date(lv_obj_t *obj, const char *week_day, int32_t date, int32_t month, int32_t year)
{
    ms_date_t *widget = (ms_date_t *)obj;

    MAYBE_LOCK_LVGL({
        lv_label_set_text(widget->day_of_week_label, week_day);
        lv_label_set_text_fmt(widget->date_label, "%02ld.%02ld.%04ld", date, month, year);
    })
}

/**********************
 *   STATIC FUNCTIONS
 **********************/