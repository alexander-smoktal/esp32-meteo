#pragma once

#include "../state.h"

class UiConnectedState : public State {
public:
    UiConnectedState() = default;
    ~UiConnectedState() override = default;

    bool enter() override;
    void exit() override;
};