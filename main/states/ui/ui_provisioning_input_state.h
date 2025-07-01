#pragma once

#include "../state.h"

// First state of provisioning starts WiFi access point and HTTP server.
// Deletes previous screen if it exists.
class UiProvisioningInputState : public State {
public:
    UiProvisioningInputState() = default;
    ~UiProvisioningInputState() override = default;

    bool enter() override;
    void exit() override;
};