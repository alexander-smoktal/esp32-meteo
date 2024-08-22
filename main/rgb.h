#pragma once

#include <cstdint>

class RGB
{
public:
    RGB();
    ~RGB();

    void blink();
    void set_colors(uint8_t value_r, uint8_t value_g, uint8_t value_b);
};
