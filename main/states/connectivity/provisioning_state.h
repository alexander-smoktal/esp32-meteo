#pragma once

#include <memory>

#include "esp_netif.h"

#include "../state.h"
#include "http_server/http_server.h"
#include "nv_storage.h"
#include "wifi/wifi_manager.h"
#include "spi_flash.h"

// WiFi provisioning state.
// This state starts an access point and HTTP server to collect WiFi credentials.
// It uses the HttpServer class to handle HTTP requests and serve a form for entering credentials.
// After the credentials are collected, it saves them to non-volatile storage and switches to the
// connection test state to verify the connection to the WiFi network.
class ProvisioningState : public State {
public:
    ProvisioningState(std::shared_ptr<SPIFlash> spi_flash,
                      std::shared_ptr<NVStorage> storage,
                      std::shared_ptr<WiFiManager> wifi_manager);
    ~ProvisioningState() override = default;

    bool enter() override;
    void exit() override;

private:
    void credentials_callback(const Credentials &credentials);

    std::shared_ptr<SPIFlash> m_spi_flash;
    std::shared_ptr<NVStorage> m_storage;
    std::shared_ptr<WiFiManager> m_wifi_manager;
    std::unique_ptr<HttpServer> m_server = nullptr;
};