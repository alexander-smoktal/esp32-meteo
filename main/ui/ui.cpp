#include "ui/ui.h"

#include "utils.h"
#include "fonts.h"

#include <esp_lvgl_port.h>
#include <driver/gpio.h>
#include <driver/spi_common.h>
#include <esp_lcd_panel_vendor.h>
#include <esp_log.h>

#define LCD_GPIO_RST (GPIO_NUM_3)
#define LCD_GPIO_DC (GPIO_NUM_4)
#define LCD_GPIO_BL (GPIO_NUM_5)
#define LCD_GPIO_SCLK (GPIO_NUM_21)
#define LCD_GPIO_MOSI (GPIO_NUM_19)
#define LCD_GPIO_CS (GPIO_NUM_18)

#define LCD_SPI_NUM (SPI2_HOST)
#define LCD_PIXEL_CLK_HZ (60 * 1000 * 1000)
#define LCD_CMD_BITS (8)
#define LCD_PARAM_BITS (8)
#define LCD_COLOR_SPACE (ESP_LCD_COLOR_SPACE_BGR)
#define LCD_BITS_PER_PIXEL (16)
#define LCD_DRAW_BUFF_DOUBLE (1)
#define LCD_DRAW_BUFF_HEIGHT (70)
#define LCD_BL_ON_LEVEL (1)

static const char *TAG = "ui";
static uint32_t FADE_TIME = 300;

UI::UI()
{
    ESP_ERROR_CHECK(lcd_init());
    ESP_ERROR_CHECK(lvgl_init());
    draw_ui();
}

esp_err_t
UI::lcd_init(void)
{
    ESP_LOGI(TAG, "Initializing LCD");

    esp_err_t ret = ESP_OK;

    /* LCD backlight */
    gpio_config_t bk_gpio_config = {};
    bk_gpio_config.mode = GPIO_MODE_OUTPUT;
    bk_gpio_config.pin_bit_mask = 1ULL << LCD_GPIO_BL;
    ESP_ERROR_CHECK(gpio_config(&bk_gpio_config));

    /* LCD initialization */
    spi_bus_config_t buscfg = {};
    buscfg.sclk_io_num = LCD_GPIO_SCLK;
    buscfg.mosi_io_num = LCD_GPIO_MOSI;
    buscfg.miso_io_num = GPIO_NUM_NC;
    buscfg.quadwp_io_num = GPIO_NUM_NC;
    buscfg.quadhd_io_num = GPIO_NUM_NC;
    buscfg.max_transfer_sz = LCD_H_RES * LCD_DRAW_BUFF_HEIGHT * sizeof(uint16_t);

    spi_bus_initialize(LCD_SPI_NUM, &buscfg, SPI_DMA_CH_AUTO);

    esp_lcd_panel_io_spi_config_t io_config = {};
    io_config.dc_gpio_num = LCD_GPIO_DC;
    io_config.cs_gpio_num = LCD_GPIO_CS;
    io_config.pclk_hz = LCD_PIXEL_CLK_HZ;
    io_config.lcd_cmd_bits = LCD_CMD_BITS;
    io_config.lcd_param_bits = LCD_PARAM_BITS;
    io_config.spi_mode = 0;
    io_config.trans_queue_depth = 20;

    esp_lcd_new_panel_io_spi((esp_lcd_spi_bus_handle_t)LCD_SPI_NUM, &io_config, &lcd_io);

    esp_lcd_panel_dev_config_t panel_config = {};
    panel_config.reset_gpio_num = LCD_GPIO_RST;
    panel_config.rgb_ele_order = LCD_RGB_ELEMENT_ORDER_RGB;
    panel_config.bits_per_pixel = LCD_BITS_PER_PIXEL;

    esp_lcd_new_panel_st7789(lcd_io, &panel_config, &lcd_panel);

    esp_lcd_panel_reset(lcd_panel);
    esp_lcd_panel_init(lcd_panel);
    esp_lcd_panel_swap_xy(lcd_panel, true);
    esp_lcd_panel_mirror(lcd_panel, 1, 0);
    esp_lcd_panel_invert_color(lcd_panel, true);
    esp_lcd_panel_set_gap(lcd_panel, 20, 0);
    esp_lcd_panel_disp_on_off(lcd_panel, true);

    return ret;
}

esp_err_t UI::lvgl_init(void)
{
    ESP_LOGI(TAG, "Initializing LVGL");

    /* Initialize LVGL */
    lvgl_port_cfg_t lvgl_cfg = {};
    lvgl_cfg.task_priority = 4;       /* LVGL task priority */
    lvgl_cfg.task_stack = 8096;       /* LVGL task stack size */
    lvgl_cfg.task_affinity = -1;      /* LVGL task pinned to core (-1 is no affinity) */
    lvgl_cfg.task_max_sleep_ms = 500; /* Maximum sleep in LVGL task */
    lvgl_cfg.timer_period_ms = 5;     /* LVGL timer tick period in ms */

    lvgl_port_init(&lvgl_cfg);

    /* Add LCD screen */
    lvgl_port_display_cfg_t disp_cfg = {};
    disp_cfg.io_handle = lcd_io;
    disp_cfg.panel_handle = lcd_panel;
    disp_cfg.buffer_size = LCD_H_RES * LCD_DRAW_BUFF_HEIGHT;
    disp_cfg.double_buffer = LCD_DRAW_BUFF_DOUBLE;
    disp_cfg.hres = LCD_H_RES;
    disp_cfg.vres = LCD_V_RES;
    disp_cfg.monochrome = false;
#if LVGL_VERSION_MAJOR >= 9
    disp_cfg.color_format = LV_COLOR_FORMAT_RGB565;
#endif

    disp_cfg.rotation.swap_xy = false;
    disp_cfg.rotation.mirror_x = true;
    disp_cfg.rotation.mirror_y = true;

    disp_cfg.flags.buff_dma = true;
#if LVGL_VERSION_MAJOR >= 9
    disp_cfg.flags.swap_bytes = true,
#endif

    lvgl_disp = lvgl_port_add_disp(&disp_cfg);

    lv_lodepng_init();

    return ESP_OK;
}

void UI::draw_ui()
{
    ESP_LOGI(TAG, "Drawing UI");

    lv_obj_t *scr = lv_scr_act();

    print_lvgl_mem_usage("Before splash");

    /* Task lock */
    lvgl_port_lock(0);
    lv_display_set_rotation(lvgl_disp, LV_DISPLAY_ROTATION_90);

    lv_color_t black = {0, 0, 0};
    lv_obj_set_style_bg_color(scr, black, 0);

    /* Task unlock */
    lvgl_port_unlock();

    splash();

    print_lvgl_mem_usage("Splash created");
}

void UI::splash()
{
    lv_obj_t *scr = lv_scr_act();

    /* Task lock */
    lvgl_port_lock(0);
    m_logo = std::make_unique<Logo>();
    m_logo->attach(scr);
    lvgl_port_unlock();

    /* LCD backlight on */
    vTaskDelay(pdMS_TO_TICKS(100));
    ESP_ERROR_CHECK(gpio_set_level(LCD_GPIO_BL, LCD_BL_ON_LEVEL));
}

void UI::start_task()
{
    ESP_LOGI(TAG, "Starting UI task");
}

void UI::time_updated()
{
    m_time_label->time_updated();
}

void UI::set_outdoor_weather(WeatherIconType icon, float temp, float humidity)
{
    if (m_current_weather)
    {
        m_current_weather->set_outdoor(icon, temp, humidity);
    }
}

void UI::set_indoor_weather(float temp, float humidity)
{
    if (m_current_weather)
    {
        m_current_weather->set_indoor(temp, humidity);
    }
}

void UI::set_forecast(std::vector<ForecastDay> data)
{
    if (m_forecast)
    {
        m_forecast->set_forecast(data);
    }
}

void UI::splash_fade_out()
{
    if (!m_logo)
    {
        return;
    }

    m_logo->fade_out(FADE_TIME);
    vTaskDelay(pdMS_TO_TICKS(FADE_TIME));

    lv_obj_t *scr = lv_scr_act();

    lvgl_port_lock(0);
    m_logo->detach();
    m_logo.reset();

    print_lvgl_mem_usage("Splash deleted");

    m_grid = lv_obj_create(scr);
    lv_obj_set_layout(m_grid, LV_LAYOUT_GRID);
    lv_obj_set_style_bg_opa(m_grid, 0, 0);
    lv_obj_set_style_border_opa(m_grid, 0, 0);
    lv_obj_set_scrollbar_mode(m_grid, LV_SCROLLBAR_MODE_OFF);
    lv_obj_set_style_pad_row(m_grid, 0, 0);
    lv_obj_set_style_pad_column(m_grid, 0, 0);
    lv_obj_set_size(m_grid, LCD_V_RES, LCD_H_RES + 10);
    lv_obj_align(m_grid, LV_ALIGN_CENTER, -15, -10);

    static int32_t column_dsc[] = {LCD_V_RES, LV_GRID_TEMPLATE_LAST};
    static int32_t row_dsc[] = {70, 80, 70, LV_GRID_TEMPLATE_LAST};
    lv_obj_set_grid_dsc_array(m_grid, column_dsc, row_dsc);

    m_time_label = std::make_unique<TimeLabel>();
    auto time_root = m_time_label->root(m_grid);
    lv_obj_set_grid_cell(time_root, LV_GRID_ALIGN_STRETCH, 0, 1, LV_GRID_ALIGN_START, 0, 1);
    print_lvgl_mem_usage("Time added");

    m_current_weather = std::make_unique<CurrentWeather>();
    auto weather_root = m_current_weather->root(m_grid);
    lv_obj_set_grid_cell(weather_root, LV_GRID_ALIGN_STRETCH, 0, 1, LV_GRID_ALIGN_START, 1, 1);
    print_lvgl_mem_usage("Weather added");

    m_forecast = std::make_unique<Forecast>();
    auto pred_root = m_forecast->root(m_grid);
    lv_obj_set_grid_cell(pred_root, LV_GRID_ALIGN_STRETCH, 0, 1, LV_GRID_ALIGN_START, 2, 1);
    print_lvgl_mem_usage("Forecast added");

    lv_color_t prediction_color = {255, 255, 255};
    m_prediction = lv_label_create(scr);
    lv_obj_set_style_text_align(m_prediction, LV_TEXT_ALIGN_CENTER, 0);
    lv_obj_set_style_text_font(m_prediction, &noto_24, 0);
    lv_obj_set_style_text_color(m_prediction, prediction_color, 0);
    lv_obj_align(m_prediction, LV_ALIGN_CENTER, 0, 0);
    lv_obj_set_width(m_prediction, UI::LCD_V_RES);
    lv_label_set_text(m_prediction, "");
    print_lvgl_mem_usage("Prediciton added");

    lvgl_port_unlock();
}

void UI::toggle_prediction(const char *prediction)
{
    if (m_logo)
    {
        return;
    }

    lvgl_port_lock(0);

    if (prediction)
    {
        ESP_LOGI(TAG, "Prediction on: %s", prediction);

        lv_label_set_text(m_prediction, prediction);
        lv_obj_set_style_opa(m_prediction, 255, 0);
        lv_obj_set_style_opa(m_grid, 0, 0);
    }
    else
    {
        ESP_LOGI(TAG, "Prediction off");

        lv_obj_set_style_opa(m_prediction, 0, 0);
        lv_obj_set_style_opa(m_grid, 255, 0);
    }

    lvgl_port_unlock();
}

void UI::toggle_light()
{
    m_backlight_on = !m_backlight_on;

    ESP_ERROR_CHECK(gpio_set_level(LCD_GPIO_BL, m_backlight_on ? 1 : 0));
}