#pragma once

#include "esp_event.h"

#include "updater.h"

class WiFi
{
public:
    WiFi(Updater *updater);
    ~WiFi();

private:
    esp_err_t init_sta();
    esp_err_t init_sntp();
    static void event_handler(void *arg, esp_event_base_t event_base,
                              int32_t event_id, void *event_data);

    Updater *m_updater;
};