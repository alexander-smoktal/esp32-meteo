#include "events.h"

#include "event_bus.h"
#include "workers/ntp_updater.h"
#include "workers/time_generator.h"
#include "workers/weather_fetcher.h"
#include "workers/forecast_fetcher.h"
#include "workers/bme_sensor_reader.h"

Events::Events(std::shared_ptr<NVStorage> storage)
    : m_scheduler(std::make_shared<Scheduler>(storage))
    , m_storage(std::move(storage)) {
    EventBus::init_instance(m_storage);
}

void Events::init_workers() {
    if (m_initialized) {
        return; // Already initialized
    }
    m_initialized = true;
    m_scheduler->add_task(std::make_shared<NtpUpdater>(m_storage), "ntp", Scheduler::hours(24));
    m_scheduler->add_task(std::make_shared<WeatherFetcher>(m_storage), "weather", Scheduler::hours(1));
    m_scheduler->add_task(std::make_shared<ForecastFetcher>(m_storage), "forcst", Scheduler::hours(3));
    m_scheduler->add_task(std::make_shared<TimeGenerator>(), "time", Scheduler::seconds(1));
    m_scheduler->add_task(std::make_shared<BmeSensorReader>(), "bme", Scheduler::seconds(10));
}
