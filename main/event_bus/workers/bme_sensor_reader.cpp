#include "bme_sensor_reader.h"

#include "esp_log.h"

#include "../event_bus.h"

static constexpr auto TAG = "BME Sensor Reader";

BmeSensorReader::BmeSensorReader() {
    execute();
}

bool BmeSensorReader::execute() {
    BmeSensorData sensor_data = {};
    sensor_data.temperature = m_sensor.read_temperature();
    sensor_data.humidity = m_sensor.read_humidity();
    sensor_data.pressure = m_sensor.read_pressure() * 0.75006375541921; // Convert to mmHg

    // ESP_LOGI(TAG, "Temperature: %.4f °C, Humidity: %.4f %%, Pressure: %.4f mm",
    //     sensor_data.temperature, sensor_data.humidity, sensor_data.pressure);

    EventBus::get_instance().pubish(EventBusEvent::BmeSensorEvent, &sensor_data, sizeof(sensor_data), false);

    return true;
}
