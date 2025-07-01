#pragma once

#include <unordered_map>
#include <functional>

#include "esp_event.h"

#include "cache.h"

#include "event_bus_c.h"

/* And event bus is a system that allows different parts of the application to communicate with each other.
   Uses data cache to store events and their data and if cached value is available send it on subscription.
   It is a singleton class that can be used to publish and subscribe to events.
   Events are identified by an enum EventBusEvent, which is defined in event_bus_c.h
   and can be used to identify the event type.
*/
class EventBus {
public:
    using EventSubscription = std::function<void(void* data)>;

    // Because we need C interfaces for the event bus, we create a singleton class.
    static EventBus& init_instance(std::shared_ptr<NVStorage> data_storage);
    static EventBus& get_instance();

    // Publish data. If save_to_cache is true, the data will be saved to the cache.
    // If the data is already cached, it will be sent to all subscribers whn they subscribe.
    void pubish(EventBusEvent event, const void* data, size_t size, bool save_to_cache = true);

    // Subscribe to an event.
    // If the event is already cached, the data will be sent immediately.
    // If the event is not cached, the callback will be called when the event is published
    void subscribe(EventBusEvent event, EventSubscription callback);

private:
    EventBus(std::shared_ptr<NVStorage> data_storage);
    static void event_handler(void* arg, esp_event_base_t event_base, int32_t event_id, void* event_data);

    std::unique_ptr<Cache> m_cache;
    std::unordered_multimap<EventBusEvent, EventSubscription> m_subscriptions;
};
