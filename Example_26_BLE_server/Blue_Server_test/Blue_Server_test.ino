//This example is to create a Bluetooth cable server and then communicate with a mobile app

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
#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLEUtils.h>
#include <BLE2902.h>
#include "ST77922.h"

#define SERVICE_UUID             "DFCD0001-36E1-4688-B7F5-EA07361B26A8"
#define CHARACTERISTIC1_UUID     "DFCD000A-36E1-4688-B7F5-EA07361B26A8"
bool deviceConnected = false;
BLEServer *pServer;
BLEService *pService;
BLECharacteristic* pCharacteristic;
int i = 0;


TFT_eSPI tft = TFT_eSPI();
TFT_eSprite my_lcd = TFT_eSprite(&tft);
ST77922 mylcd = ST77922();  

class MyServerCallbacks: public BLEServerCallbacks 
{
    void onConnect(BLEServer* pServer) 
    {
      my_lcd.fillRect(10, 0, my_lcd.width()-1, 20,TFT_WHITE);
      my_lcd.drawString("BLE device connected.", 10, 0); 
      mylcd.Fill_Colors(0, 0, mylcd.Get_Width(), mylcd.Get_Height(),(uint16_t*)my_lcd.getPointer());
      deviceConnected = true;
    };
    void onDisconnect(BLEServer* pServer) 
    {
      my_lcd.fillRect(10, 0, my_lcd.width()-1, 20,TFT_WHITE);
      my_lcd.drawString("BLE device disconnected.", 10, 0);
      mylcd.Fill_Colors(0, 0, mylcd.Get_Width(), mylcd.Get_Height(),(uint16_t*)my_lcd.getPointer());
      deviceConnected = false;
    }
};

class MyCallbacks: public BLECharacteristicCallbacks 
{
    void onWrite(BLECharacteristic *pCharacteristic) 
    {
      String value = pCharacteristic->getValue();
      if (value.length() > 0) 
      {
        if(i>27)
        {
          i = 0;
          my_lcd.fillRect(0, 32, my_lcd.width()-1, my_lcd.height()-32,TFT_WHITE); 
        }
        my_lcd.drawString(value.c_str(),5,32+16*i);
        mylcd.Fill_Colors(0, 0, mylcd.Get_Width(), mylcd.Get_Height(),(uint16_t*)my_lcd.getPointer());
        Serial.println(value.c_str());
        pCharacteristic->notify();
        i++;
      }
    } 
};
void setupBLE()
{
  BLEDevice::init("ESP32_BT_BLE");   //Create BLE device
  pServer = BLEDevice::createServer();   //Create BLE server
  pServer->setCallbacks(new MyServerCallbacks());   //Set the callback function for the server
  pService = pServer->createService(SERVICE_UUID); //Create BLE service
  pCharacteristic = pService->createCharacteristic(
                                                 CHARACTERISTIC1_UUID,
                                                 BLECharacteristic::PROPERTY_READ|
                                                 BLECharacteristic::PROPERTY_NOTIFY|
                                                 BLECharacteristic::PROPERTY_WRITE);   //Create characteristic values for services
  pCharacteristic->setCallbacks(new MyCallbacks());    //Callback function for setting eigenvalues
  pCharacteristic->addDescriptor(new BLE2902());
  pCharacteristic->setValue("ESP32 BT BLE");
  pService->start();
  BLEAdvertising *pAdvertising = pServer->getAdvertising();
  pAdvertising->start();
}
void setup() 
{
  Serial.begin(115200);
  mylcd.Init();
  mylcd.Set_Rotation(0);
  my_lcd.createSprite(mylcd.Get_Width(), mylcd.Get_Height());
  my_lcd.setSwapBytes(0);
  my_lcd.fillSprite(TFT_WHITE);
  my_lcd.setTextFont(2);
  my_lcd.setTextColor(TFT_RED);
  setupBLE();
  my_lcd.drawString("BLE server start.", 10, 0); 
  my_lcd.drawString("receive data :",10,16,2);
  mylcd.Fill_Colors(0, 0, mylcd.Get_Width(), mylcd.Get_Height(),(uint16_t*)my_lcd.getPointer());
}

void loop() 
{
   delay(3000);
   
}
