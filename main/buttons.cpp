#include "buttons.h"

#include <inttypes.h>

#include "freertos/task.h"
#include "driver/gpio.h"
#include "esp_intr_alloc.h"
#include "esp_log.h"

#define ESP_INTR_FLAG_DEFAULT 0

static const gpio_num_t BUTTON_GPIO_0 = GPIO_NUM_15;
static const gpio_num_t BUTTON_GPIO_1 = GPIO_NUM_14;

static const uint64_t ONE_SEC_IN_mSEC = 1000000;
static const uint64_t LOCK_TIME = ONE_SEC_IN_mSEC / 3;

static const char *TAG = "buttons";

void IRAM_ATTR Buttons::gpio_isr_handler_0(void *ctx)
{
    auto self = (Buttons *)ctx;

    gpio_num_t io_num = BUTTON_GPIO_0;
    xQueueSendFromISR(self->m_gpio_evt_queue, &io_num, NULL);
}

void IRAM_ATTR Buttons::gpio_isr_handler_1(void *ctx)
{
    auto self = (Buttons *)ctx;

    gpio_num_t io_num = BUTTON_GPIO_1;
    xQueueSendFromISR(self->m_gpio_evt_queue, &io_num, NULL);
}

Buttons::Buttons(Prediction *prediction, UI *ui)
    : m_gpio_evt_queue(xQueueCreate(3, sizeof(uint32_t))), m_prediction(prediction), m_ui(ui)
{
    gpio_config_t io_conf = {};
    io_conf.intr_type = GPIO_INTR_POSEDGE;
    io_conf.mode = GPIO_MODE_INPUT;
    io_conf.pull_down_en = GPIO_PULLDOWN_ENABLE;
    io_conf.pin_bit_mask = (1ULL << BUTTON_GPIO_0) | (1ULL << BUTTON_GPIO_1);
    ESP_ERROR_CHECK(gpio_config(&io_conf));

    // start gpio task
    xTaskCreate(gpio_task, "buttons task", 4096, this, 10, nullptr);

    ESP_ERROR_CHECK(gpio_install_isr_service(ESP_INTR_FLAG_DEFAULT));
    ESP_ERROR_CHECK(gpio_isr_handler_add(BUTTON_GPIO_0, gpio_isr_handler_0, this));
    ESP_ERROR_CHECK(gpio_isr_handler_add(BUTTON_GPIO_1, gpio_isr_handler_1, this));

    esp_timer_create_args_t timer_args = {};
    timer_args.callback = &unlock_cb;
    timer_args.arg = this;
    timer_args.name = "unlock timer";

    ESP_ERROR_CHECK(esp_timer_create(&timer_args, &m_lock_timer));
}

void Buttons::gpio_task(void *ctx)
{
    auto self = (Buttons *)ctx;

    gpio_num_t io_num;
    for (;;)
    {
        if (xQueueReceive(self->m_gpio_evt_queue, &io_num, portMAX_DELAY))
        {

            auto level = gpio_get_level(io_num);
            ESP_LOGI(TAG, "Button %d. Level: %d", io_num, level);

            if (!level || self->m_locked)
            {
                continue;
            }

            if (io_num == BUTTON_GPIO_0)
            {
                self->m_prediction->toggle();
            }
            else if (io_num == BUTTON_GPIO_1)
            {
                self->m_ui->toggle_light();
            }

            self->m_locked = true;
            ESP_ERROR_CHECK(esp_timer_start_once(self->m_lock_timer, LOCK_TIME));
        }
    }
}

void Buttons::unlock_cb(void *ctx)
{
    auto self = (Buttons *)ctx;
    self->m_locked = false;
}