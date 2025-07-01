#pragma once

#include "../state.h"

// The last state of provisioning that shows the main screen
class UiConnectedState : public State {
public:
    UiConnectedState() = default;
    ~UiConnectedState() override = default;

    bool enter() override;
    void exit() override;
};