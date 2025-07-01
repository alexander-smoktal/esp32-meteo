#pragma once

#include <memory>

#include "nv_storage.h"
#include "esp_netif.h"

#include "iwifi_connector.h"

/* A convenient wrapper for WiFI routines.
   Helps to:
   - Scan for available networks.
   - Test temporary credentials when provisioning.
   - Connect station.
   - Start access point.
   Is intended be able to switch between states seamlessly.
*/
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

    // The modes mainly help to prevent reconnecting to WiFi if already connected
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
