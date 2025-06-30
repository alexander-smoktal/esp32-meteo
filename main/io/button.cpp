#include "button.h"

#include "esp_log.h"
#include "button_gpio.h"
#include "driver/gpio.h"

static const char *TAG = "Button"; // Log tag for this module

static constexpr auto POWER_GPIO_NUM = GPIO_NUM_21; // GPIO pin for button power in
static constexpr auto BUTTON_GPIO_NUM = GPIO_NUM_38; // GPIO pin for button input, change as needed

Button::Button() {
    // Set up button power in
    gpio_config_t io_conf = {};
    io_conf.intr_type = GPIO_INTR_DISABLE;
    io_conf.mode = GPIO_MODE_OUTPUT;
    io_conf.pin_bit_mask = (1ULL << POWER_GPIO_NUM);
    io_conf.pull_down_en = GPIO_PULLDOWN_DISABLE;
    io_conf.pull_up_en = GPIO_PULLUP_ENABLE;
    ESP_ERROR_CHECK(gpio_config(&io_conf));

    ESP_ERROR_CHECK(gpio_set_level(POWER_GPIO_NUM, 1));

    // Initialize button configuration
    button_config_t button_config = {};
    button_config.long_press_time = 5000; // Default long press time in milliseconds
    button_config.short_press_time = 180; // Default short press time in milliseconds

    button_gpio_config_t gpio_config = {};
    gpio_config.gpio_num = BUTTON_GPIO_NUM; // Example GPIO pin, change as needed
    gpio_config.active_level = 1; // Active low
    gpio_config.enable_power_save = true; // Enable power saving mode
    gpio_config.disable_pull = false; // Enable internal pull-up resistor

    ESP_ERROR_CHECK(iot_button_new_gpio_device(&button_config, &gpio_config, &m_button_handle)); // Create new GPIO button device

    ESP_ERROR_CHECK(iot_button_register_cb(m_button_handle, BUTTON_PRESS_DOWN, nullptr, &Button::button_event_handler, this));
    ESP_ERROR_CHECK(iot_button_register_cb(m_button_handle, BUTTON_LONG_PRESS_START, nullptr, &Button::button_event_handler, this));

    ESP_LOGI(TAG, "Button initialized on GPIO %ld", gpio_config.gpio_num); // Log button initialization
}

void Button::register_callback(button_event_t event, std::function<void()> callback) {
    if (!callback) {
        m_callbacks.erase(event); // If callback is null, remove the event from the map
        return;
    }

    // Register a callback for a specific button event
    // Note: settings a callback disables the previous callback for the same event
    m_callbacks[event] = callback;
}

void Button::button_event_handler(void *arg, void *data) {
    auto *self = static_cast<Button *>(data);

    button_event_t event = iot_button_get_event(self->m_button_handle);
    ESP_LOGI(TAG, "%s", iot_button_get_event_str(event));
    if (BUTTON_PRESS_DOWN == event || BUTTON_LONG_PRESS_START == event) {
        if (self->m_callbacks.contains(event)) {
            self->m_callbacks[event](); // Call the registered callback for single click
        }
    }
}