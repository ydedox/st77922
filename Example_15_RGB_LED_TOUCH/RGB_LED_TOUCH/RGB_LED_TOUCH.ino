#include <Arduino.h>
#include "TFT_eSPI.h" 
#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
 #include <avr/power.h> // Required for 16 MHz Adafruit Trinket
#endif
#include "ST77922.h"
#include "ST77922_Touch.h"  // 替换为ST77922触摸库

#define LED_PIN    40 
#define LED_COUNT  60 
#define BRIGHTNESS 50 

int currentColorIndex[3] = {0, 0, 0};

Adafruit_NeoPixel strip(LED_COUNT, LED_PIN, NEO_GRB + NEO_KHZ800);

const uint32_t colors[] = {
  strip.Color(255, 0, 0), 
  strip.Color(0, 255, 0), 
  strip.Color(0, 0, 255)  
};

#define NUM_KEYS 3

#define KEY_X 75 
#define KEY_Y 65
#define KEY_W 65 
#define KEY_H 35
#define KEY_SPACING_X 20 
#define KEY_SPACING_Y 1
#define KEY_TEXTSIZE 1  

uint16_t k_color[NUM_KEYS] = {TFT_RED, TFT_GREEN, TFT_BLUE};

TFT_eSPI my_lcd = TFT_eSPI();
TFT_eSprite tft = TFT_eSprite(&my_lcd);
ST77922 mylcd = ST77922();

TFT_eSPI_Button key_b[NUM_KEYS];


// 新增：ST77922触摸对象及状态变量
ST77922_TOUCH my_touch;  // 触摸控制器对象
uint16_t last_touch_x = 0xFFFF;  // 记录上一次触摸X坐标
uint16_t last_touch_y = 0xFFFF;  // 记录上一次触摸Y坐标

void initNeoPixel() {
  strip.begin();           
  strip.show();            
  strip.setBrightness(BRIGHTNESS);
}

void setNeoPixelColor(uint32_t color) {
  for (int i = 0; i < strip.numPixels(); i++) {
    strip.setPixelColor(i, color);
  }
  strip.show();
}

void neoPixelFlick(uint8_t colorIndex) {
  for (int i = 0; i < 6; i++) {
    setNeoPixelColor(0);
    delay(500);
    setNeoPixelColor(colors[colorIndex]);
    delay(500);
  }
}

void setup() {
  #if defined(__AVR_ATtiny85__) && (F_CPU == 16000000)
  clock_prescale_set(clock_div_1);
#endif
  mylcd.Init();
  mylcd.Set_Rotation(0);
  tft.createSprite(mylcd.Get_Width(), mylcd.Get_Height());
  tft.setSwapBytes(1);
  
  // 初始化触摸控制器（匹配屏幕旋转）
  my_touch.init();
  my_touch.Set_Rotation(0);  // 关键：与屏幕旋转角度一致
  
  tft.fillSprite(TFT_BLACK);
  initNeoPixel();

  tft.setCursor(5, 25);
  tft.setTextFont(2);
  tft.setTextSize(1);
  tft.setTextColor(TFT_WHITE, TFT_BLACK);

  tft.setTextSize(2);
  tft.setCursor(tft.width()/2 - 150, 195);
  tft.println("0");
  tft.setCursor(tft.width()/2 + 110, 195);
  tft.println("255");
  tft.fillRoundRect(tft.width()/2 - 138, 165, 276, 21, 10, 0x8430);
  tft.fillCircle(tft.width()/2 - 128, 175, 10, TFT_MAGENTA);

  tft.setTextColor(0xFFE0);
  tft.drawNumber(0, tft.width()/2 - 15, 120, 1);
  tft.setFreeFont(&FreeSansBold9pt7b);

  // 按钮初始化不变
  for (int i = 0; i < NUM_KEYS; i++) {
    key_b[i].initButton(&tft,
                        KEY_X + i * (KEY_W + KEY_SPACING_X),
                        KEY_Y + 0 * (KEY_H + KEY_SPACING_Y),
                        KEY_W, KEY_H,
                        TFT_WHITE, k_color[i], TFT_WHITE,
                        "", KEY_TEXTSIZE);
    key_b[i].setLabelDatum(13 - (KEY_W/2), 2, ML_DATUM);
    key_b[i].drawButton(false, "OFF");
  }
  mylcd.Fill_Colors(0, 0, mylcd.Get_Width(), mylcd.Get_Height(), (uint16_t *)tft.getPointer());
}

void loop() {
  uint8_t i = 0;
  int16_t t_x = 0, t_y = 0;  // 当前触摸坐标

  // 检测触摸事件（关键修改点）
  if (my_touch.Get_Touch()) {  
    t_x = my_touch.touch.x[0];  // 取第一个触摸点（单点模式）
    t_y = my_touch.touch.y[0];
    
    // 更新按钮状态
    for (i = 0; i < NUM_KEYS; i++) {
      key_b[i].press(key_b[i].contains(t_x, t_y));
    }

    // 处理亮度调节滑动条
    if (t_y > 164 && t_y < 184) {
      int brightness = constrain(t_x - (tft.width()/2 - 128), 0, 255);
      strip.setBrightness(brightness);
      strip.show();

      // 刷新滑动条显示
      //tft.fillCircle(last_touch_x, 175, 10, 0x8430);  // 清除旧滑块
      tft.fillRoundRect(tft.width()/2 - 138, 165, 276, 21, 10, 0x8430);
      tft.fillCircle(t_x, 175, 10, TFT_MAGENTA);        // 绘制新滑块
      mylcd.Fill_Colors(0, 0, mylcd.Get_Width(), mylcd.Get_Height(), (uint16_t *)tft.getPointer());
      //刷新数据
      tft.fillRoundRect(tft.width()/2 - 15, 120, 276, 21, 10, TFT_BLACK);
      mylcd.Fill_Colors(0, 0, mylcd.Get_Width(), mylcd.Get_Height(), (uint16_t *)tft.getPointer());
      tft.drawNumber(brightness, tft.width()/2 - 15, 120, 1);
      //tft.print(brightness);
      mylcd.Fill_Colors(0, 0, mylcd.Get_Width(), mylcd.Get_Height(), (uint16_t *)tft.getPointer());
      last_touch_x = t_x;  // 记录当前位置
    }
  } else {
    // 无触摸时重置按钮状态
    for (i = 0; i < NUM_KEYS; i++) {
      key_b[i].press(false);
    }
    last_touch_x = 0xFFFF;  // 标记为无效坐标
  }

  // 按钮状态处理（保持不变）
  for (i = 0; i < NUM_KEYS; i++) {
    if (key_b[i].justPressed()) {
      key_b[i].drawButton(true, currentColorIndex[i] ? "ON" : "OFF");
    }
    if (key_b[i].justReleased()) {
      switch (currentColorIndex[i]) {
        case 0:
          key_b[i].drawButton(false, "ON");
          mylcd.Fill_Colors(0, 0, mylcd.Get_Width(), mylcd.Get_Height(), (uint16_t *)tft.getPointer());
          setNeoPixelColor(colors[i]);
          currentColorIndex[i] = 1;
          break;
        case 1:
          key_b[i].drawButton(false, "FLI");
          mylcd.Fill_Colors(0, 0, mylcd.Get_Width(), mylcd.Get_Height(), (uint16_t *)tft.getPointer());
          neoPixelFlick(i);
          currentColorIndex[i] = 2;
          break;
        case 2:
          key_b[i].drawButton(false, "OFF");
          mylcd.Fill_Colors(0, 0, mylcd.Get_Width(), mylcd.Get_Height(), (uint16_t *)tft.getPointer());
          setNeoPixelColor(0);
          currentColorIndex[i] = 0;
          break;
      }
    }
  }
}