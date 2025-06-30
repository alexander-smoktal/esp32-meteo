#include "scheduler.h"

#include <cstring>
#include <ctime>
#include <iostream>
#include <thread>

#include <sys/time.h>

#include "esp_log.h"

static constexpr auto TAG = "Scheduler";

static const auto SCHEDULER_PREFIX = "_scdl_";
static const auto TIME_CACHE_KEY = "_time_cache";

InternalTask::InternalTask(std::shared_ptr<NVStorage> database)
    : m_database(std::move(database)) {
    if (m_database->has_key(TIME_CACHE_KEY)) {
        auto time_cache = m_database->get_binary(TIME_CACHE_KEY);

        timeval *tv = reinterpret_cast<timeval*>(time_cache.data());
        if (settimeofday(tv, nullptr) != 0) {
            ESP_LOGE(TAG, "Failed to set time from cache: %s", strerror(errno));
        } else {
            ESP_LOGI(TAG, "Time set from cache");
        }
    }
}

bool InternalTask::execute() {
    timeval tv;
    gettimeofday(&tv, nullptr);

    // Store the current time in the database
    m_database->set_binary(TIME_CACHE_KEY, &tv, sizeof(tv));

    return true; // Indicate success
}

Scheduler::Scheduler(std::shared_ptr<NVStorage> db) : m_database(std::move(db)) {
    // Start internal task
    add_task(std::make_shared<InternalTask>(m_database), "schd_task", seconds(1));

    // Initialize the timer
    esp_timer_create_args_t timer_args = {};
    timer_args.callback = &Scheduler::tick;
    timer_args.arg = this;
    timer_args.name = "Scheduler";

    ESP_ERROR_CHECK(esp_timer_create(&timer_args, &m_timer_handle));
    ESP_ERROR_CHECK(esp_timer_start_periodic(m_timer_handle, 1000000));
}

void Scheduler::add_task(std::shared_ptr<Task> task, const std::string& name, size_t frequency_secs) {
    auto key = SCHEDULER_PREFIX + name;

    size_t last_execution = -1;
    if (m_database->has_key(key)) {
        last_execution = (size_t)m_database->get_int(key);

        // Print next execution debug info
        {
            time_t now = 0;
            tm timeinfo = {};

            time(&now);
            localtime_r(&now, &timeinfo);

            auto secs_since_midnight = timeinfo.tm_hour * 3600 + timeinfo.tm_min * 60 + timeinfo.tm_sec;
            auto next_run = (last_execution + 1) * frequency_secs;

            if (next_run > secs_since_midnight) {
                auto diff = next_run - secs_since_midnight;
                ESP_LOGI(TAG, "Next '%s' task run in %zu hours, %zu minutes, %zu seconds",
                         name.c_str(),
                         diff / 3600,
                         (diff % 3600) / 60,
                         diff % 60);
            }
        }
    }

    // We create FreeRTOS task for each scheduler task so internal tasks wouldn't block each other.
    // This allows performing long running tasks without additional complexity.
    TaskHandle_t rtos_task_handle = nullptr;
    if (xTaskCreate(&Scheduler::taskLoop, name.c_str(), 4196, task.get(), 3, &rtos_task_handle) != pdPASS) {
        ESP_LOGW(TAG, "Failed to create task for %s", name.c_str());
        return;
    }

    m_tasks.push_back({std::move(task), std::move(name), frequency_secs, last_execution, rtos_task_handle});
}

void Scheduler::tick(void* scheduler) {
    auto* self = static_cast<Scheduler*>(scheduler);

    time_t now = 0;
    tm timeinfo = {};

    time(&now);
    localtime_r(&now, &timeinfo);

    auto secs_since_midnight = timeinfo.tm_hour * 3600 + timeinfo.tm_min * 60 + timeinfo.tm_sec;

    for (auto& entry : self->m_tasks) {
        auto task_execution_num = secs_since_midnight / entry.frequency_secs;

        ESP_LOGV(TAG, "Task %s execution number: %zu, last execution number: %zu",
                 entry.name.c_str(), task_execution_num, entry.last_execution_num);

        if (task_execution_num != entry.last_execution_num) {
            // We notify the task to execute the interface method.
            // This is done to avoid blocking the scheduler thread.
            if (xTaskNotify(entry.task_handle, 0, eNoAction) != pdPASS) {
                ESP_LOGW(TAG, "Failed to notify task %s", entry.name.c_str());
                continue;
            }

            entry.last_execution_num = task_execution_num;
            auto key = SCHEDULER_PREFIX + entry.name;

            // Store the last execution number in the database
            self->m_database->set_int(key, (int32_t)entry.last_execution_num);
        }
    }
}

void Scheduler::taskLoop(void* context) {
    auto task = static_cast<Task*>(context);
    while (true) {
        xTaskNotifyWait(0, 0, nullptr, portMAX_DELAY);
        // Execute the task
        task->execute();
    }
}