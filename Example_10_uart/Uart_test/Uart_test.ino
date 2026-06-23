//This program is a demo of how to use uart to send and receive messages

//pin usage as follow:
//            CS  DC/RS  RESET  SDI/MOSI  SCK  SDO/MISO  LED   SD_CS   VCC     GND    
//ESP32-S3:   10    2      15      11      12      13     21    7       5V     GND  

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
#include <SPI.h>
#include "ST77922.h"


String r_data;

#define CENTER 320

TFT_eSPI tft = TFT_eSPI();
TFT_eSprite my_lcd = TFT_eSprite(&tft);
ST77922 mylcd = ST77922();

void setup() 
{
  Serial.begin(115200); //Set the serial port baud rate 115200
  mylcd.Init();
  mylcd.Set_Rotation(0);
  my_lcd.createSprite(mylcd.Get_Width(), mylcd.Get_Height());
  my_lcd.setSwapBytes(1);
  my_lcd.fillScreen(TFT_BLACK);
  my_lcd.setTextColor(TFT_GREEN);
  my_lcd.drawString("This is uart test!!",0,10,2);
  my_lcd.drawString("receive data :",0,25,2);
  my_lcd.setTextColor(TFT_WHITE);
  Serial.println(55, BIN); //Binary
  Serial.println(55, OCT); //octonary
  Serial.println(55, DEC); //decimalism
  Serial.println(55, HEX); //hexadecimal
  Serial.println(9.19999, 0); //Keep 0 decimal places
  Serial.println(9.11999, 1); //Keep 1 decimal places
  Serial.println(9.11119, 4); //Keep 4 decimal places
  Serial.println('Q');
  Serial.println("Hello! this send.");
  Serial.print("x =");
  Serial.print(20);
  Serial.print(",y =");
  Serial.print(40);
  Serial.print('\n');
  mylcd.Fill_Colors(0, 0, mylcd.Get_Width(), mylcd.Get_Height(), (uint16_t *)my_lcd.getPointer());
}
void loop() 
{
  int i = 0,j=0;
  String temp;
  if(Serial.available() > 0)//Serial port receives data
  {
      r_data = Serial.readString();//Obtain the data received by the serial port
      Serial.println(r_data);
      //Serial.println(r_data.length());
      my_lcd.fillRect(0, 40, my_lcd.width(), my_lcd.height()-40,TFT_BLACK);
      if((r_data.length()-1)<30)
      {
        my_lcd.drawString(r_data,5,40,2);   
      }
      else
      {
        for(i=0; i<=((r_data.length()-1)/29); i++)
        {
          j = r_data.length()-i*29;
          if(j<29)
          {
              temp = r_data.substring(29*i);
          }
          else
          {
            temp = r_data.substring(29*i,29*i+29);
          }
          my_lcd.drawString(temp,5,40+15*i,2);
        }
      }
  }
  mylcd.Fill_Colors(0, 0, mylcd.Get_Width(), mylcd.Get_Height(), (uint16_t *)my_lcd.getPointer());
  delay(1000);
}
