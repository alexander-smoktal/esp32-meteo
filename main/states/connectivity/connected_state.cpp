#include "connected_state.h"

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"

static constexpr auto TAG = "ConnectedState";

bool ConnectedState::enter() {
    ESP_LOGI(TAG, "Entering Connected state");
    m_wifi_manager->start_station();
    m_events->init_workers();

    return true;
}

void ConnectedState::exit() {
}