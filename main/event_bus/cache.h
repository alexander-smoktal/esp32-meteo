#pragma once

#include <memory>
#include <optional>
#include <vector>
#include <string>

#include "nv_storage.h"

class Cache {
public:
    Cache(std::shared_ptr<NVStorage> data_storage)
        : m_data_storage(std::move(data_storage)) {}

    std::optional<std::vector<uint8_t>> read_data(const std::string& key);
    void write_data(const std::string& key, const void* data, size_t size);

    void clear_entry(const std::string& key) {
        m_data_storage->clear_entry(key);
    }

private:
    std::shared_ptr<NVStorage> m_data_storage;
};
