/*
 * SPDX-FileCopyrightText: 2010-2022 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: CC0-1.0
 */
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include <esp_log.h>

#include "buttons.h"
#include "cpu_info.h"
#include "prediction.h"
#include "updater.h"
#include "flash.h"
#include "rgb.h"
#include "wifi.h"
#include "ui/ui.h"

extern "C" void app_main(void)
{
    CPUInfo::print();

    Flash flash{};
    UI ui{};
    Prediction prediction{&ui};
    Updater updater{&ui};
    WiFi wifi{&updater};
    Buttons buttons{&prediction, &ui};

    auto free_mem_size = heap_caps_get_free_size(MALLOC_CAP_8BIT);
    ESP_LOGI("mem", "Empty free mem: %d", free_mem_size);

    while (true)
    {
        vTaskDelay(pdMS_TO_TICKS(10000));
    }
}
