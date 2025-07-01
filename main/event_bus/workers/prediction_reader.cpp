#include "prediction_reader.h"

#include <string>

#include "esp_random.h"
#include "esp_log.h"
#include "esp_timer.h"
#include "event_bus/event_bus.h"
#include "common.h"

static constexpr auto TAG = "PredictionReader";

static constexpr auto PREDICTIONS_NUM = 500;

std::string PredictionReader::read_prediction() {
    // Start timing
    int64_t start_time = esp_timer_get_time();

    auto file = m_data_partition->open_file("predictions.txt", std::fstream::in);

    if (!file.is_open()) {
        ESP_LOGE(TAG, "Failed to open predictions file");
        return {};
    }

    // Generate random number between 0 and PREDICTIONS_NUM-1
    uint32_t random_line = esp_random() % PREDICTIONS_NUM;

    // Read to the randomly selected line
    std::string selected_prediction;
    for (uint32_t i = 0; i <= random_line; i++) {
        if (!std::getline(file, selected_prediction)) {
            ESP_LOGE(TAG, "Failed to read line %lu", i + 1);
            file.close();
            return {};
        }
    }

    file.close();

    // Calculate elapsed time in milliseconds
    int64_t end_time = esp_timer_get_time();
    double elapsed_ms = (end_time - start_time) / 1000.0;

    ESP_LOGI(TAG, "Random prediction: %s", selected_prediction.c_str());
    ESP_LOGI(TAG, "Time to find prediction: %.2f ms", elapsed_ms);

    auto owner_name = m_storage->get_string(NVS_OWNER_NAME_KEY);

    return owner_name + ", " + selected_prediction;
}