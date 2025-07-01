#include "wifi_manager.h"

#include <cstring>
#include <cassert>
#include <sys/socket.h>

#include "esp_wifi.h"
#include "esp_log.h"

#include "common.h"
#include "freertos/event_groups.h"
#include <portmacro.h>

static constexpr auto TAG = "WiFi Manager";
static constexpr auto WIFI_CONNECTED_BIT = BIT0;
static constexpr auto WIFI_FAIL_BIT = BIT1;
static constexpr auto WIFI_SCAN_BIT = BIT2;

static constexpr auto s_wifi_ssid_key = "wifi_ssid";
static constexpr auto s_wifi_password_key = "wifi_password";

static const std::string s_wifi_ap_ssid = "MeteoProvisioning";

namespace
{
    EventGroupHandle_t s_wifi_event_group = nullptr;

    // When scanning for available networks
    static void scan_event_handler(void *arg, esp_event_base_t event_base, int32_t event_id, void *event_data) {
        if (event_base == WIFI_EVENT && event_id == WIFI_EVENT_STA_START) {
            xEventGroupSetBits(s_wifi_event_group, WIFI_SCAN_BIT);
        }

        ESP_LOGI(TAG, "Scan event handler called with base: %s, id: %ld", event_base, event_id);
    }

    // Blocks and waits to connect when testing temporary credentials when provisioning
    static void blocking_event_handler(void *arg, esp_event_base_t event_base,
                                       int32_t event_id, void *event_data) {
        if (event_base == WIFI_EVENT && event_id == WIFI_EVENT_STA_START) {
            esp_wifi_connect();
        } else if (event_base == WIFI_EVENT && event_id == WIFI_EVENT_STA_DISCONNECTED) {
            xEventGroupSetBits(s_wifi_event_group, WIFI_FAIL_BIT);
            ESP_LOGI(TAG, "Test WiFi connection failed");
        } else if (event_base == IP_EVENT && event_id == IP_EVENT_STA_GOT_IP) {
            ip_event_got_ip_t* event = (ip_event_got_ip_t*) event_data;
            ESP_LOGI(TAG, "Test WiFi connection succeeded with IP:" IPSTR, IP2STR(&event->ip_info.ip));
            xEventGroupSetBits(s_wifi_event_group, WIFI_CONNECTED_BIT);
        }
    }

    // Station handler that reconnects when disconnected
    static void async_event_handler(void *arg, esp_event_base_t event_base,
                                    int32_t event_id, void *event_data) {
        if (event_base == WIFI_EVENT && event_id == WIFI_EVENT_STA_START) {
            esp_wifi_connect();
        } else if (event_base == WIFI_EVENT && event_id == WIFI_EVENT_STA_DISCONNECTED) {
            vTaskDelay(3000 / portTICK_PERIOD_MS);
            esp_wifi_connect();
            ESP_LOGI(TAG, "WiFi connection lost. Trying to reconnect");
        } else if (event_base == IP_EVENT && event_id == IP_EVENT_STA_GOT_IP) {
            ip_event_got_ip_t* event = (ip_event_got_ip_t*) event_data;
            ESP_LOGI(TAG, "Connected to WiFi with IP: " IPSTR, IP2STR(&event->ip_info.ip));
            xEventGroupSetBits(s_wifi_event_group, WIFI_CONNECTED_BIT);
        }
    }
}

WiFiManager::WiFiManager(std::shared_ptr<NVStorage> storage) : m_storage(std::move(storage)) {
    s_wifi_event_group = xEventGroupCreate();

    const wifi_init_config_t wifi_init_config = WIFI_INIT_CONFIG_DEFAULT();
    ESP_ERROR_CHECK(esp_wifi_init(&wifi_init_config));

    ESP_ERROR_CHECK(esp_event_handler_instance_register(IP_EVENT,
                    IP_EVENT_STA_GOT_IP,
                    &async_event_handler,
                    nullptr,
                    &m_ip_event_handler));
}

WiFiManager::~WiFiManager() {
    esp_wifi_stop();

    if (m_wifi_sta_interface) {
        esp_netif_destroy_default_wifi(m_wifi_sta_interface);
    }

    if (m_wifi_ap_interface) {
        esp_netif_destroy_default_wifi(m_wifi_ap_interface);
    }
}

void WiFiManager::start_station() {
    if (m_current_mode == CurrentMode::Station) {
        ESP_LOGI(TAG, "WiFi Station mode already started");
        return;
    }

    m_current_mode = CurrentMode::Station;

    ESP_LOGI(TAG, "Starting WiFi Station");

    esp_wifi_stop();

    if (m_wifi_ap_interface) {
        stop_dhcp();
        esp_wifi_set_mode(WIFI_MODE_STA);

        esp_netif_destroy_default_wifi(m_wifi_ap_interface);
    }

    if (!m_wifi_sta_interface) {
        m_wifi_sta_interface = esp_netif_create_default_wifi_sta();
    }

    if (m_wifi_event_handler == nullptr) {
        // Handle WiFi events
        ESP_ERROR_CHECK(esp_event_handler_instance_register(WIFI_EVENT,
                        ESP_EVENT_ANY_ID,
                        &async_event_handler,
                        nullptr,
                        &m_wifi_event_handler));
    }

    auto ssid = m_storage->get_string(s_wifi_ssid_key);
    auto password = m_storage->get_string(s_wifi_password_key);
    if (ssid.empty() || password.empty()) {
        ESP_LOGE(TAG, "No WiFi credentials found");
        return;
    }

    ESP_LOGI(TAG, "Connecting to WiFi SSID: %s", ssid.c_str());
    wifi_sta_config_t sta_config = {};
    strncpy((char*)sta_config.ssid, ssid.c_str(), ssid.length());
    strncpy((char*)sta_config.password, password.c_str(), password.length());
    sta_config.threshold.authmode = WIFI_AUTH_WPA2_PSK;

    wifi_config_t wifi_config = {
        .sta = sta_config
    };

    ESP_ERROR_CHECK(esp_wifi_set_config(WIFI_IF_STA, &wifi_config));
    ESP_ERROR_CHECK(esp_wifi_start());
}

void WiFiManager::start_access_point() {
    if (m_current_mode == CurrentMode::AccessPoint) {
        ESP_LOGI(TAG, "WiFi Access Point mode already started");
        return;
    }

    m_current_mode = CurrentMode::AccessPoint;

    ESP_LOGI(TAG, "Starting WiFi Access Point with SSID: %s", s_wifi_ap_ssid.c_str());

    esp_wifi_disconnect();

    if (!m_wifi_ap_interface)
    {
        m_wifi_ap_interface = esp_netif_create_default_wifi_ap();
        esp_netif_set_default_netif(m_wifi_ap_interface);
    }

    if (!m_wifi_sta_interface) {
        m_wifi_sta_interface = esp_netif_create_default_wifi_sta();
    }

    wifi_config_t wifi_config = {};
    strncpy((char*)wifi_config.ap.ssid, s_wifi_ap_ssid.c_str(), s_wifi_ap_ssid.length());
    wifi_config.ap.ssid_len = s_wifi_ap_ssid.length();
    wifi_config.ap.max_connection = 4;
    wifi_config.ap.authmode = WIFI_AUTH_OPEN;

    ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_APSTA));
    ESP_ERROR_CHECK(esp_wifi_set_config(WIFI_IF_AP, &wifi_config));
    ESP_ERROR_CHECK(esp_wifi_start());

    start_dhcp();
}

void WiFiManager::set_credentials(const std::string &ssid, const std::string &password) {
    m_storage->set_string(s_wifi_ssid_key, ssid);
    m_storage->set_string(s_wifi_password_key, password);
}

bool WiFiManager::has_credentials() const {
    return m_storage->has_key(s_wifi_ssid_key) && m_storage->has_key(s_wifi_password_key);
}

// Connector interface
bool WiFiManager::test_connection(const std::string &ssid, const std::string &password) {
    ESP_LOGI(TAG, "Testing WiFi connection to SSID: %s", ssid.c_str());

    wifi_mode_t mode;
    ESP_ERROR_CHECK(esp_wifi_get_mode(&mode));
    assert(mode == WIFI_MODE_APSTA && "WiFi mode must be APSTA");

    if (m_wifi_event_handler != nullptr) {
        // Disable reconnection
        ESP_ERROR_CHECK(esp_event_handler_instance_unregister(WIFI_EVENT,
                                                              ESP_EVENT_ANY_ID,
                                                              m_wifi_event_handler));
        m_wifi_event_handler = nullptr;
    }

    // Handle WiFi events
    esp_event_handler_instance_t instance_any_id;
    ESP_ERROR_CHECK(esp_event_handler_instance_register(WIFI_EVENT,
                                                        ESP_EVENT_ANY_ID,
                                                        &blocking_event_handler,
                                                        nullptr,
                                                        &instance_any_id));

    esp_wifi_disconnect();
    // Wait for WiFi to disconnect
    xEventGroupWaitBits(s_wifi_event_group, WIFI_FAIL_BIT, pdTRUE, pdFALSE, 1000/portTICK_PERIOD_MS);

    wifi_sta_config_t sta_config = {};
    strncpy((char*)sta_config.ssid, ssid.c_str(), ssid.length());
    strncpy((char*)sta_config.password, password.c_str(), password.length());
    sta_config.threshold.authmode = WIFI_AUTH_WPA2_PSK;

    wifi_config_t wifi_config = {
        .sta = sta_config
    };

    ESP_ERROR_CHECK(esp_wifi_set_config(WIFI_IF_STA, &wifi_config));
    ESP_ERROR_CHECK(esp_wifi_connect());

    // Wait for connection
    xEventGroupWaitBits(s_wifi_event_group, WIFI_CONNECTED_BIT | WIFI_FAIL_BIT,
                        pdFALSE, pdFALSE, portMAX_DELAY);

    esp_event_handler_instance_unregister(WIFI_EVENT, ESP_EVENT_ANY_ID, instance_any_id);

    ESP_LOGI(TAG, "Connection test completed with: %lu", xEventGroupGetBits(s_wifi_event_group));

    bool success = (xEventGroupGetBits(s_wifi_event_group) & WIFI_CONNECTED_BIT) != 0;
    xEventGroupClearBits(s_wifi_event_group, WIFI_CONNECTED_BIT | WIFI_FAIL_BIT);

    return success;
}

IWiFiConnector::EndpointsList WiFiManager::get_endpoints() {
    ESP_LOGI(TAG, "Scanning for WiFi networks...");

    if (m_current_mode == CurrentMode::Station && m_wifi_event_handler != nullptr) {
        ESP_LOGI(TAG, "Device is provisioned. Disabling reconnect routine");

        esp_event_handler_instance_unregister(WIFI_EVENT,
                                              ESP_EVENT_ANY_ID,
                                              m_wifi_event_handler);
        m_wifi_event_handler = nullptr;
    }

    ESP_ERROR_CHECK(esp_wifi_stop());

    auto rssi_to_quality = [](int8_t rssi) -> int {
        if (rssi <= -100) return 0; // No signal
        if (rssi >= -50) return 100; // Excellent signal
        return (rssi + 100) * 2; // Scale to 0-100
    };

    esp_event_handler_instance_t instance_any_id;
    ESP_ERROR_CHECK(esp_event_handler_instance_register(WIFI_EVENT,
                                                        ESP_EVENT_ANY_ID,
                                                        &scan_event_handler,
                                                        nullptr,
                                                        &instance_any_id));

    if (!m_wifi_sta_interface) {
        m_wifi_sta_interface = esp_netif_create_default_wifi_sta();
    }

    ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_STA));
    ESP_ERROR_CHECK(esp_wifi_start());

    ESP_LOGI(TAG, "Waiting for scan to complete...");

    // Wait for connection WiFi to start
    xEventGroupWaitBits(s_wifi_event_group, WIFI_SCAN_BIT,
                        pdTRUE, pdFALSE, portMAX_DELAY);

    esp_event_handler_instance_unregister(WIFI_EVENT,
                                          ESP_EVENT_ANY_ID,
                                          instance_any_id);

    ESP_ERROR_CHECK(esp_wifi_scan_start(NULL, true));

    uint16_t ap_count = 0;
    ESP_ERROR_CHECK(esp_wifi_scan_get_ap_num(&ap_count));
    ESP_LOGI(TAG, "Total APs found = %u", ap_count);

    if (ap_count == 0) {
        ESP_LOGW(TAG, "No APs found");
        return {};
    }

    std::vector<wifi_ap_record_t> ap_info;
    ap_info.resize(ap_count, {});

    ESP_ERROR_CHECK(esp_wifi_scan_get_ap_records(&ap_count, ap_info.data()));

    IWiFiConnector::EndpointsList endpoints {};
    for (const auto &ap : ap_info) {
        endpoints.push_back(Endpoint{(const char*)ap.ssid, rssi_to_quality(ap.rssi)});
    }

    return endpoints;
}

void WiFiManager::start_dhcp() {
    assert(m_wifi_ap_interface != nullptr && "WiFi AP interface must be created before starting DHCP");

    esp_netif_ip_info_t ap_ip_info;
    memset(&ap_ip_info, 0x00, sizeof(ap_ip_info));
    inet_pton(AF_INET, "192.168.4.1", &ap_ip_info.ip);
    inet_pton(AF_INET, "192.168.4.1", &ap_ip_info.gw);
    inet_pton(AF_INET, "255.255.255.0", &ap_ip_info.netmask);
    ESP_ERROR_CHECK_WITHOUT_ABORT(esp_netif_dhcps_stop(m_wifi_ap_interface));
    ESP_ERROR_CHECK(esp_netif_set_ip_info(m_wifi_ap_interface, &ap_ip_info));
    ESP_ERROR_CHECK(esp_netif_dhcps_start(m_wifi_ap_interface));
}

void WiFiManager::stop_dhcp() {
    assert(m_wifi_ap_interface != nullptr && "WiFi AP interface must be created before stopping DHCP");

    ESP_ERROR_CHECK_WITHOUT_ABORT(esp_netif_dhcps_stop(m_wifi_ap_interface));
}