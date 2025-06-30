#include "cache.h"

#include "esp_log.h"

static constexpr auto TAG = "Cache";
static const auto CACHE_PREFIX = "_cache_";

std::optional<std::vector<uint8_t>> Cache::read_data(const std::string& key) {
    auto key_path = CACHE_PREFIX + key;

    if (!m_data_storage->has_key(key_path)) {
        return std::nullopt; // Key not found
    }

    auto data = m_data_storage->get_binary(key_path);

    ESP_LOGI(TAG, "Read cached value for a key: %s", key.c_str());

    return data;
}

void Cache::write_data(const std::string& key, const void* data, size_t size) {
    auto key_path = CACHE_PREFIX + key;

    m_data_storage->set_binary(key_path, data, size);

    ESP_LOGI(TAG, "Saved cached value for a key: %s", key.c_str());
}
