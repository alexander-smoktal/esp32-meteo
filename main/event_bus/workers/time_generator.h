#pragma once

#include "../scheduler.h"

// Ticks every second to generate the current time and publish it to the event bus.
class TimeGenerator : public Task {
public:
    TimeGenerator();

    bool execute() override;
};
