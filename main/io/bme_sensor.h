#pragma once

#include "i2c_bus.h"
#include "bme280.h"
#include "esp_timer.h"

// A wrapper for the BME280 sensor.
// This class provides methods to read temperature, humidity, and pressure from the sensor.
// It uses the I2C bus to communicate with the sensor and handles initialization and cleanup.
class BME280Sensor {
public:
    BME280Sensor();
    ~BME280Sensor();

    float read_temperature();
    float read_humidity();
    float read_pressure();

private:
    i2c_bus_handle_t m_i2c_bus = nullptr;
    bme280_handle_t m_bme280 = nullptr;
};
