#pragma once

#include "../scheduler.h"
#include "io/bme_sensor.h"

class BmeSensorReader : public Task {
public:
    BmeSensorReader();

    bool execute() override;

private:
    BME280Sensor m_sensor = {};
};
