#pragma once

#include "esp_event.h"

#include "../scheduler.h"
#include "nv_storage.h"

class NtpUpdater : public Task {
public:
    NtpUpdater(std::shared_ptr<NVStorage> storage);

    bool execute() override;

private:
    static void ipEventHandler(void *arg, esp_event_base_t event_base, int32_t event_id, void *event_data);
};

