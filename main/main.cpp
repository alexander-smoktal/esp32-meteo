/*
 * SPDX-FileCopyrightText: 2022 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Unlicense OR CC0-1.0
 */
/* HTTP File Server Example

   This example code is in the Public Domain (or CC0 licensed, at your option.)

   Unless required by applicable law or agreed to in writing, this
   software is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
   CONDITIONS OF ANY KIND, either express or implied.
*/

#include "esp_event.h"
#include "esp_log.h"
#include "esp_netif.h"
#include "esp_err.h"
#include "esp_wifi.h"

#include "common.h"
#include "nv_storage.h"
#include "states/state_manager.h"
#include "wifi/wifi_manager.h"
#include "io/bme_sensor.h"
#include "io/button.h"
#include "spi_flash.h"
#include "ui/display.h"
#include "event_bus/events.h"

static const char *TAG = "Meteostation main";

extern "C" void app_main(void)
{
    ESP_LOGI(TAG, "Starting example");

    ESP_ERROR_CHECK(esp_netif_init());
    ESP_ERROR_CHECK(esp_event_loop_create_default());

    auto data_partition = std::make_shared<SPIFlash>("/data", "data");
    auto images_partition = std::make_shared<SPIFlash>("/images", "images");
    auto http_static_partition = std::make_shared<SPIFlash>("/static", "static");

    auto storage = std::make_shared<NVStorage>();
    auto state_manager = std::make_shared<StateManager>();
    auto wifi_manager = std::make_shared<WiFiManager>(storage);

    auto events = std::make_shared<Events>(storage);
    auto display = std::make_shared<Display>();

    auto button = std::make_shared<Button>();
    // storage->erase();

    state_manager->registerStates(http_static_partition, data_partition, storage, wifi_manager, events, button);

    vTaskDelay(2000 / portTICK_PERIOD_MS);
    if (wifi_manager->has_credentials()) {
        state_manager->switch_to(StateId::MainScreen);
    } else {
        state_manager->switch_to(StateId::Provisioning);
    }

    ESP_LOGI(TAG, "Device has started");
    while (true) {
        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
}
