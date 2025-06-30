#pragma once

#include <vector>
#include <memory>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include "state_subsystem.h"
#include "spi_flash.h"
#include "nv_storage.h"
#include "wifi/wifi_manager.h"
#include "io/button.h"
#include "event_bus/events.h"

class StateManager : public std::enable_shared_from_this<StateManager>
{
public:
    StateManager();
    void registerStates(std::shared_ptr<SPIFlash> http_static_fs,
                        std::shared_ptr<SPIFlash> data_partition,
                        std::shared_ptr<NVStorage> storage,
                        std::shared_ptr<WiFiManager> wifi_manager,
                        std::shared_ptr<Events> events,
                        std::shared_ptr<Button> button);

    StateSubsystem::Pointer add_subsystem(int priority, const std::string &&name);
    // Sends a request to switch to a new state
    void switch_to(StateId state);

private:
    // Performs the actual state switch, ensuring all subsystems are switched correctly
    void perform_switch(StateId new_state);
    // Loops waiting for state switch requests and performs them
    static void switch_loop(void *context);

    StateId m_current_state = StateId::Init;
    std::vector<StateSubsystem::Pointer> m_subsystems = {};
    TaskHandle_t m_task_handle = nullptr;
};
