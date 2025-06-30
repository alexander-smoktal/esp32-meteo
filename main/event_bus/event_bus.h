#pragma once

#include <unordered_map>
#include <functional>

#include "esp_event.h"

#include "cache.h"

#include "event_bus_c.h"


class EventBus {
public:
    using EventSubscription = std::function<void(void* data)>;

    // Because we need C interfaces for the event bus, we create a singleton class.
    static EventBus& init_instance(std::shared_ptr<NVStorage> data_storage);
    static EventBus& get_instance();

    void pubish(EventBusEvent event, const void* data, size_t size, bool save_to_cache = true);
    void subscribe(EventBusEvent event, EventSubscription callback);

private:
    EventBus(std::shared_ptr<NVStorage> data_storage);
    static void event_handler(void* arg, esp_event_base_t event_base, int32_t event_id, void* event_data);

    std::unique_ptr<Cache> m_cache;
    std::unordered_multimap<EventBusEvent, EventSubscription> m_subscriptions;
};
