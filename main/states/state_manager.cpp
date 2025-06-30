#include "state_manager.h"

#include <algorithm>

#include "esp_log.h"
#include "esp_event.h"

#include "connectivity/connected_state.h"
#include "connectivity/connection_test_state.h"
#include "connectivity/provisioning_state.h"
#include "ui/ui_provisioning_input_state.h"
#include "ui/ui_connection_test_state.h"
#include "ui/ui_connected_state.h"
#include "ui/ui_main_screen_state.h"
#include "ui/ui_prediction_state.h"

static constexpr auto TAG = "StateManager";

StateManager::StateManager()
{
    // Because we switch states from withing the states, we need to perform this asynchronously.
    if (xTaskCreate(&StateManager::switch_loop, "State Switcher", 8139, this, 10, &m_task_handle) != pdPASS)
    {
        ESP_LOGE(TAG, "Failed to create state switcher task");
        return;
    }

    ESP_LOGI(TAG, "StateManager initialized");
}

void StateManager::registerStates(std::shared_ptr<SPIFlash> http_static_partition,
                                  std::shared_ptr<SPIFlash> data_partition,
                                  std::shared_ptr<NVStorage> storage,
                                  std::shared_ptr<WiFiManager> wifi_manager,
                                  std::shared_ptr<Events> events,
                                  std::shared_ptr<Button> button)
{
    auto connectivity_subsystem = add_subsystem(1, "Connectivity");
    connectivity_subsystem->add_state(StateId::Provisioning,
                                      std::make_shared<ProvisioningState>(http_static_partition, storage, wifi_manager));
    connectivity_subsystem->add_state(StateId::ConnectionTest,
                                      std::make_shared<ConnectionTestState>(storage, wifi_manager, events));
    connectivity_subsystem->add_state(StateId::MainScreen,
                                      std::make_shared<ConnectedState>(storage, wifi_manager, events));

    auto ui_subsystem = add_subsystem(2, "UI");
    ui_subsystem->add_state(StateId::Provisioning, std::make_shared<UiProvisioningInputState>());
    ui_subsystem->add_state(StateId::ConnectionTest, std::make_shared<UiConnectionTestState>());
    ui_subsystem->add_state(StateId::Connected, std::make_shared<UiConnectedState>());
    ui_subsystem->add_state(StateId::MainScreen, std::make_shared<UiMainScreenState>(button));
    ui_subsystem->add_state(StateId::Prediction, std::make_shared<UiPredictionState>(button, data_partition, storage));
}

StateSubsystem::Pointer StateManager::add_subsystem(int priority, const std::string &&name)
{
    ESP_LOGI(TAG, "Adding subsystem: '%s' with priority: %d", name.c_str(), priority);

    auto subsystem = std::make_shared<StateSubsystem>(shared_from_this(), priority, std::move(name));
    m_subsystems.push_back(subsystem);

    std::sort(m_subsystems.begin(), m_subsystems.end(), [](const auto &a, const auto &b)
              { return a->priority() > b->priority(); });

    return subsystem;
}

void StateManager::switch_to(StateId state)
{
    // Note: the method uses a task notification to signal the state switch, not switches it immediately.
    xTaskNotify(m_task_handle, static_cast<uint32_t>(state), eSetValueWithOverwrite);

    ESP_LOGI(TAG, "State switch request sent: '%s'", state_id_to_string(state));
}

void StateManager::perform_switch(StateId new_state)
{
    ESP_LOGI(TAG, "Switching to state: '%s'", state_id_to_string(new_state));

    std::vector<StateSubsystem::Pointer> migrated_subsystems;
    for (const auto &subsystem : m_subsystems)
    {
        if (subsystem->switch_to(new_state))
        {
            migrated_subsystems.push_back(subsystem);
        }
        else
        {
            ESP_LOGW(TAG, "Subsystem '%s' failed to switch to state: '%s'", subsystem->name().c_str(), state_id_to_string(new_state));

            if (m_current_state == StateId::Init)
            {
                ESP_LOGE(TAG, "Cannot rollback from initial state");
                return; // Cannot rollback from initial state
            }

            for (const auto &migrated_subsystem : migrated_subsystems)
            {
                // Rollback previously migrated subsystems
                migrated_subsystem->switch_to(m_current_state);
                ESP_LOGI(TAG, "Rolled back subsystem '%s' to state: '%s'",
                         migrated_subsystem->name().c_str(),
                         state_id_to_string(m_current_state));
            }

            return; // If any subsystem fails, rollback and return
        }
    }

    m_current_state = new_state;
    ESP_LOGI(TAG, "Switched to state: '%s' successfully", state_id_to_string(new_state));
}

void StateManager::switch_loop(void *context)
{
    auto *self = static_cast<StateManager *>(context);

    ESP_LOGI(TAG, "StateManager loop started");

    uint32_t notification_value = 0;
    while (true)
    {
        if (xTaskNotifyWait(ULONG_MAX, 0, &notification_value, portMAX_DELAY) == pdTRUE)
        {
            self->perform_switch(static_cast<StateId>(notification_value));
        }
    }
}
