/**
 * @file ms_moon_phase.c
 *
 */

/*********************
 *      INCLUDES
 *********************/
#include "ms_moon_phase_private_gen.h"
#include "ui.h"

#if MS_DEVICE_BUILD
#include <time.h>

#include "esp_lvgl_port.h"
#include "esp_log.h"

#include "event_bus/event_bus_c.h"
#endif

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

static const uint64_t LAST_FULL_MOON = 1719004080;
static const uint64_t MOON_PERIOD = 2551443;

static const char *TAG = "Moon Phase Widget";

#if MS_DEVICE_BUILD
static const char *calculate_phase_image()
{
    ESP_LOGD(TAG, "Calculating moon phase");

    time_t now;
    time(&now);

    if (now < LAST_FULL_MOON)
    {
        ESP_LOGW(TAG, "Time is less than start");
        return NULL;
    }

    float diff = now - LAST_FULL_MOON;
    int32_t percentage = (int32_t)(diff / MOON_PERIOD * 100.0) % 100;
    ESP_LOGI(TAG, "Moon phase: %ld", percentage);

    if (percentage < 7 || percentage >= 94)
    {
        return moon_full_icon;
    }
    else if (percentage < 19)
    {
        return moon_lagbous_icon;
    }
    else if (percentage < 32)
    {
        return moon_last_quarter_icon;
    }
    else if (percentage < 44)
    {
        return moon_laxing_icon;
    }
    else if (percentage < 56)
    {
        return moon_new_icon;
    }
    else if (percentage < 69)
    {
        return moon_waxing_icon;
    }
    else if (percentage < 82)
    {
        return moon_first_quarter_icon;
    }
    else
    {
        return moon_gibbous_icon;
    }
}

static void event_callback(void *data, void *context)
{
    struct tm *timeinfo = (struct tm *)(data);

    if (last_hour == timeinfo->tm_hour)
    {
        // No need to update the phase if the dayas hasn't changed
        return;
    }
    last_hour = timeinfo->tm_hour;

    lv_obj_t *widget = (lv_obj_t *)(context);
    ms_moon_phase_set_phase_image(widget, calculate_phase_image());
}
#endif

void ms_moon_phase_constructor_hook(lv_obj_t * obj)
{
#if MS_DEVICE_BUILD
    // Reset phase image from the beginning to avoid flickering on start
    ms_moon_phase_set_phase_image(obj, calculate_phase_image());
    event_bus_subscribe(TimeEvent, event_callback, obj);
#endif
}

void ms_moon_phase_destructor_hook(lv_obj_t * obj)
{

}

void ms_moon_phase_event_hook(lv_event_t * e)
{

}

void ms_moon_phase_set_phase_image(lv_obj_t * obj, void * phase_image)
{
    ms_moon_phase_t * widget = (ms_moon_phase_t *)obj;

    MAYBE_LOCK_LVGL({
        lv_image_set_src(widget->moon_image, phase_image);
    })
}


/**********************
 *   STATIC FUNCTIONS
 **********************/