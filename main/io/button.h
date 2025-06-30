#pragma once

#include <unordered_map>
#include <functional>

#include "iot_button.h"

class Button {
public:
    Button();

    void register_callback(button_event_t event, std::function<void()> callback);

private:
    static void button_event_handler(void *arg, void *data);

    std::unordered_map<button_event_t, std::function<void()>> m_callbacks;
    button_handle_t m_button_handle = nullptr;
};
