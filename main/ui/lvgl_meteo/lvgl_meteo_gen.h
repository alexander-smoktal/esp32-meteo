/**
 * @file lvgl_meteo_gen.h
 */

#ifndef LVGL_METEO_GEN_H
#define LVGL_METEO_GEN_H

#ifndef UI_SUBJECT_STRING_LENGTH
#define UI_SUBJECT_STRING_LENGTH 256
#endif

#ifdef __cplusplus
extern "C" {
#endif

/*********************
 *      INCLUDES
 *********************/

#include "lvgl/lvgl.h"

/*Include all the widget and components of this library*/
#include "widgets/current_weather/ms_current_weather_gen.h"
#include "widgets/date/ms_date_gen.h"
#include "widgets/date_time/ms_date_time_gen.h"
#include "widgets/forecast/ms_forecast_gen.h"
#include "widgets/forecast_row/ms_forecast_row_gen.h"
#include "widgets/moon_phase/ms_moon_phase_gen.h"
#include "widgets/time/ms_time_gen.h"
#include "widgets/weather/ms_weather_gen.h"
#include "screens/main/main_screen_gen.h"
#include "screens/prediction/prediction_screen_gen.h"
#include "screens/provisioning/provisioning_screen_gen.h"
#include "screens/splash/splash_screen_gen.h"

/*********************
 *      DEFINES
 *********************/

// TODO: should these definitions be prefixed with the project name?


/**********************
 *      TYPEDEFS
 **********************/

/**********************
 * GLOBAL VARIABLES
 **********************/

/* Global Styles */

/* Fonts */
extern lv_font_t * forecast_label_font;
extern lv_font_t * weather_label_font;
extern lv_font_t * prediction_label_font;
extern lv_font_t * time_label_font;

/* Images */
extern const void * the_one_image;
extern const void * house_image;
extern const void * outdoor_image;
extern const void * weather_sunny_icon;
extern const void * weather_sunny_cloud_icon;
extern const void * weather_cloudy_icon;
extern const void * weather_cloudy_moon_icon;
extern const void * weather_cold_icon;
extern const void * weather_fog_icon;
extern const void * weather_heavy_rain_icon;
extern const void * weather_rain_icon;
extern const void * weather_rain_cloud_icon;
extern const void * weather_sleet_icon;
extern const void * weather_snow_icon;
extern const void * weather_storm_icon;
extern const void * weather_windy_icon;
extern const void * weather_hot_icon;
extern const void * weather_moon_icon;
extern const void * moon_first_quarter_icon;
extern const void * moon_last_quarter_icon;
extern const void * moon_full_icon;
extern const void * moon_new_icon;
extern const void * moon_waxing_icon;
extern const void * moon_laxing_icon;
extern const void * moon_gibbous_icon;
extern const void * moon_lagbous_icon;
extern const void * prov_wifi;
extern const void * prov_site;
extern const void * prov_complete;

/* Subjects */

/**********************
 * GLOBAL PROTOTYPES
 **********************/

/* callbacks */

/**
 * Initialize the component library
 */

void lvgl_meteo_init_gen(const char * asset_path);

/**********************
 *      MACROS
 **********************/

#ifdef __cplusplus
} /*extern "C"*/
#endif

#endif /*LVGL_METEO_GEN_H*/
