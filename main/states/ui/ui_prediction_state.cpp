#include "ui_prediction_state.h"

#include "esp_lvgl_port.h"
#include "esp_err.h"

#include "ui/lvgl_meteo/screens/prediction/prediction_screen_gen.h"

static constexpr auto TIMER_TIMEOUT_US = 10000 * 1000; // 10 seconds in microseconds

UiPredictionState::UiPredictionState(std::shared_ptr<Button> m_button,
                                     std::shared_ptr<SPIFlash> data_partition,
                                     std::shared_ptr<NVStorage> storage)
    : m_button(m_button)
    , m_prediction_reader(data_partition, storage) {
    // Initialize the timer that switches back to the main screen
    // after a timeout if the button is not pressed to switch off
    esp_timer_create_args_t timer_args = {};
    timer_args.callback = &UiPredictionState::timer_callback;
    timer_args.name = "PredictionOffTimer";
    timer_args.arg = this;
    timer_args.dispatch_method = ESP_TIMER_TASK;
    timer_args.skip_unhandled_events = false;

    ESP_ERROR_CHECK(esp_timer_create(&timer_args, &m_timer));
}

bool UiPredictionState::enter() {
    auto prediction = m_prediction_reader.read_prediction();
    m_button->register_callback(BUTTON_PRESS_DOWN, [this]() {
        switch_off();
    });

    // if (m_prediction_screen == nullptr) {
    m_prediction_screen = prediction_screen_create();
    // }

    if (lvgl_port_lock(-1)) {
        // Set the prediction text
        auto prediction_label = lv_obj_get_child(m_prediction_screen, 0);
        lv_label_set_text(prediction_label, prediction.c_str());
        lvgl_port_unlock();
    }

    lv_screen_load(m_prediction_screen);

    // Start the timer to switch off after TIMER_TIMEOUT_US
    ESP_ERROR_CHECK(esp_timer_start_once(m_timer, TIMER_TIMEOUT_US));

    return true;
}

void UiPredictionState::exit() {
    if (m_timer) {
        esp_timer_stop(m_timer);
    }
}

void UiPredictionState::timer_callback(void *arg) {
    auto self = static_cast<UiPredictionState *>(arg);

    self->switch_off();
}

void UiPredictionState::switch_off() {
    // Stop timer if early exit by button press
    switch_to(StateId::MainScreen);
}