#pragma once

#include "esp_event.h"

#include "../scheduler.h"
#include "nv_storage.h"

// Starts NTP update server
class NtpUpdater : public Task {
public:
    NtpUpdater(std::shared_ptr<NVStorage> storage);

    bool execute() override;
};

