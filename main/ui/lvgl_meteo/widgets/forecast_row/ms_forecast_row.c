/**
 * @file ms_forecast_row.c
 *
 */

/*********************
 *      INCLUDES
 *********************/
 #include <string.h>

#include "ms_forecast_row_private_gen.h"
#include "../forecast/ms_forecast_gen.h"
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

static const char *TAG = "Forecast Widget";

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
    }

    ESP_LOGW(TAG, "Unknown icon");
    return weather_sunny_icon;
}

static void event_callback(void *data, void *context)
{
    lv_obj_t *widget = (lv_obj_t *)(context);

    WeatherForecast weather = {0};
    memcpy(weather, data, sizeof(weather));

    ms_forecast_row_set_forecast_1(widget,
        weather[0].temp_low,
        weather[0].temp_high,
        icon_to_name(weather[0].icon_type));

    ms_forecast_row_set_forecast_2(widget,
        weather[1].temp_low,
        weather[1].temp_high,
        icon_to_name(weather[1].icon_type));

    ms_forecast_row_set_forecast_3(widget,
        weather[2].temp_low,
        weather[2].temp_high,
        icon_to_name(weather[2].icon_type));
}
#endif

void ms_forecast_row_constructor_hook(lv_obj_t * obj)
{
#if MS_DEVICE_BUILD
    event_bus_subscribe(WeatherForecastEvent, event_callback, obj);
#endif
}

void ms_forecast_row_destructor_hook(lv_obj_t * obj)
{

}

void ms_forecast_row_event_hook(lv_event_t * e)
{

}

void ms_forecast_row_set_forecast_1(lv_obj_t * obj, int32_t temp_low, int32_t temp_high, void * icon)
{
    ms_forecast_row_t * widget = (ms_forecast_row_t *)obj;

    MAYBE_LOCK_LVGL({
        ms_forecast_set_temp(widget->forecast_1_widget, temp_low, temp_high);
        ms_forecast_set_icon(widget->forecast_1_widget, icon);
    })
}

void ms_forecast_row_set_forecast_2(lv_obj_t * obj, int32_t temp_low, int32_t temp_high, void * icon)
{
    ms_forecast_row_t * widget = (ms_forecast_row_t *)obj;

    MAYBE_LOCK_LVGL({
        ms_forecast_set_temp(widget->forecast_2_widget, temp_low, temp_high);
        ms_forecast_set_icon(widget->forecast_2_widget, icon);
    })
}

void ms_forecast_row_set_forecast_3(lv_obj_t * obj, int32_t temp_low, int32_t temp_high, void * icon)
{
    ms_forecast_row_t * widget = (ms_forecast_row_t *)obj;

    MAYBE_LOCK_LVGL({
        ms_forecast_set_temp(widget->forecast_3_widget, temp_low, temp_high);
        ms_forecast_set_icon(widget->forecast_3_widget, icon);
    })
}


/**********************
 *   STATIC FUNCTIONS
 **********************/