#pragma once

#include "../state.h"

// Second state of provisioning that tests WiFi connection
class UiConnectionTestState : public State {
public:
    UiConnectionTestState() = default;
    ~UiConnectionTestState() override = default;

    bool enter() override;
    void exit() override;
};