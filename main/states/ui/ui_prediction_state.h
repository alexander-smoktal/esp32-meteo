#pragma once

#include "esp_timer.h"

#include "io/button.h"
#include "../state.h"
#include "event_bus/workers/prediction_reader.h"

#include "lvgl/lvgl.h"

// Show prediction screen
class UiPredictionState : public State {
public:
    UiPredictionState(std::shared_ptr<Button> button,
                      std::shared_ptr<SPIFlash> data_partition,
                      std::shared_ptr<NVStorage> storage);
    ~UiPredictionState() override = default;

    bool enter() override;
    void exit() override;

private:
    static void timer_callback(void *arg);
    void switch_off();

    esp_timer_handle_t m_timer = nullptr; // Switch off timer
    std::shared_ptr<Button> m_button;
    PredictionReader m_prediction_reader; // Prediction reader worker
    lv_obj_t *m_prediction_screen = nullptr; // Prediction screen object
};