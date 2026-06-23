//How does this program connect to WiFi AP mode.

//pin usage as follow:
//            CS  DC/RS  RESET  SDI/MOSI  SCK  SDO/MISO  LED    VCC     GND    
//ESP32-S3:   10    2      15      11      12      13     21     5V     GND   

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
#include <WiFi.h>
#include "ST77922.h"

//AP mode SSID and PWD
const char *ssid = "ESP32_AP";
const char *password = "0123456789";

char t_buf[100] = {0};
TFT_eSPI tft = TFT_eSPI();
TFT_eSprite my_lcd = TFT_eSprite(&tft);
ST77922 mylcd = ST77922();  

void setup()
{
  Serial.begin(115200);
  mylcd.Init();
  mylcd.Set_Rotation(0);
  my_lcd.createSprite(mylcd.Get_Width(), mylcd.Get_Height());
  my_lcd.setSwapBytes(0);
  my_lcd.fillSprite(TFT_WHITE);
  my_lcd.setTextColor(TFT_RED);
  my_lcd.drawString("Enable ESP32 WIFI AP mode...", 22,85,2);
  WiFi.softAP(ssid, password);
  my_lcd.setTextColor(TFT_BLUE);
  sprintf(t_buf, "SSID : %s", ssid);
  my_lcd.drawString(t_buf, 22,105,2);
  sprintf(t_buf, "PASSWORD : %s", password);
  my_lcd.drawString(t_buf, 22,125,2);
  sprintf(t_buf, "HOST IP : %s", WiFi.softAPIP().toString().c_str());
  my_lcd.drawString(t_buf, 22,145,2);
  sprintf(t_buf, "HOST MAC : %s", WiFi.softAPmacAddress().c_str());
  my_lcd.drawString(t_buf, 22,165,2);
  my_lcd.drawString("CONNECTED NUMBER : ", 22,195,2);
  mylcd.Fill_Colors(0, 0, mylcd.Get_Width(), mylcd.Get_Height(),(uint16_t*)my_lcd.getPointer());
}


void loop()
{
  sprintf(t_buf, "%d", WiFi.softAPgetStationNum());
  my_lcd.fillRect(174, 195, my_lcd.width()-80, 25,TFT_WHITE);
  my_lcd.drawString(t_buf, 174,195,2);
  mylcd.Fill_Colors(0, 0, mylcd.Get_Width(), mylcd.Get_Height(),(uint16_t*)my_lcd.getPointer());
  delay(1000);
}
