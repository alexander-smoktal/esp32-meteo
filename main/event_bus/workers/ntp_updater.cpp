#include "ntp_updater.h"

#include <ctime>

#include "freertos/FreeRTOS.h"
#include "freertos/event_groups.h"
#include "esp_log.h"
#include "esp_netif_sntp.h"

static constexpr auto TAG = "NTP Updater";
static constexpr auto SNTP_TIME_SERVER = "pool.ntp.org"; // Default NTP server

static void sync_cb(struct timeval *tv) {
    struct tm *timeinfo = localtime(&tv->tv_sec);
    ESP_LOGI(TAG, "Time update: %02d:%02d:%02d", timeinfo->tm_hour, timeinfo->tm_min, timeinfo->tm_sec);
}

NtpUpdater::NtpUpdater(std::shared_ptr<NVStorage> storage)
{
    ESP_LOGI(TAG, "Initializing NTP Updater");

    if (storage->has_key("time_zone")) {
        auto time_zone = storage->get_string("time_zone");
        ESP_LOGI(TAG, "Setting time zone from storage: %s", time_zone.c_str());

        setenv("TZ", time_zone.c_str(), 1);
    } else {
        ESP_LOGW(TAG, "No time zone found in storage, using default EET-2EEST-3");

        // Default to Eastern European Summer Time (EEST) with a 2-hour offset and 3-hour DST
        setenv("TZ", "EET-2EEST-3,M4.5.0/3,M10.5.0/4", 1);
    }

    tzset();

    esp_sntp_config_t config = ESP_NETIF_SNTP_DEFAULT_CONFIG(SNTP_TIME_SERVER);
    config.sync_cb = sync_cb; // Set the callback for successful synchronization
    ESP_ERROR_CHECK(esp_netif_sntp_init(&config));
}

// Note: Netif SNTP server will react to new IP events, so we don't need to handle them manually here.
// This code is a stup if need manual updating
bool NtpUpdater::execute()
{
    ESP_LOGI(TAG, "Updating NTP time");

    // Start SNTP
    ESP_ERROR_CHECK(esp_netif_sntp_start());

    return true;
}
