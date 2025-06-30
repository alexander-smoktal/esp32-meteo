#include "provisioning_state.h"

#include "esp_wifi.h"
#include "esp_log.h"

#include "common.h"

static constexpr auto TAG = "ProvisioningState";

ProvisioningState::ProvisioningState(std::shared_ptr<SPIFlash> spi_flash,
                                     std::shared_ptr<NVStorage> storage,
                                     std::shared_ptr<WiFiManager> wifi_manager)
    : m_spi_flash(std::move(spi_flash))
    , m_storage(std::move(storage))
    , m_wifi_manager(std::move(wifi_manager)) {
}

bool ProvisioningState::enter() {
    ESP_LOGI(TAG, "Entering Provisioning state");
    auto enpoints = m_wifi_manager->get_endpoints();

    ESP_LOGI(TAG, "Found %zu endpoints", enpoints.size());

    m_server = std::make_unique<HttpServer>(m_spi_flash,
                                            m_wifi_manager,
                                            std::move(enpoints),
                                            [this](auto creds) { credentials_callback(creds); },
                                            m_storage);
    m_wifi_manager->start_access_point();

    return true;
}

void ProvisioningState::exit() {
    m_server.reset(); // Stop the server after setting credentials
}

void ProvisioningState::credentials_callback(const Credentials &credentials) {
    for (const auto &[key, value] : credentials) {
        // If it's WiFi creds, we create a temp keys to try to test connection
        if (key == "wifi_ssid") {
            ESP_LOGI(TAG, "Setting temporary WiFi SSID: %s", value.c_str());

            m_storage->set_string(s_temp_ssid_key, value);
        } else if (key == "wifi_password") {
            ESP_LOGI(TAG, "Setting temporary WiFi Password: %s", value.c_str());

            m_storage->set_string(s_temp_password_key, value);
        } else {
            ESP_LOGI(TAG, "Setting credential: %s = %s", key.c_str(), value.c_str());

            m_storage->set_string(key, value);
        }
    }

    ESP_LOGI(TAG, "Switching to Connection Test state");
    switch_to(StateId::ConnectionTest);
}