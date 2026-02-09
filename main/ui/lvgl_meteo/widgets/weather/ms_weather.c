/**
 * @file ms_weather.c
 *
 */

/*********************
 *      INCLUDES
 *********************/
#include "ms_weather_private_gen.h"
#include "ui.h"

#if MS_DEVICE_BUILD
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

#if MS_DEVICE_BUILD
static const char *TAG = "Weather Widget";

static void event_callback(void *data, void *context)
{
    lv_obj_t *widget = (lv_obj_t *)(context);
    WeatherData *weather = (WeatherData *)(data);

    ms_weather_set_outdoor(widget, weather->temperature, weather->humidity);
}

static void bme_event_callback(void *data, void *context)
{
    lv_obj_t *widget = (lv_obj_t *)(context);
    BmeSensorData *weather = (BmeSensorData *)(data);

    ms_weather_set_inhouse(widget, weather->temperature,
                           weather->humidity,
                           weather->pressure);
}
#endif

void ms_weather_constructor_hook(lv_obj_t *obj)
{
#if MS_DEVICE_BUILD
    event_bus_subscribe(CurrentWeatherEvent, event_callback, obj);
    event_bus_subscribe(BmeSensorEvent, bme_event_callback, obj);
#endif
}

void ms_weather_destructor_hook(lv_obj_t *obj)
{
}

void ms_weather_event_hook(lv_event_t *e)
{
}

void ms_weather_set_inhouse(lv_obj_t *obj, int32_t temp, int32_t humid, int32_t pressure)
{
    ms_weather_t *widget = (ms_weather_t *)obj;

    MAYBE_LOCK_LVGL({
        lv_label_set_text_fmt(widget->inhouse_temp, "T: %ld°C", temp);
        lv_label_set_text_fmt(widget->inhouse_humidity, "H: %ld%%", humid);
        lv_label_set_text_fmt(widget->inhouse_pressure, "P:%ldm", pressure);
    })
}

void ms_weather_set_outdoor(lv_obj_t *obj, int32_t temp, int32_t humid)
{
    ms_weather_t *widget = (ms_weather_t *)obj;

    MAYBE_LOCK_LVGL({
        if (temp < -9) {
            lv_label_set_text_fmt(widget->outdoor_temp, "T: %ld°", temp);
        } else {
            lv_label_set_text_fmt(widget->outdoor_temp, "T: %ld°C", temp);
        }
        lv_label_set_text_fmt(widget->outdoor_humidity, "H: %ld%%", humid);
    })
}

/**********************
 *   STATIC FUNCTIONS
 **********************/