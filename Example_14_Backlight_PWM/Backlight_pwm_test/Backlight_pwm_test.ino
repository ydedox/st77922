#include <TFT_eSPI.h> 
#include <SPI.h>
#include "ST77922.h"
#include "ST77922_Touch.h"  // 替换原有touch.h

#define BACKLIGHT_PIN 41
#define SLIDER_Y 140         // 滑块Y坐标中心
#define SLIDER_WIDTH 256     // 滑动条宽度
#define SLIDER_X_MIN -128    // 滑动条相对中心的X偏移范围
#define SLIDER_X_MAX 128


TFT_eSPI tft = TFT_eSPI();
TFT_eSprite my_lcd = TFT_eSprite(&tft);
ST77922 mylcd = ST77922();
ST77922_TOUCH my_touch = ST77922_TOUCH();// 定义ST77922触摸对象

// 存储上一次触摸点坐标（用于单点跟踪）
uint16_t last_touch_x = 0xFFFF;
uint16_t last_touch_y = 0xFFFF;

void setup()
{
    Serial.begin(115200);
    mylcd.Init();
    mylcd.Set_Rotation(1);
    my_lcd.createSprite(mylcd.Get_Width(), mylcd.Get_Height());
    my_lcd.setSwapBytes(1);
    
    // 初始化触摸控制器（匹配屏幕旋转）
    my_touch.init();
    my_touch.Set_Rotation(1);  // 关键：与屏幕旋转角度一致
    
    my_lcd.fillSprite(TFT_WHITE);
    my_lcd.setTextColor(TFT_BLACK, TFT_WHITE);
    my_lcd.setTextSize(2);
    
    // 绘制滑动条背景
    my_lcd.fillRoundRect(my_lcd.width()/2 - SLIDER_WIDTH/2, 
                        SLIDER_Y - 10, SLIDER_WIDTH, 21, 10, 0x07FF);
    
    // 绘制初始滑块位置（右侧最大值）
    update_slider_position(my_lcd.width()/2 + SLIDER_X_MAX);
    
    // 初始化背光
    ledcAttachChannel(BACKLIGHT_PIN, 2000, 8, 0);
    ledcWrite(BACKLIGHT_PIN, 255);
    mylcd.Fill_Colors(0, 0, mylcd.Get_Width(), mylcd.Get_Height(),(uint16_t*)my_lcd.getPointer());
}

// 更新滑块位置及显示
void update_slider_position(int16_t x_pos) {
    int16_t center_x = my_lcd.width()/2;
    
    // 限制滑块在有效范围内
    int16_t clamped_x = constrain(x_pos, 
                                center_x + SLIDER_X_MIN, 
                                center_x + SLIDER_X_MAX);
    
    // 计算亮度值（0-255）
    int brightness = map(clamped_x, 
                        center_x + SLIDER_X_MIN, 
                        center_x + SLIDER_X_MAX, 
                        0, 255);
    my_lcd.fillSprite(TFT_WHITE);
        my_lcd.fillRoundRect(my_lcd.width()/2 - SLIDER_WIDTH/2, 
                        SLIDER_Y - 10, SLIDER_WIDTH, 21, 10, 0x07FF);
    
    // 绘制新滑块
    my_lcd.fillCircle(clamped_x, SLIDER_Y, 10, TFT_MAGENTA);
    
    // 更新数值显示
    my_lcd.setTextColor(TFT_RED);
    my_lcd.setCursor(center_x - 15, 65);
    my_lcd.setCursor(center_x - 15, 65);
    my_lcd.print(brightness, DEC);
    
    // 更新背光亮度
    ledcWrite(BACKLIGHT_PIN, brightness);
    mylcd.Fill_Colors(0, 0, mylcd.Get_Width(), mylcd.Get_Height(),(uint16_t*)my_lcd.getPointer());
    // 记录当前位置
    last_touch_x = clamped_x;
    last_touch_y = SLIDER_Y;
}

void loop()
{
    if (my_touch.Get_Touch()) {  // 检测触摸事件
        // 处理第一个触摸点（单点模式）
        int16_t t_x = my_touch.touch.x[0];
        int16_t t_y = my_touch.touch.y[0];
        
        // 检查触摸点是否在滑动条区域内
        if (t_y > (SLIDER_Y - 10) && t_y < (SLIDER_Y + 10)) {
            update_slider_position(t_x);
        }
        mylcd.Fill_Colors(0, 0, mylcd.Get_Width(), mylcd.Get_Height(),(uint16_t*)my_lcd.getPointer());
    } else {
        // 无触摸时重置状态
        last_touch_x = 0xFFFF;
        last_touch_y = 0xFFFF;
    }
}