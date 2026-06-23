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

static const char *TAG = "i2s_es8311";

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

void setup()
{
    Serial.begin(115200);
    I2C_init();

   //SD card init
  if(! SD_MMC.setPins(SD_SCK, SD_CMD, SD_D0, SD_D1, SD_D2, SD_D3))
  {
        Serial.println("Pin change failed!");
        return;
  }
   while(!SD_MMC.begin())
   {
      Serial.println("SD card does not exist,please insert SD card.");
      delay(100);
   }

    pinMode(AP_ENABLE, OUTPUT);
    digitalWrite(AP_ENABLE, LOW);
    
   //es8311 init
   if (es8311_codec_init() != ESP_OK) 
   {
      Serial.println("ES8311 init failed!");
      return;
   }
   //audio init
   audioInit();
   audio.setPinout(I2S_BCK, I2S_WS, I2S_DOUT, I2S_MCK);
   //audio.setVolumeSteps(255);
   audio.setVolume(10);
   if(!demo_music())
   {
    return;
   }

   //play music
   demo_music_play(0);
}

void loop()
{
    Serial.println("IDLE loop");
    delay(1000);
}
