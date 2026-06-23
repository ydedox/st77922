//This example is to build a TCP server-side

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
#include <WiFi.h>
#include "ST77922.h"

//Manually modifying parameters
const char *ssid = "QDtech";
const char *password = "quandong0515";

char t_buf[100] = {0};
int port = 10000;

WiFiServer server(port); //Declare server objects

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
    WiFi.mode(WIFI_STA);
    WiFi.setSleep(false);
    my_lcd.drawString("Start connecting to WiFi ...", 10,55,2);
    mylcd.Fill_Colors(0, 0, mylcd.Get_Width(), mylcd.Get_Height(),(uint16_t*)my_lcd.getPointer());
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED)
    {
        delay(500);
        Serial.print(".");
    }
    my_lcd.fillRect(10, 55, my_lcd.width()-1, 25,TFT_WHITE);
    my_lcd.setTextColor(TFT_GREEN);
    my_lcd.drawString("Connection WIFI successful!!!", 10,55,2);
    my_lcd.setTextColor(TFT_BLUE);
    sprintf(t_buf, "SSID : %s", ssid);
    my_lcd.drawString(t_buf, 10,75,2);
    sprintf(t_buf, "  IP : %s", WiFi.localIP().toString().c_str());
    my_lcd.drawString(t_buf, 10,95,2);
    sprintf(t_buf, " MAC : %s", WiFi.macAddress().c_str());
    my_lcd.drawString(t_buf, 10,115,2);
    sprintf(t_buf, " PORT : %d", port);
    my_lcd.drawString(t_buf, 10,135,2);
    my_lcd.setTextColor(TFT_RED);
    server.begin(); //Start server 
    my_lcd.drawString("Waiting for client connection", 10,155,2);
    mylcd.Fill_Colors(0, 0, mylcd.Get_Width(), mylcd.Get_Height(),(uint16_t*)my_lcd.getPointer());
}

void loop()
{
    WiFiClient client = server.available(); // 等待客户端连接
    if (client) // 当客户端连接时
    {
        my_lcd.fillRect(10, 155, my_lcd.width()-1, 25, TFT_WHITE);
        my_lcd.drawString("Client connected.", 10, 155, 2);
        mylcd.Fill_Colors(0, 0, mylcd.Get_Width(), mylcd.Get_Height(), (uint16_t*)my_lcd.getPointer());

        String readBuff = ""; // 存储接收的数据
        
        while (client.connected()) // 客户端保持连接时循环
        {
            if (client.available() > 0) // 有数据可读取时
            {
                // 读取数据
                while (client.available() > 0)
                {
                    char c = client.read(); // 逐个读取字符
                    Serial.write(c); // 串口打印
                    readBuff += c;   // 拼接数据
                }
                
                // 发送回应给客户端
                client.print("Received: " + readBuff);
                
                // 更新屏幕显示
                my_lcd.fillRect(10, 175, my_lcd.width()-1, 40, TFT_WHITE); // 加大清除区域防止文字重叠
                my_lcd.drawString("Received: " + readBuff, 10, 175, 2);
                mylcd.Fill_Colors(0, 0, mylcd.Get_Width(), mylcd.Get_Height(), (uint16_t*)my_lcd.getPointer());
                
                // 清空缓冲区，准备接收下一批数据
                readBuff = "";
            }
            delay(10); // 短暂延迟，避免CPU占用过高
        }
        
        // 客户端断开连接
        client.stop();
        my_lcd.fillRect(10, 155, my_lcd.width()-1, 25, TFT_WHITE);
        my_lcd.drawString("Client Disconnected.", 10, 155, 2);
        mylcd.Fill_Colors(0, 0, mylcd.Get_Width(), mylcd.Get_Height(), (uint16_t*)my_lcd.getPointer());
    }
}
