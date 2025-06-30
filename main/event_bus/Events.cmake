set(EVENTS_SOURCES
    "event_bus/cache.cpp"
    "event_bus/event_bus.cpp"
    "event_bus/scheduler.cpp"
    "event_bus/events.cpp"
    "event_bus/workers/ntp_updater.cpp"
    "event_bus/workers/time_generator.cpp"
    "event_bus/workers/weather_fetcher.cpp"
    "event_bus/workers/forecast_fetcher.cpp"
    "event_bus/workers/bme_sensor_reader.cpp"
    "event_bus/workers/prediction_reader.cpp"
    )

list(APPEND SOURCES ${EVENTS_SOURCES})