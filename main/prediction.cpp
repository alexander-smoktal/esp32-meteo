#include "prediction.h"

#include <fstream>

#include <esp_random.h>
#include <esp_log.h>

#include "flash.h"
#include "predictions_list.h"

static const char *TAG = "prediction";

static const uint32_t NUM_PREDICTION_LINES = sizeof(PREDICTION_LIST) / sizeof(PREDICTION_LIST[0]);

static const uint32_t MAX_HTTP_OUTPUT_BUFFER = 1000;
static char sg_local_response_buffer[MAX_HTTP_OUTPUT_BUFFER + 1] = {0};

Prediction::Prediction(UI *ui) : m_ui(ui) {}

void Prediction::toggle()
{
    if (m_ui->is_prediction_toggled())
    {
        m_ui->toggle_prediction(nullptr);
    }
    else
    {
        auto linum = esp_random() % NUM_PREDICTION_LINES;
        m_ui->toggle_prediction(PREDICTION_LIST[linum]);
    }
}