#pragma once

#include "../state.h"

class UiProvisioningInputState : public State {
public:
    UiProvisioningInputState() = default;
    ~UiProvisioningInputState() override = default;

    bool enter() override;
    void exit() override;
};