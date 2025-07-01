#pragma once

#include "../scheduler.h"
#include "io/bme_sensor.h"

// Reads data from the BME280 sensor periodically and publishes it to the event bus.
class BmeSensorReader : public Task {
public:
    BmeSensorReader();

    bool execute() override;

private:
    BME280Sensor m_sensor = {};
};
