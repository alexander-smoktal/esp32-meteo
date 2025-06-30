/**
 * @file lvgl_meteo_gen.c
 */

/*********************
 *      INCLUDES
 *********************/
#include "lvgl_meteo_gen.h"

#if LV_USE_XML
   #include "widgets/current_weather/ms_current_weather_private_gen.h"
   #include "widgets/date/ms_date_private_gen.h"
   #include "widgets/date_time/ms_date_time_private_gen.h"
   #include "widgets/forecast/ms_forecast_private_gen.h"
   #include "widgets/forecast_row/ms_forecast_row_private_gen.h"
   #include "widgets/moon_phase/ms_moon_phase_private_gen.h"
   #include "widgets/time/ms_time_private_gen.h"
   #include "widgets/weather/ms_weather_private_gen.h"
#endif

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
 *  GLOBAL VARIABLES
 **********************/

/* Global Styles */

/* Fonts */
lv_font_t * forecast_label_font;
extern lv_font_t forecast_label_font_data;
lv_font_t * weather_label_font;
extern lv_font_t weather_label_font_data;
lv_font_t * prediction_label_font;
extern lv_font_t prediction_label_font_data;
lv_font_t * time_label_font;
extern lv_font_t time_label_font_data;

/* Images */
const void * the_one_image;
const void * house_image;
const void * outdoor_image;
const void * weather_sunny_icon;
const void * weather_sunny_cloud_icon;
const void * weather_cloudy_icon;
const void * weather_cloudy_moon_icon;
const void * weather_cold_icon;
const void * weather_fog_icon;
const void * weather_heavy_rain_icon;
const void * weather_rain_icon;
const void * weather_rain_cloud_icon;
const void * weather_sleet_icon;
const void * weather_snow_icon;
const void * weather_storm_icon;
const void * weather_windy_icon;
const void * weather_hot_icon;
const void * weather_moon_icon;
const void * moon_first_quarter_icon;
const void * moon_last_quarter_icon;
const void * moon_full_icon;
const void * moon_new_icon;
const void * moon_waxing_icon;
const void * moon_laxing_icon;
const void * moon_gibbous_icon;
const void * moon_lagbous_icon;
const void * prov_wifi;
const void * prov_site;
const void * prov_complete;

/*Subjects*/

/**********************
 *      MACROS
 **********************/

/**********************
 *   GLOBAL FUNCTIONS
 **********************/

void lvgl_meteo_init_gen(const char * asset_path)
{
    char buf[256];

    /* Global Styles */

    /* Subjects */

    /* Fonts */
    /* get font 'forecast_label_font' from a C array */
    forecast_label_font = &forecast_label_font_data;
    /* get font 'weather_label_font' from a C array */
    weather_label_font = &weather_label_font_data;
    /* get font 'prediction_label_font' from a C array */
    prediction_label_font = &prediction_label_font_data;
    /* get font 'time_label_font' from a C array */
    time_label_font = &time_label_font_data;

    /* Images */
    lv_snprintf(buf, 256, "%s%s", asset_path, "images/the_one_ring.png");
    the_one_image = lv_strdup(buf);
    lv_snprintf(buf, 256, "%s%s", asset_path, "images/icons8-house-48.png");
    house_image = lv_strdup(buf);
    lv_snprintf(buf, 256, "%s%s", asset_path, "images/icons8-nature-48.png");
    outdoor_image = lv_strdup(buf);
    lv_snprintf(buf, 256, "%s%s", asset_path, "images/weather/icons8-sunny-96.png");
    weather_sunny_icon = lv_strdup(buf);
    lv_snprintf(buf, 256, "%s%s", asset_path, "images/weather/icons8-cloudy-96.png");
    weather_sunny_cloud_icon = lv_strdup(buf);
    lv_snprintf(buf, 256, "%s%s", asset_path, "images/weather/icons8-cloudy-f-96.png");
    weather_cloudy_icon = lv_strdup(buf);
    lv_snprintf(buf, 256, "%s%s", asset_path, "images/weather/icons8-cloudy-moon-96.png");
    weather_cloudy_moon_icon = lv_strdup(buf);
    lv_snprintf(buf, 256, "%s%s", asset_path, "images/weather/icons8-cold-96.png");
    weather_cold_icon = lv_strdup(buf);
    lv_snprintf(buf, 256, "%s%s", asset_path, "images/weather/icons8-fog-96.png");
    weather_fog_icon = lv_strdup(buf);
    lv_snprintf(buf, 256, "%s%s", asset_path, "images/weather/icons8-heavy-rain-96.png");
    weather_heavy_rain_icon = lv_strdup(buf);
    lv_snprintf(buf, 256, "%s%s", asset_path, "images/weather/icons8-rain-96.png");
    weather_rain_icon = lv_strdup(buf);
    lv_snprintf(buf, 256, "%s%s", asset_path, "images/weather/icons8-rain-cloud-96.png");
    weather_rain_cloud_icon = lv_strdup(buf);
    lv_snprintf(buf, 256, "%s%s", asset_path, "images/weather/icons8-sleet-96.png");
    weather_sleet_icon = lv_strdup(buf);
    lv_snprintf(buf, 256, "%s%s", asset_path, "images/weather/icons8-snow-96.png");
    weather_snow_icon = lv_strdup(buf);
    lv_snprintf(buf, 256, "%s%s", asset_path, "images/weather/icons8-storm-with-heavy-rain-96.png");
    weather_storm_icon = lv_strdup(buf);
    lv_snprintf(buf, 256, "%s%s", asset_path, "images/weather/icons8-windy-weather-96.png");
    weather_windy_icon = lv_strdup(buf);
    lv_snprintf(buf, 256, "%s%s", asset_path, "images/weather/icons8-hot-96.png");
    weather_hot_icon = lv_strdup(buf);
    lv_snprintf(buf, 256, "%s%s", asset_path, "images/weather/icons8-full-moon-96.png");
    weather_moon_icon = lv_strdup(buf);
    lv_snprintf(buf, 256, "%s%s", asset_path, "images/moon_phase/icons8-first-quarter-96.png");
    moon_first_quarter_icon = lv_strdup(buf);
    lv_snprintf(buf, 256, "%s%s", asset_path, "images/moon_phase/icons8-first-quarter-96-flip.png");
    moon_last_quarter_icon = lv_strdup(buf);
    lv_snprintf(buf, 256, "%s%s", asset_path, "images/moon_phase/icons8-full-moon-96.png");
    moon_full_icon = lv_strdup(buf);
    lv_snprintf(buf, 256, "%s%s", asset_path, "images/moon_phase/icons8-new-moon-96.png");
    moon_new_icon = lv_strdup(buf);
    lv_snprintf(buf, 256, "%s%s", asset_path, "images/moon_phase/icons8-waxing-crescent-96.png");
    moon_waxing_icon = lv_strdup(buf);
    lv_snprintf(buf, 256, "%s%s", asset_path, "images/moon_phase/icons8-waxing-crescent-96-flip.png");
    moon_laxing_icon = lv_strdup(buf);
    lv_snprintf(buf, 256, "%s%s", asset_path, "images/moon_phase/icons8-waxing-gibbous-96.png");
    moon_gibbous_icon = lv_strdup(buf);
    lv_snprintf(buf, 256, "%s%s", asset_path, "images/moon_phase/icons8-waxing-gibbous-96-flip.png");
    moon_lagbous_icon = lv_strdup(buf);
    lv_snprintf(buf, 256, "%s%s", asset_path, "images/icons8-wifi-96.png");
    prov_wifi = lv_strdup(buf);
    lv_snprintf(buf, 256, "%s%s", asset_path, "images/icons8-website-96.png");
    prov_site = lv_strdup(buf);
    lv_snprintf(buf, 256, "%s%s", asset_path, "images/icons8-task-completed-96.png");
    prov_complete = lv_strdup(buf);

    #if LV_USE_XML
        ms_current_weather_register();
        ms_date_register();
        ms_date_time_register();
        ms_forecast_register();
        ms_forecast_row_register();
        ms_moon_phase_register();
        ms_time_register();
        ms_weather_register();

        lv_xml_register_font(NULL, "forecast_label_font", forecast_label_font);
        lv_xml_register_font(NULL, "weather_label_font", weather_label_font);
        lv_xml_register_font(NULL, "prediction_label_font", prediction_label_font);
        lv_xml_register_font(NULL, "time_label_font", time_label_font);

        lv_xml_register_image(NULL, "the_one_image", the_one_image);
        lv_xml_register_image(NULL, "house_image", house_image);
        lv_xml_register_image(NULL, "outdoor_image", outdoor_image);
        lv_xml_register_image(NULL, "weather_sunny_icon", weather_sunny_icon);
        lv_xml_register_image(NULL, "weather_sunny_cloud_icon", weather_sunny_cloud_icon);
        lv_xml_register_image(NULL, "weather_cloudy_icon", weather_cloudy_icon);
        lv_xml_register_image(NULL, "weather_cloudy_moon_icon", weather_cloudy_moon_icon);
        lv_xml_register_image(NULL, "weather_cold_icon", weather_cold_icon);
        lv_xml_register_image(NULL, "weather_fog_icon", weather_fog_icon);
        lv_xml_register_image(NULL, "weather_heavy_rain_icon", weather_heavy_rain_icon);
        lv_xml_register_image(NULL, "weather_rain_icon", weather_rain_icon);
        lv_xml_register_image(NULL, "weather_rain_cloud_icon", weather_rain_cloud_icon);
        lv_xml_register_image(NULL, "weather_sleet_icon", weather_sleet_icon);
        lv_xml_register_image(NULL, "weather_snow_icon", weather_snow_icon);
        lv_xml_register_image(NULL, "weather_storm_icon", weather_storm_icon);
        lv_xml_register_image(NULL, "weather_windy_icon", weather_windy_icon);
        lv_xml_register_image(NULL, "weather_hot_icon", weather_hot_icon);
        lv_xml_register_image(NULL, "weather_moon_icon", weather_moon_icon);
        lv_xml_register_image(NULL, "moon_first_quarter_icon", moon_first_quarter_icon);
        lv_xml_register_image(NULL, "moon_last_quarter_icon", moon_last_quarter_icon);
        lv_xml_register_image(NULL, "moon_full_icon", moon_full_icon);
        lv_xml_register_image(NULL, "moon_new_icon", moon_new_icon);
        lv_xml_register_image(NULL, "moon_waxing_icon", moon_waxing_icon);
        lv_xml_register_image(NULL, "moon_laxing_icon", moon_laxing_icon);
        lv_xml_register_image(NULL, "moon_gibbous_icon", moon_gibbous_icon);
        lv_xml_register_image(NULL, "moon_lagbous_icon", moon_lagbous_icon);
        lv_xml_register_image(NULL, "prov_wifi", prov_wifi);
        lv_xml_register_image(NULL, "prov_site", prov_site);
        lv_xml_register_image(NULL, "prov_complete", prov_complete);


    #endif
}

/* callbacks */

/**********************
 *   STATIC FUNCTIONS
 **********************/
