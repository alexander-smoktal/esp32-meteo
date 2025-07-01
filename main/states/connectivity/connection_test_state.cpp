#include "connection_test_state.h"

#include "esp_log.h"

#include "common.h"

static constexpr auto TAG = "ConnectionTestState";

bool ConnectionTestState::enter() {
    ESP_LOGI(TAG, "Entering Connection Test state");

    const auto ssid = m_storage->get_string(NVS_TEMP_SSID_KEY);
    const auto password = m_storage->get_string(NVS_TEMP_PASSWORD_KEY);

    if (m_wifi_manager->test_connection(ssid, password)) {
        ESP_LOGI(TAG, "Connection test succeeded with SSID: %s", ssid.c_str());

        m_wifi_manager->set_credentials(ssid, password);

        m_events->init_workers(); // Initialize workers after successful connection
        vTaskDelay(2000 / portTICK_PERIOD_MS); // HTTP clients use a lot of resources, so we delay a bit

        switch_to(StateId::Connected);
    } else {
        ESP_LOGE(TAG, "Connection test failed with SSID: %s", ssid.c_str());
        // Optionally, you can switch back to Provisioning state or handle the failure
        switch_to(StateId::Provisioning);
    }

    return true; // Indicate that the state was entered successfully
}

void ConnectionTestState::exit() {
}