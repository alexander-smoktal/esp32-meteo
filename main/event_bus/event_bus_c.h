#pragma once

enum EventBusEvent {
    TimeEvent,
    CurrentWeatherEvent,
    WeatherForecastEvent,
    BmeSensorEvent,
    PredictionEvent,
};

#ifndef __cplusplus
typedef enum EventBusEvent EventBusEvent;
#endif

enum WeatherIconType
{
    Sunny,
    MostlyCloudy,
    Cloudy,
    ClearMoon,
    CloudyMoon,
    Rain,
    MostlyRain,
    HeavyRain,
    Snow,
    Wind,
    Storm,
    Fog,
    Sleet,
    Hot,
    Cold
};

#ifndef __cplusplus
typedef enum WeatherIconType WeatherIconType;
#endif

struct WeatherData {
    WeatherIconType icon_type;
    float temperature;
    float humidity;
};

#ifndef __cplusplus
typedef struct WeatherData WeatherData;
#endif

struct WeatherForecastEntry {
    WeatherIconType icon_type;
    float temp_low;
    float temp_high;
};

#ifndef __cplusplus
typedef struct WeatherForecastEntry WeatherForecastEntry;
#endif

typedef WeatherForecastEntry WeatherForecast[3];

struct BmeSensorData {
    float temperature;
    float humidity;
    float pressure;
};

#ifndef __cplusplus
typedef struct BmeSensorData BmeSensorData;
#endif

#ifdef __cplusplus
extern "C"
#endif
void event_bus_subscribe(EventBusEvent event, void (*callback)(void* data, void* context), void *context);

