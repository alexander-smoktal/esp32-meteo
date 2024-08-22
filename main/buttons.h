#pragma once

#include "freertos/FreeRTOS.h"
#include "freertos/queue.h"

#include "prediction.h"

class Buttons
{
public:
    Buttons(Prediction *prediction, UI *ui);

private:
    static void IRAM_ATTR gpio_isr_handler_0(void *ctx);
    static void IRAM_ATTR gpio_isr_handler_1(void *ctx);
    static void gpio_task(void *ctx);

    static void unlock_cb(void *ctx);

    QueueHandle_t m_gpio_evt_queue = nullptr;
    Prediction *m_prediction;
    UI *m_ui;

    esp_timer_handle_t m_lock_timer = nullptr;
    bool m_locked = false;
};