#pragma once

#include <unordered_map>
#include <functional>

#include "iot_button.h"

// A hardware button interface.
class Button {
public:
    Button();

    // Register a callback for a specific button event.
    // If the callback is null, it removes the event from the map.
    // If the callback is not null, it sets the callback for the event.
    // Note: Setting a callback disables the previous callback for the same event.
    // Supported events: BUTTON_PRESS_DOWN, BUTTON_LONG_PRESS_START, etc.
    void register_callback(button_event_t event, std::function<void()> callback);

private:
    static void button_event_handler(void *arg, void *data);

    std::unordered_map<button_event_t, std::function<void()>> m_callbacks;
    button_handle_t m_button_handle = nullptr;
};
