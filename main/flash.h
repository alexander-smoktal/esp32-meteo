#pragma once

#include <fstream>

class Flash
{
public:
    Flash();
    ~Flash();

    static std::fstream open_file(const char *path, std::fstream::openmode open_mode);

private:
    int32_t wl_handle = 0;
};