#pragma once

#include <memory>

#include "scheduler.h"
#include "nv_storage.h"
#include "spi_flash.h"

// Event system initalizer
class Events {
public:
    Events(std::shared_ptr<NVStorage> storage);
    void init_workers();

private:
    std::shared_ptr<Scheduler> m_scheduler = std::make_shared<Scheduler>(std::make_shared<NVStorage>());
    std::shared_ptr<NVStorage> m_storage;
    bool m_initialized = false;
};
