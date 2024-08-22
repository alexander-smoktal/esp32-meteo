set(EXTRA_COMPONENT_DIRS "../components")
set(COMPONENT_ADD_INCLUDEDIRS "display")

idf_component_register(SRCS "logo_image.c" "flash.cpp" "cpu_info.cpp" "rgb.cpp" "utils.cpp" "buttons.cpp"
                        "main.cpp" "wifi.cpp" "updater.cpp" "sensor.cpp" "prediction.cpp"
                        "ui/ui.cpp" "ui/time_label.cpp" "ui/logo.cpp" "ui/current_weather.cpp"
                        "ui/forecast.cpp" "fonts/noto_36.c"
                        "fonts/noto_24.c" "fonts/noto_15.c" "fonts/noto_60.c"
                        INCLUDE_DIRS ".")

fatfs_create_spiflash_image(storage "../flash_image" FLASH_IN_PROJECT)