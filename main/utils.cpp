#include "utils.h"

#include <esp_lvgl_port.h>
#include <esp_log.h>

static const char *TAG = "LVGL mem";

void print_lvgl_mem_usage(const char *ctx)
{
    lv_mem_monitor_t mon;
    lv_mem_monitor(&mon);

    ESP_LOGI(TAG, "%s: Total: %d, Free: %d, Used: %6d (%3d %%), Frag: %3d %%, Biggest free: %6d\n",
             ctx, mon.total_size,
             mon.free_size,
             (int)mon.total_size - mon.free_size,
             mon.used_pct,
             mon.frag_pct,
             (int)mon.free_biggest_size);
}