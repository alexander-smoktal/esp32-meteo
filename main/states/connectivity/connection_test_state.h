#pragma once

#include <memory>

#include "esp_netif.h"

#include "../state.h"
#include "nv_storage.h"
#include "wifi/wifi_manager.h"
#include "event_bus/events.h"

// Connection test state.
// This state tests the connection to the WiFi network using the provided credentials.
// It is used after the provisioning state to ensure that the device can connect to the network.
// If the connection is successful, it transitions to the connected state.
class ConnectionTestState : public State {
public:
    ConnectionTestState(std::shared_ptr<NVStorage> storage,
                        std::shared_ptr<WiFiManager> wifi_manager,
                        std::shared_ptr<Events> events)
        : m_storage(std::move(storage))
        , m_wifi_manager(wifi_manager)
        , m_events(events) {}
    ~ConnectionTestState() override = default;

    bool enter() override;
    void exit() override;

private:
    std::shared_ptr<NVStorage> m_storage;
    std::shared_ptr<WiFiManager> m_wifi_manager;
    std::shared_ptr<Events> m_events;
};