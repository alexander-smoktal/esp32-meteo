set(SOURCES
    "main.cpp"
    "nv_storage.cpp"
    "http_server/http_server.cpp"
    "wifi/wifi_manager.cpp"
    "io/bme_sensor.cpp"
    "io/button.cpp"
    "spi_flash.cpp"
)

include(${CMAKE_CURRENT_SOURCE_DIR}/../main/ui/Ui.cmake)
include(${CMAKE_CURRENT_SOURCE_DIR}/../main/states/States.cmake)
include(${CMAKE_CURRENT_SOURCE_DIR}/../main/event_bus/Events.cmake)

idf_component_register(SRCS ${SOURCES}
                       INCLUDE_DIRS "." "ui/lvgl_meteo")

target_compile_features(${COMPONENT_LIB} PRIVATE cxx_std_23)

# A define for LVGL editor to not compile platform specific code
target_compile_definitions(${COMPONENT_LIB} PRIVATE MS_DEVICE_BUILD=1 NDEBUG=1)
# Disable warnings for LVGL editor generated code
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wno-discarded-qualifiers -Wno-unused-variable")

# Uncomment when flashing fir the first time
# fatfs_create_spiflash_image(static "http_server/static" FLASH_IN_PROJECT)
# fatfs_create_spiflash_image(data "data" FLASH_IN_PROJECT)
# fatfs_create_spiflash_image(images "ui/lvgl_meteo/images" FLASH_IN_PROJECT)
