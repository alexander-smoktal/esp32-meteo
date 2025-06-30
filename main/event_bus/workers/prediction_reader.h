#pragma once

#include <memory>

#include "spi_flash.h"
#include "nv_storage.h"

class PredictionReader {
public:
    PredictionReader(std::shared_ptr<SPIFlash> data_partition, std::shared_ptr<NVStorage> storage)
        : m_data_partition(data_partition)
        , m_storage(storage) {}

    // Method to read predictions from a source
    std::string read_prediction();

private:
    std::shared_ptr<SPIFlash> m_data_partition;
    std::shared_ptr<NVStorage> m_storage;
};
