#include "sensor.h"

#include <esp_log.h>
#include <driver/i2c.h>
#include <esp_err.h>

#define SCL_IO_PIN (GPIO_NUM_0)
#define SDA_IO_PIN (GPIO_NUM_1)

#define I2C_PORT I2C_NUM_0

static const uint8_t DEVICE_ADDR = 0b1000000;
float POW_2_16 = 65536.0;

static const char *TAG = "sensor";

Sensor::Sensor()
{
    i2c_config_t i2c_conf = {};
    i2c_conf.mode = I2C_MODE_MASTER;
    i2c_conf.sda_io_num = SDA_IO_PIN;
    i2c_conf.scl_io_num = SCL_IO_PIN;
    i2c_conf.sda_pullup_en = GPIO_PULLUP_ENABLE;
    i2c_conf.scl_pullup_en = GPIO_PULLUP_ENABLE;
    i2c_conf.master.clk_speed = 400000;

    /* Initialize I2C */
    ESP_ERROR_CHECK(i2c_param_config(I2C_PORT, &i2c_conf));
    ESP_ERROR_CHECK(i2c_driver_install(I2C_PORT, i2c_conf.mode, 0, 0, 0));
}

std::pair<float, float> Sensor::get_values()
{
    ESP_LOGI(TAG, "Updating in house conditions");

    get_temp();
    return {get_temp(), get_humid()};
}

float Sensor::get_temp()
{
    ESP_LOGI(TAG, "Reading temperature");

    uint8_t tx_buffer[1] = {0b11100011};
    uint8_t rx_buffer[3] = {0};

    auto err = i2c_master_write_to_device(I2C_PORT, DEVICE_ADDR, tx_buffer, 1, pdMS_TO_TICKS(3000));
    if (err != ESP_OK)
    {
        reset_sensor();

        err = i2c_master_write_to_device(I2C_PORT, DEVICE_ADDR, tx_buffer, 1, pdMS_TO_TICKS(3000));
        if (err != ESP_OK)
        {
            ESP_LOGI(TAG, "Failed to write to sensor: %d", err);
            return 0;
        }
    }

    vTaskDelay(pdMS_TO_TICKS(20));
    err = i2c_master_read_from_device(I2C_PORT, DEVICE_ADDR, rx_buffer, 3, pdMS_TO_TICKS(3000));
    if (err != ESP_OK)
    {
        ESP_LOGI(TAG, "Failed to read from sensor: %d", err);
        return 0;
    }

    uint16_t temp_measure = rx_buffer[1] + ((uint16_t)rx_buffer[0] << 8);
    float real_temp = -46.75 + 175.72 * (float)temp_measure / POW_2_16;

    return real_temp;
}

float Sensor::get_humid()
{
    ESP_LOGI(TAG, "Reading humidity");
    uint8_t tx_buffer[1] = {0b11100101};
    uint8_t rx_buffer[3] = {0};

    auto err = i2c_master_write_to_device(I2C_PORT, DEVICE_ADDR, tx_buffer, 1, pdMS_TO_TICKS(3000));
    if (err != ESP_OK)
    {
        reset_sensor();

        err = i2c_master_write_to_device(I2C_PORT, DEVICE_ADDR, tx_buffer, 1, pdMS_TO_TICKS(3000));
        if (err != ESP_OK)
        {
            ESP_LOGI(TAG, "Failed to write to sensor: %d", err);
            return 0;
        }
    }

    vTaskDelay(pdMS_TO_TICKS(20));
    err = i2c_master_read_from_device(I2C_PORT, DEVICE_ADDR, rx_buffer, 3, pdMS_TO_TICKS(3000));
    if (err != ESP_OK)
    {
        ESP_LOGI(TAG, "Failed to read from sensor: %d", err);
        return 0;
    }

    uint16_t hum_measure = rx_buffer[1] + ((uint16_t)rx_buffer[0] << 8);
    float real_hum = -6.0 + 125.0 * (float)hum_measure / POW_2_16;

    return real_hum;
}

void Sensor::reset_sensor()
{
    ESP_LOGI(TAG, "Resetting sensor");

    uint8_t reset_buffer[1] = {0b11111110};
    i2c_master_write_to_device(I2C_PORT, DEVICE_ADDR, reset_buffer, 1, pdMS_TO_TICKS(3000));

    vTaskDelay(pdMS_TO_TICKS(500));
}