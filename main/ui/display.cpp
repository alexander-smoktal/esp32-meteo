#include "display.h"

#include "esp_err.h"
#include "esp_log.h"
#include "esp_lvgl_port.h"
#include "esp_lcd_panel_interface.h"
#include "esp_lcd_panel_io.h"
#include "esp_lcd_panel_vendor.h"
#include "esp_lcd_panel_ops.h"
#include "esp_lcd_sh8601.h"
#include "driver/gpio.h"
#include "driver/spi_master.h"

#include "lvgl_meteo/screens/splash/splash_screen_gen.h"
#include "lvgl_meteo/ui.h"

#include "globals.h"

lv_obj_t *s_provisioning_screen = nullptr;

static constexpr auto TAG = "Display";
static constexpr int splash_anim_ms = 500;

static constexpr auto LCD_HOST = SPI2_HOST;

static constexpr auto LCD_H_RES = 320;
static constexpr auto LCD_V_RES = 170;

static constexpr auto LCD_DMA_LINE_SIZE = (LCD_V_RES / 2);

static constexpr auto PIN_NUM_RST = GPIO_NUM_9;
static constexpr auto PIN_NUM_CLK = GPIO_NUM_10;
static constexpr auto PIN_NUM_DC = GPIO_NUM_11;
static constexpr auto PIN_NUM_CS = GPIO_NUM_12;
static constexpr auto PIN_NUM_MOSI = GPIO_NUM_13;

static const sh8601_lcd_init_cmd_t lcd_init_cmds[] =
    {
        {0x36, (uint8_t[]){0x70}, 1, 0},
        {0xb2, (uint8_t[]){0x0c, 0x0c, 0x00, 0x33, 0x33}, 5, 0},
        {0xb7, (uint8_t[]){0x35}, 1, 0},
        {0xbb, (uint8_t[]){0x13}, 1, 0},
        {0xc0, (uint8_t[]){0x2c}, 1, 0},
        {0xc2, (uint8_t[]){0x01}, 1, 0},
        {0xc3, (uint8_t[]){0x0b}, 1, 0},
        {0xc4, (uint8_t[]){0x20}, 1, 0},
        {0xc6, (uint8_t[]){0x0f}, 1, 0},
        {0xd0, (uint8_t[]){0xa4, 0xa1}, 2, 0},
        {0xd6, (uint8_t[]){0xa1}, 1, 0},
        {0xe0, (uint8_t[]){0x00, 0x03, 0x07, 0x08, 0x07, 0x15, 0x2A, 0x44, 0x42, 0x0A, 0x17, 0x18, 0x25, 0x27}, 14, 0},
        {0xe1, (uint8_t[]){0x00, 0x03, 0x08, 0x07, 0x07, 0x23, 0x2A, 0x43, 0x42, 0x09, 0x18, 0x17, 0x25, 0x27}, 14, 0},
        {0x21, (uint8_t[]){0x21}, 0, 0},
        {0x11, (uint8_t[]){0x11}, 0, 120},
        {0x29, (uint8_t[]){0x29}, 0, 0},
};

esp_err_t swap_xy_stub(esp_lcd_panel_t *panel, bool swap_axes) { return ESP_OK; }

Display::Display()
{
    lvgl_port_cfg_t lvgl_cfg = ESP_LVGL_PORT_INIT_CONFIG();
    lvgl_cfg.task_priority = 1;
    lvgl_cfg.task_stack = 16384;
    lvgl_cfg.task_affinity = -1; // No affinity, run on any core
    ESP_ERROR_CHECK(lvgl_port_init(&lvgl_cfg));

    spi_bus_config_t buscfg = {};
    buscfg.mosi_io_num = PIN_NUM_MOSI;
    buscfg.sclk_io_num = PIN_NUM_CLK;
    buscfg.quadwp_io_num = -1;
    buscfg.quadhd_io_num = -1;

    ESP_ERROR_CHECK(spi_bus_initialize(LCD_HOST, &buscfg, SPI_DMA_CH_AUTO));

    esp_lcd_panel_io_handle_t io_handle = nullptr;
    esp_lcd_panel_io_spi_config_t io_config = {};
    io_config.dc_gpio_num = PIN_NUM_DC;
    io_config.cs_gpio_num = PIN_NUM_CS;
    io_config.pclk_hz = 80 * 1000 * 1000;
    io_config.lcd_cmd_bits = 8;
    io_config.lcd_param_bits = 8;
    io_config.spi_mode = 0;
    io_config.trans_queue_depth = 20;

    sh8601_vendor_config_t vendor_config = {};
    vendor_config.init_cmds = lcd_init_cmds;
    vendor_config.init_cmds_size = sizeof(lcd_init_cmds) / sizeof(lcd_init_cmds[0]);

    ESP_ERROR_CHECK(esp_lcd_new_panel_io_spi((esp_lcd_spi_bus_handle_t)LCD_HOST, &io_config, &io_handle));

    esp_lcd_panel_handle_t panel_handle = nullptr;
    esp_lcd_panel_dev_config_t panel_config = {};
    panel_config.reset_gpio_num = PIN_NUM_RST;
    panel_config.rgb_ele_order = LCD_RGB_ELEMENT_ORDER_RGB;
    panel_config.bits_per_pixel = 16;
    panel_config.vendor_config = &vendor_config;
    panel_config.data_endian = LCD_RGB_DATA_ENDIAN_BIG;

    ESP_ERROR_CHECK(esp_lcd_new_panel_sh8601(io_handle, &panel_config, &panel_handle));
    ESP_ERROR_CHECK(esp_lcd_panel_reset(panel_handle));
    ESP_ERROR_CHECK(esp_lcd_panel_init(panel_handle));
    ESP_ERROR_CHECK(esp_lcd_panel_set_gap(panel_handle, 0, 35));
    ESP_ERROR_CHECK(esp_lcd_panel_disp_on_off(panel_handle, false));
    // Aviod annoying errors during initialization. The display doesn't support swapping axes
    panel_handle->swap_xy = swap_xy_stub;

    lvgl_port_display_cfg_t disp_cfg = {};
    disp_cfg.io_handle = io_handle;
    disp_cfg.panel_handle = panel_handle;
    disp_cfg.buffer_size = LCD_H_RES * LCD_DMA_LINE_SIZE * sizeof(uint16_t);
    disp_cfg.double_buffer = true;
    disp_cfg.hres = LCD_H_RES;
    disp_cfg.vres = LCD_V_RES;
    disp_cfg.monochrome = false;
    disp_cfg.color_format = LV_COLOR_FORMAT_RGB565;

    disp_cfg.rotation.swap_xy = false;
    disp_cfg.rotation.mirror_x = false;
    disp_cfg.rotation.mirror_y = false;

    disp_cfg.flags.buff_dma = true;
    disp_cfg.flags.buff_spiram = true;
    disp_cfg.flags.swap_bytes = true;
    disp_cfg.flags.sw_rotate = true;

    m_display = lvgl_port_add_disp(&disp_cfg);
    lv_display_set_rotation(m_display, LV_DISPLAY_ROTATION_180);

    start_splash();
    vTaskDelay(200 / portTICK_PERIOD_MS);
    ESP_ERROR_CHECK(esp_lcd_panel_disp_on_off(panel_handle, true));
    vTaskDelay(splash_anim_ms / portTICK_PERIOD_MS);
}

void Display::start_splash()
{
    ui_init("F:/");
    auto splash_screen = splash_screen_create();

    if (lvgl_port_lock(-1))
    {
        lv_obj_set_style_bg_color(lv_screen_active(), lv_color_black(), 0);
        lv_screen_load_anim(splash_screen, LV_SCR_LOAD_ANIM_FADE_OUT, splash_anim_ms, 0, true);
        lvgl_port_unlock();
    }
}