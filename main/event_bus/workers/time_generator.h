#pragma once

#include "../scheduler.h"

class TimeGenerator : public Task {
public:
    TimeGenerator() = default;

    bool execute() override;
};
