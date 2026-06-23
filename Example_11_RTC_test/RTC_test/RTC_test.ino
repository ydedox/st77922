//This example function displays the time using RTC

//pin usage as follow:
//                   CS  DC/RS  RESET    SDI/MOSI  SCK   SDO/MISO  BL      VCC    GND    
//ESP32-WROOM-32E:   15    2   ESP32-EN     13      14      12     21      5V     GND  

/***********************************************************************************
* @attention
*
* THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
* WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
* TIME. AS A RESULT, QD electronic SHALL NOT BE HELD LIABLE FOR ANY
* DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
* FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE 
* CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
**********************************************************************************/

#include <TFT_eSPI.h> 
#include <ESP32Time.h>    //ESP32Time library installation required
#include "ST77922.h"

#define CENTER 320
ESP32Time esp32_rtc;
char t_buf[100];          //Store RTC information

TFT_eSPI tft = TFT_eSPI();
TFT_eSprite my_lcd = TFT_eSprite(&tft);
ST77922 mylcd = ST77922();
 
void setup() 
{
    Serial.begin(115200);
    mylcd.Init();
    mylcd.Set_Rotation(0);
    my_lcd.createSprite(mylcd.Get_Width(), mylcd.Get_Height());
    my_lcd.setSwapBytes(1);
    my_lcd.fillSprite(TFT_WHITE);
    my_lcd.setFreeFont(&FreeSansBold18pt7b);
    my_lcd.setTextColor(TFT_RED);
    my_lcd.drawString("ESP32 RTC TEST", 10, 30);
    my_lcd.setTextFont(4);
    esp32_rtc.setTime(28, 48, 15, 26, 8, 2024);  //Set initial time: 2024-07-05, 16:15:28
}

void loop() 
{
    sprintf(t_buf, "Time : %02d:%02d:%02d", esp32_rtc.getHour(true), esp32_rtc.getMinute(), esp32_rtc.getSecond());   
    my_lcd.fillRect(112, 100, my_lcd.width()/2, 32,TFT_WHITE);   
    my_lcd.drawString(t_buf, 30, 100); 
    sprintf(t_buf, "Date : %04d-%02d-%02d", esp32_rtc.getYear(), esp32_rtc.getMonth() + 1, esp32_rtc.getDay());
    my_lcd.fillRect(112, 140, my_lcd.width()/2, 32,TFT_WHITE); 
    my_lcd.drawString(t_buf, 30, 140); 
    sprintf(t_buf, "Week : %d", esp32_rtc.getDayofWeek());
    my_lcd.fillRect(120, 180, my_lcd.width()/2, 32,TFT_WHITE); 
    my_lcd.drawString(t_buf, 30, 180); 
    mylcd.Fill_Colors(0, 0, mylcd.Get_Width(), mylcd.Get_Height(), (uint16_t *)my_lcd.getPointer());
    delay(1000);
}
