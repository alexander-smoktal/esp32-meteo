#include "spi_flash.h"

#include <fstream>
#include <iostream>
#include <wear_levelling.h>
#include "sdkconfig.h"

#include "esp_vfs.h"
#include "esp_vfs_fat.h"
#include "esp_log.h"

static const char *TAG = "SPIFlash";

SPIFlash::SPIFlash(const std::string &mount_path, const std::string &partition_name)
    : m_mount_path(mount_path)
{
    ESP_LOGI(TAG, "Mounting SPI Flash partition %s at: %s", partition_name.c_str(), mount_path.c_str());

    esp_vfs_fat_mount_config_t mount_config{};
    mount_config.max_files = 10;
    mount_config.format_if_mount_failed = false;
    mount_config.allocation_unit_size = CONFIG_WL_SECTOR_SIZE;

    esp_err_t err = esp_vfs_fat_spiflash_mount_rw_wl(mount_path.c_str(), partition_name.c_str(),
                                                     &mount_config, &wl_handle);
    ESP_ERROR_CHECK(err);
}

SPIFlash::~SPIFlash()
{
    esp_vfs_fat_spiflash_unmount_rw_wl(m_mount_path.c_str(), wl_handle);
}

std::fstream SPIFlash::open_file(const std::string& path, std::fstream::openmode open_mode)
{
    std::string true_path = m_mount_path;
    true_path.append("/");
    true_path.append(path);

    ESP_LOGI(TAG, "Opening file at: %s", true_path.c_str());

    std::fstream fs;
    fs.open(true_path, open_mode);

    if (!fs.good())
    {
        ESP_LOGI(TAG, "Failed to open file at: %s", true_path.c_str());
    }

    return fs;
}
