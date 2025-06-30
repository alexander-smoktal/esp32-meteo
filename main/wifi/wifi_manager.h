#pragma once

#include <memory>

#include "nv_storage.h"
#include "esp_netif.h"

#include "iwifi_connector.h"

class WiFiManager: public IWiFiConnector
{
public:
    WiFiManager(std::shared_ptr<NVStorage> storage);
    ~WiFiManager();

    void start_station();
    void start_access_point();
    void set_credentials(const std::string &ssid, const std::string &password);
    bool has_credentials() const;

    // Connector interface
    bool test_connection(const std::string &ssid, const std::string &password) override;
    IWiFiConnector::EndpointsList get_endpoints() override;

private:
    void start_dhcp();
    void stop_dhcp();

    enum class CurrentMode {
        Station,
        AccessPoint,
        Init,
    };

    std::shared_ptr<NVStorage> m_storage;
    esp_netif_t *m_wifi_sta_interface = nullptr;
    esp_netif_t *m_wifi_ap_interface = nullptr;
    esp_event_handler_instance_t m_wifi_event_handler = nullptr;
    esp_event_handler_instance_t m_ip_event_handler = nullptr;
    CurrentMode m_current_mode = CurrentMode::Init;
};
