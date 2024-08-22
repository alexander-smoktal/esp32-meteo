#include "rgb.h"

#include "sdkconfig.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include "esp_chip_info.h"
#include "esp_flash.h"
#include "esp_random.h"
#include "lightbulb.h"

const uint8_t WS2812_LED_NUM = 3;
const gpio_num_t WS2812_LED_GPIO = GPIO_NUM_8;

RGB::RGB()
{
    driver_ws2812_t ws2812 = {
        .led_num = WS2812_LED_NUM,
        .ctrl_io = WS2812_LED_GPIO,
    };

    lightbulb_capability_t capability;
    capability.led_beads = LED_BEADS_3CH_RGB;

    lightbulb_status_t init_status;
    init_status.mode = WORK_COLOR;
    init_status.on = false;
    init_status.hue = 0;
    init_status.saturation = 100;
    init_status.value = 100;

    lightbulb_config_t config;
    config.type = DRIVER_WS2812;
    config.driver_conf.ws2812 = ws2812;
    config.capability = capability;
    config.external_limit = NULL;
    config.gamma_conf = NULL;
    config.init_status = init_status;

    esp_err_t err = lightbulb_init(&config);
    if (err != ESP_OK)
    {
        printf("There may be some errors in the configuration, please check the log.");
        return;
    }
}

RGB::~RGB()
{
    ws2812_deinit();
}

void RGB::blink()
{
    /* Red */
    this->set_colors(10, 0, 0);
    vTaskDelay(pdMS_TO_TICKS(1000));

    this->set_colors(0, 10, 0);
    vTaskDelay(pdMS_TO_TICKS(1000));

    this->set_colors(0, 0, 10);
    vTaskDelay(pdMS_TO_TICKS(1000));
}

void RGB::set_colors(uint8_t value_r, uint8_t value_g, uint8_t value_b)
{
    ws2812_set_rgb_channel(value_r, value_g, value_b);
}