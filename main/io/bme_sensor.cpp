#include "bme_sensor.h"

#include "esp_log.h"

#include "driver/i2c_master.h"

static constexpr auto I2C_MASTER_SCL_IO = GPIO_NUM_48;   /*!< gpio number for I2C master clock */
static constexpr auto I2C_MASTER_SDA_IO = GPIO_NUM_47;    /*!< gpio number for I2C master data  */

static constexpr auto I2C_MASTER_FREQ_HZ = 100000;     /*!< I2C master clock frequency */

static constexpr auto I2C_MASTER_NUM  = I2C_NUM_0;   /*!< I2C port number for master dev */

BME280Sensor::BME280Sensor() {
    i2c_master_bus_config_t i2c_mst_config = {};
    i2c_mst_config.clk_source = I2C_CLK_SRC_DEFAULT;
    i2c_mst_config.i2c_port = I2C_MASTER_NUM;
    i2c_mst_config.scl_io_num = I2C_MASTER_SCL_IO;
    i2c_mst_config.sda_io_num = I2C_MASTER_SDA_IO;
    i2c_mst_config.glitch_ignore_cnt = 7;
    i2c_mst_config.flags.enable_internal_pullup = true;

    i2c_master_bus_handle_t bus_handle;
    ESP_ERROR_CHECK(i2c_new_master_bus(&i2c_mst_config, &bus_handle));

    // Step1: Init I2C bus
    i2c_config_t conf = {};
    conf.mode = I2C_MODE_MASTER;
    conf.sda_io_num = I2C_MASTER_SDA_IO;
    conf.sda_pullup_en = GPIO_PULLUP_ENABLE;
    conf.scl_io_num = I2C_MASTER_SCL_IO;
    conf.scl_pullup_en = GPIO_PULLUP_ENABLE;
    conf.master.clk_speed = I2C_MASTER_FREQ_HZ;

    // ESP_ERROR_CHECK(i2c_param_config(I2C_MASTER_NUM, &conf));
    m_i2c_bus = i2c_bus_create(I2C_MASTER_NUM, &conf);

    // Step2: Init bme280
    m_bme280 = bme280_create(m_i2c_bus, BME280_I2C_ADDRESS_DEFAULT);

    ESP_LOGI("BME sensor", "Reading: %d", bme280_is_reading_calibration(m_bme280));

    ESP_ERROR_CHECK(bme280_default_init(m_bme280));
}

BME280Sensor::~BME280Sensor() {
    // Cleanup resources
    if (m_bme280) {
        bme280_delete(&m_bme280);
    }

    if (m_i2c_bus) {
        i2c_bus_delete(&m_i2c_bus);
    }
}

float BME280Sensor::read_temperature() {
    float temperature = 0.0;
    bme280_read_temperature(m_bme280, &temperature);
    return temperature;
}

float BME280Sensor::read_humidity() {
    float humidity = 0.0;
    bme280_read_humidity(m_bme280, &humidity);
    return humidity;
}

float BME280Sensor::read_pressure() {
    float pressure = 0.0;
    bme280_read_pressure(m_bme280, &pressure);
    return pressure;
}
