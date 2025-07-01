#include "time_generator.h"

#include <ctime>
#include "esp_log.h"

#include "../event_bus.h"

TimeGenerator::TimeGenerator() {
    execute(); // Initialize the time by executing the task immediately
}

bool TimeGenerator::execute() {
    // This method should generate the current time and publish it to the event bus.
    // For now, we will just log the current time.
    time_t now = time(nullptr);
    struct tm *timeinfo = localtime(&now);

    EventBus::get_instance().pubish(EventBusEvent::TimeEvent, timeinfo, sizeof(*timeinfo), false);
    return true;
}
