#pragma once

#include "../state.h"

class UiConnectionTestState : public State {
public:
    UiConnectionTestState() = default;
    ~UiConnectionTestState() override = default;

    bool enter() override;
    void exit() override;
};