#pragma once

#include "nv_storage.h"

#include <atomic>
#include <cstddef>
#include <memory>
#include <vector>
#include <string>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include <esp_timer.h>

class Task {
public:
    virtual ~Task() = default;

    virtual bool execute() = 0;
};

class InternalTask : public Task {
public:
    InternalTask(std::shared_ptr<NVStorage> database);

    bool execute() override;

private:
    std::shared_ptr<NVStorage> m_database;
};

class Scheduler {
    struct TaskEntry {
        std::shared_ptr<Task> task;
        std::string name;
        size_t frequency_secs;
        size_t last_execution_num;
        TaskHandle_t task_handle;
    };

public:
    Scheduler(std::shared_ptr<NVStorage> db);
    static void taskLoop(void* context);

    void add_task(std::shared_ptr<Task> task, const std::string& name, size_t frequency_secs);
    void stop();

    static size_t hours(size_t hours) {
        return hours * 3600;
    }

    static size_t minutes(size_t minutes) {
        return minutes * 60;
    }

    static size_t seconds(size_t seconds) {
        return seconds;
    }

private:
    static void tick(void* scheduler);

    std::shared_ptr<NVStorage> m_database;
    std::vector<TaskEntry> m_tasks = {};
    esp_timer_handle_t m_timer_handle = nullptr;
};