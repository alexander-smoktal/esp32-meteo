#pragma once

#include <fstream>
#include <string>

/* SPI flash partition wrapper */
class SPIFlash
{
public:
    SPIFlash(const std::string &mount_path, const std::string &partition_name);
    ~SPIFlash();

    std::fstream open_file(const std::string& path, std::fstream::openmode open_mode);

private:
    int32_t wl_handle = 0;
    std::string m_mount_path;
};