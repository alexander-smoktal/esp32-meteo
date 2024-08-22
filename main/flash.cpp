#include "flash.h"

#include <fstream>
#include <iostream>
#include <wear_levelling.h>
#include "sdkconfig.h"

#include "esp_vfs.h"
#include "esp_vfs_fat.h"
#include "esp_log.h"

const char *MOUNT_PATH = "/flash";
const char *PARTITION_NAME = "storage";

static const char *TAG = "fs";

Flash::Flash()
{
    esp_vfs_fat_mount_config_t mount_config{};
    mount_config.max_files = 4;
    mount_config.format_if_mount_failed = false;
    mount_config.allocation_unit_size = CONFIG_WL_SECTOR_SIZE;

    esp_err_t err = esp_vfs_fat_spiflash_mount_rw_wl(MOUNT_PATH, PARTITION_NAME,
                                                     &mount_config, &wl_handle);
    ESP_ERROR_CHECK(err);
}

Flash::~Flash()
{
    esp_vfs_fat_spiflash_unmount_rw_wl(MOUNT_PATH, wl_handle);
}

std::fstream Flash::open_file(const char *path, std::fstream::openmode open_mode)
{
    std::string true_path{MOUNT_PATH};
    true_path.append("/");
    true_path.append(path);

    ESP_LOGI(TAG, "Opening file at: %s", true_path.c_str());

    std::fstream fs;
    fs.open(true_path, open_mode);

    if (!fs.good())
    {
        ESP_LOGW(TAG, "Failed to open file at: %s", true_path.c_str());
    }

    return fs;
}
