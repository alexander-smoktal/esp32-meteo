#pragma once

#include <memory>

#include "esp_netif.h"

#include "../state.h"
#include "nv_storage.h"
#include "wifi/wifi_manager.h"
#include "event_bus/events.h"

// Represents the connected state of the device.
// This state is entered after the connection test state if the connection is successful.
// Also initializes UI update workers because when provisioning worker can't do much.
class ConnectedState : public State {
public:
    ConnectedState(std::shared_ptr<NVStorage> storage, std::shared_ptr<WiFiManager> wifi_manager, std::shared_ptr<Events> events)
        : m_storage(std::move(storage))
        , m_wifi_manager(wifi_manager)
        , m_events(events) {}
    ~ConnectedState() override = default;

    bool enter() override;
    void exit() override;

private:
    std::shared_ptr<NVStorage> m_storage;
    std::shared_ptr<WiFiManager> m_wifi_manager;
    std::shared_ptr<Events> m_events;
};