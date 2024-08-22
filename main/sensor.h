#pragma once

#include <tuple>

class Sensor
{
public:
    Sensor();

    std::pair<float, float> get_values();

private:
    float get_temp();
    float get_humid();
    void reset_sensor();
};