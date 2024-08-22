#include "prediction.h"

#include <fstream>

#include <esp_random.h>
#include <esp_log.h>

#include "flash.h"

static const char *TAG = "prediction";

static uint32_t NUM_PREDICTION_LINES = 190;
static const char *PREDICTIONS_FILE = "predictions.txt";

static const uint32_t MAX_HTTP_OUTPUT_BUFFER = 1000;
static char sg_local_response_buffer[MAX_HTTP_OUTPUT_BUFFER + 1] = {0};

Prediction::Prediction(UI *ui) : m_ui(ui) {}

void Prediction::toggle()
{
    if (m_toggled)
    {
        m_ui->toggle_prediction(nullptr);
    }
    else
    {
        auto linum = esp_random() % NUM_PREDICTION_LINES;
        auto pred_file = Flash::open_file(PREDICTIONS_FILE, std::fstream::in);

        if (!pred_file.good())
        {
            ESP_LOGI(TAG, "Failed to open predictions file");
            return;
        }

        for (size_t i = 0; i < linum; ++i)
        {
            pred_file.getline(sg_local_response_buffer, MAX_HTTP_OUTPUT_BUFFER);
        }

        m_ui->toggle_prediction(sg_local_response_buffer);
    }

    m_toggled = !m_toggled;
}