#include "event_bus.h"

#include <cassert>
#include <string>

#include "esp_log.h"

static constexpr auto TAG = "EventBus";

static std::unique_ptr<EventBus> s_instance = nullptr;

ESP_EVENT_DEFINE_BASE(EventBusBase);

namespace {
    const char* bus_event_to_string(EventBusEvent event) {
        switch (event) {
            case TimeEvent: return "TimeEvent";
            case CurrentWeatherEvent: return "CurrentWeatherEvent";
            case WeatherForecastEvent: return "WeatherForecastEvent";
            default: return "UnknownEvent";
        }
    }
}

const std::string EVENT_DATA_CACHE_PREFIX = "ev_";

extern "C" void event_bus_subscribe(EventBusEvent event, void (*callback)(void* data, void* context), void *context) {
    EventBus::get_instance().subscribe(event, [callback, context](void* data) {
        callback(data, context);
    });
}

EventBus& EventBus::init_instance(std::shared_ptr<NVStorage> data_storage) {
    assert(s_instance == nullptr && "EventBus instance is already initialized");

    s_instance.reset(new EventBus(data_storage));

    return *s_instance;
}

EventBus& EventBus::get_instance() {
    return *s_instance;
}

EventBus::EventBus(std::shared_ptr<NVStorage> data_storage)
    : m_cache(std::make_unique<Cache>(data_storage)) {
    esp_event_handler_register(EventBusBase, ESP_EVENT_ANY_ID, &EventBus::event_handler, nullptr);
}

void EventBus::publish(EventBusEvent event, const void* data, size_t size, bool save_to_cache) {
    assert(s_instance != nullptr && "EventBus instance is not initialized");

    // If save_to_cache is true, save the data to cache
    if (save_to_cache) {
        auto cache_key = EVENT_DATA_CACHE_PREFIX + std::to_string(event);
        ESP_LOGI(TAG, "Saving event '%s' data to cache", bus_event_to_string(event));

        s_instance->m_cache->write_data(cache_key, data, size);
    }

    // Publish the event to the ESP event loop
    esp_event_post(EventBusBase, (int32_t)event, data, size, portMAX_DELAY);
}

void EventBus::subscribe(EventBusEvent event, EventSubscription callback) {
    assert(s_instance != nullptr && "EventBus s_instance is not initialized");

    s_instance->m_subscriptions.emplace(event, std::move(callback));

    // Read event cached data if it exists
    auto cache_key = EVENT_DATA_CACHE_PREFIX + std::to_string(event);
    auto cache_data = s_instance->m_cache->read_data(cache_key);
    if (cache_data.has_value()) {
        // If there is cached data, call the callback with the cached data
        ESP_LOGI(TAG, "Restored event '%s' data cache", bus_event_to_string(event));

        // Trigger an event for the cached data
        esp_event_post(EventBusBase, (int32_t)event, cache_data->data(), cache_data->size(), portMAX_DELAY);
    }

}

void EventBus::event_handler(void* arg, esp_event_base_t event_base, int32_t event_id, void* event_data) {
    // This function is called when an event is posted to the queue
    // It can be used to handle events in a more complex way if needed
    auto& subscriptions = get_instance().m_subscriptions;
    auto range = subscriptions.equal_range(static_cast<EventBusEvent>(event_id));
    for (auto it = range.first; it != range.second; ++it) {
        it->second(event_data);
    }
}
