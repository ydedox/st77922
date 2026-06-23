#include <Arduino.h>
#include "FS.h"
#include "SD_MMC.h"
#include "SPI.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_system.h"
#include "esp_log.h"
#include "esp_err.h"
#include "esp_check.h"
#include "es8311.h"
#include "Audio.h"
#include "demo_music.h"
#include "ESP_Panel_Board_Custom.h"

#define EXAMPLE_MCLK_MULTIPLE I2S_MCLK_MULTIPLE_384

static const char *TAG = "i2s_es8311";
static i2s_chan_handle_t tx_handle = NULL;
static i2s_chan_handle_t rx_handle = NULL;
Audio audio;

void audioTask(void *parameter)
{
  while (true) 
  {
    audio.loop();
    vTaskDelay(1);
  }
}

void audioInit() 
{
  xTaskCreatePinnedToCore(
    audioTask,
    "audioplay",
    5000,
    NULL,
    2 | portPRIVILEGE_BIT,
    NULL,
    0
  );
}

esp_err_t I2C_init(void)
{
  const i2c_config_t es_i2c_cfg = {
        .mode = I2C_MODE_MASTER,
        .sda_io_num = I2C_SDA,
        .scl_io_num = I2C_SCL,
        .sda_pullup_en = GPIO_PULLUP_ENABLE,
        .scl_pullup_en = GPIO_PULLUP_ENABLE,
        .master = { 
          .clk_speed = I2C_SPEED,
        },
    };
    ESP_RETURN_ON_ERROR(i2c_param_config(I2C_NUM, &es_i2c_cfg), TAG, "config i2c failed");
    ESP_RETURN_ON_ERROR(i2c_driver_install(I2C_NUM, I2C_MODE_MASTER,  0, 0, 0), TAG, "install i2c driver failed");
    return ESP_OK;
}

esp_err_t i2s_driver_init(void)
{
    i2s_chan_config_t chan_cfg = I2S_CHANNEL_DEFAULT_CONFIG(I2S_NUM, I2S_ROLE_MASTER);
    chan_cfg.auto_clear = true; // Auto clear the legacy data in the DMA buffer
    esp_err_t err = i2s_new_channel(&chan_cfg, &tx_handle, &rx_handle);
    if (err != ESP_OK) {
        ESP_LOGE(TAG, "Failed to create I2S channel: %s", esp_err_to_name(err));
        return err;
    }
    ESP_LOGI(TAG, "I2S channel created successfully");

    i2s_std_config_t std_cfg = {
        .clk_cfg = I2S_STD_CLK_DEFAULT_CONFIG(EXAMPLE_SAMPLE_RATE),
        .slot_cfg = I2S_STD_PHILIPS_SLOT_DEFAULT_CONFIG(I2S_DATA_BIT_WIDTH_16BIT, I2S_SLOT_MODE_STEREO),
        .gpio_cfg = {
            .mclk = GPIO_NUM_17 ,
            .bclk = GPIO_NUM_18 ,
            .ws = GPIO_NUM_21 ,
            .dout = GPIO_NUM_15 ,
            .din = GPIO_NUM_16 ,
            .invert_flags = {
             .mclk_inv = false,
             .bclk_inv = false,
             .ws_inv = false,
            },
        },
    };
    std_cfg.clk_cfg.mclk_multiple = EXAMPLE_MCLK_MULTIPLE;

    err = i2s_channel_init_std_mode(tx_handle, &std_cfg);
    if (err != ESP_OK) {
        ESP_LOGE(TAG, "Failed to init TX channel in std mode: %s", esp_err_to_name(err));
        i2s_del_channel(tx_handle);
        i2s_del_channel(rx_handle);
        return err;
    }
    err = i2s_channel_init_std_mode(rx_handle, &std_cfg);
    if (err != ESP_OK) {
        ESP_LOGE(TAG, "Failed to init RX channel in std mode: %s", esp_err_to_name(err));
        i2s_del_channel(tx_handle);
        i2s_del_channel(rx_handle);
        return err;
    }
    err = i2s_channel_enable(tx_handle);
    if (err != ESP_OK) {
        ESP_LOGE(TAG, "Failed to enable TX channel: %s", esp_err_to_name(err));
        i2s_del_channel(tx_handle);
        i2s_del_channel(rx_handle);
        return err;
    }
    err = i2s_channel_enable(rx_handle);
    if (err != ESP_OK) {
        ESP_LOGE(TAG, "Failed to enable RX channel: %s", esp_err_to_name(err));
        i2s_del_channel(tx_handle);
        i2s_del_channel(rx_handle);
        return err;
    }
    return ESP_OK;
}

static void i2s_echo(void *args)
{
    int *mic_data = static_cast<int*>(malloc(EXAMPLE_RECV_BUF_SIZE));
    if (!mic_data) {
        ESP_LOGE(TAG, "[echo] No memory for read data buffer");
        abort();
    }
    esp_err_t ret = ESP_OK;
    size_t bytes_read = 0;
    size_t bytes_write = 0;
    ESP_LOGI(TAG, "[echo] Echo start");

    while (1) {
        memset(mic_data, 0, EXAMPLE_RECV_BUF_SIZE);
        /* Read sample data from mic */
        ret = i2s_channel_read(rx_handle, mic_data, EXAMPLE_RECV_BUF_SIZE, &bytes_read, 1000);
        if (ret != ESP_OK) {
            ESP_LOGE(TAG, "[echo] i2s read failed, %s", esp_err_to_name(ret));
            abort();
        }
        /* Write sample data to earphone */
        ret = i2s_channel_write(tx_handle, mic_data, EXAMPLE_RECV_BUF_SIZE, &bytes_write, 1000);
        if (ret != ESP_OK) {
            ESP_LOGE(TAG, "[echo] i2s write failed, %s", esp_err_to_name(ret));
            abort();
        }
        if (bytes_read != bytes_write) {
            ESP_LOGW(TAG, "[echo] %d bytes read but only %d bytes are written", bytes_read, bytes_write);
        }
    }
    vTaskDelete(NULL);
}

void setup()
{
    Serial.begin(115200);
    I2C_init();

    pinMode(AP_ENABLE, OUTPUT);
    digitalWrite(AP_ENABLE, LOW);
    //i2s init
   if (i2s_driver_init() != ESP_OK) 
   {
      Serial.println("i2s init failed!");
      return;
   }
   
   //es8311 init
   if (es8311_codec_init() != ESP_OK) 
   {
      Serial.println("ES8311 init failed!");
      return;
   }
   
   xTaskCreate(i2s_echo, "i2s_echo", 8192, NULL, 5, NULL);
}

void loop()
{
    Serial.println("IDLE loop");
    delay(1000);
}