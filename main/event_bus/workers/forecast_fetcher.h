#pragma once

#include <tuple>

#include "esp_http_client.h"

#include "nvs_flash.h"

#include "../event_bus_c.h"
#include "../scheduler.h"

class ForecastFetcher : public Task {
public:
    ForecastFetcher(std::shared_ptr<NVStorage> storage): m_storage(std::move(storage)) {}

    bool execute() override;

private:
    static bool parse_forecast_json(const char *json, int len, WeatherForecast result);
    static esp_err_t http_event_handler(esp_http_client_event_t *evt);


    std::shared_ptr<NVStorage> m_storage;
};
