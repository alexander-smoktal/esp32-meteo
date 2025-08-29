/**
 * @file ms_current_weather.c
 *
 */

/*********************
 *      INCLUDES
 *********************/
#include "ms_current_weather_private_gen.h"
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

static const char *TAG = "Current Weather Widget";

static const char *icon_to_name(int icon)
{
    switch (icon)
    {
    case Sunny:
        return weather_sunny_icon;
    case MostlyCloudy:
        return weather_sunny_cloud_icon;
    case Cloudy:
        return weather_cloudy_icon;
    case ClearMoon:
        return weather_moon_icon;
    case CloudyMoon:
        return weather_cloudy_moon_icon;
    case Rain:
        return weather_rain_icon;
    case MostlyRain:
        return weather_rain_cloud_icon;
    case HeavyRain:
        return weather_heavy_rain_icon;
    case Snow:
        return weather_snow_icon;
    case Wind:
        return weather_windy_icon;
    case Storm:
        return weather_storm_icon;
    case Fog:
        return weather_fog_icon;
    case Sleet:
        return weather_sleet_icon;
    case Hot:
        return weather_hot_icon;
    case Cold:
        return weather_cold_icon;
    case Moon:
        return weather_moon_icon;
    }

    ESP_LOGW(TAG, "Unknown icon");
    return weather_sunny_icon;
}

static void event_callback(void *data, void *context)
{
    lv_obj_t *widget = (lv_obj_t *)(context);
    WeatherData *weather = (WeatherData *)(data);

    ms_current_weather_set_weather_image(widget, icon_to_name(weather->icon_type));
}
#endif

void ms_current_weather_constructor_hook(lv_obj_t *obj)
{
#if MS_DEVICE_BUILD
    event_bus_subscribe(CurrentWeatherEvent, event_callback, obj);
#endif
}

void ms_current_weather_destructor_hook(lv_obj_t *obj)
{
}

void ms_current_weather_event_hook(lv_event_t *e)
{
}

void ms_current_weather_set_weather_image(lv_obj_t *obj, void *weather_image)
{
    ms_current_weather_t *widget = (ms_current_weather_t *)obj;

    MAYBE_LOCK_LVGL({
        lv_image_set_src(widget->weather_icon, weather_image);
    })
}

/**********************
 *   STATIC FUNCTIONS
 **********************/