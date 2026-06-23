#include <TFT_eSPI.h> 
#include <SPI.h>
#include "ST77922.h"       // ST77922显示驱动
#include "ST77922_Touch.h" // ST77922触摸驱动

// 初始化显示和触摸对象
TFT_eSPI tft = TFT_eSPI();
TFT_eSprite my_lcd = TFT_eSprite(&tft);
ST77922 mylcd = ST77922();
ST77922_TOUCH my_touch = ST77922_TOUCH();

// 屏幕分辨率（320*480）
#define SCREEN_WIDTH  320
#define SCREEN_HEIGHT 480

// 颜色定义
#define BLACK        0x0000  
#define BLUE         0x001F  
#define RED          0xF800  
#define GREEN        0x07E0  
#define CYAN         0x07FF  
#define MAGENTA      0xF81F  
#define YELLOW       0xFFE0  
#define WHITE        0xFFFF  
#define LIGHTGREY    0xC618  
#define DARKGREY     0x7BEF  
#define GREENYELLOW  0xAFE5  
#define PINK         0xF81F  

// 调整UI布局参数（适配320*480）
#define BUTTON_R 35          // 按键半径（缩小为30，避免过大）
#define BUTTON_SPACING_X 30  // 水平间距（减小间距）
#define BUTTON_SPACING_Y 8   // 垂直间距（减小间距）
#define EDG_Y 20             // 底部边距（增大，避免超出屏幕）
#define EDG_X 25             // 左右边距

// 按键信息结构体
typedef struct _button_info
{
    const char button_name[10];
    uint8_t button_name_size;
    uint16_t button_name_colour;
    uint16_t button_colour;
    int16_t button_x;
    int16_t button_y;     
} button_info;

// 按键布局定义（基于320*480重新计算y坐标）
button_info phone_button[15] = 
{
    // 第1行（最上方数字键）
    "1",4,BLACK,CYAN,
    EDG_X+BUTTON_R,
    (int16_t)(SCREEN_HEIGHT - EDG_Y - 4*BUTTON_SPACING_Y - 9*BUTTON_R),  // 基于480高度计算
    
    "2",4,BLACK,CYAN,
    EDG_X + 3*BUTTON_R + BUTTON_SPACING_X,
    (int16_t)(SCREEN_HEIGHT - EDG_Y - 4*BUTTON_SPACING_Y - 9*BUTTON_R),
    
    "3",4,BLACK,CYAN,
    EDG_X + 5*BUTTON_R + 2*BUTTON_SPACING_X,
    (int16_t)(SCREEN_HEIGHT - EDG_Y - 4*BUTTON_SPACING_Y - 9*BUTTON_R),

    // 第2行数字键
    "4",4,BLACK,CYAN,
    EDG_X+BUTTON_R,
    (int16_t)(SCREEN_HEIGHT - EDG_Y - 3*BUTTON_SPACING_Y - 7*BUTTON_R),
    
    "5",4,BLACK,CYAN,
    EDG_X + 3*BUTTON_R + BUTTON_SPACING_X,
    (int16_t)(SCREEN_HEIGHT - EDG_Y - 3*BUTTON_SPACING_Y - 7*BUTTON_R),
    
    "6",4,BLACK,CYAN,
    EDG_X + 5*BUTTON_R + 2*BUTTON_SPACING_X,
    (int16_t)(SCREEN_HEIGHT - EDG_Y - 3*BUTTON_SPACING_Y - 7*BUTTON_R),

    // 第3行数字键
    "7",4,BLACK,CYAN,
    EDG_X+BUTTON_R,
    (int16_t)(SCREEN_HEIGHT - EDG_Y - 2*BUTTON_SPACING_Y - 5*BUTTON_R),
    
    "8",4,BLACK,CYAN,
    EDG_X + 3*BUTTON_R + BUTTON_SPACING_X,
    (int16_t)(SCREEN_HEIGHT - EDG_Y - 2*BUTTON_SPACING_Y - 5*BUTTON_R),
    
    "9",4,BLACK,CYAN,
    EDG_X + 5*BUTTON_R + 2*BUTTON_SPACING_X,
    (int16_t)(SCREEN_HEIGHT - EDG_Y - 2*BUTTON_SPACING_Y - 5*BUTTON_R),

    // 第4行（*、0、#）
    "*",4,BLACK,PINK,
    EDG_X+BUTTON_R,
    (int16_t)(SCREEN_HEIGHT - EDG_Y - BUTTON_SPACING_Y - 3*BUTTON_R),
    
    "0",4,BLACK,CYAN,
    EDG_X + 3*BUTTON_R + BUTTON_SPACING_X,
    (int16_t)(SCREEN_HEIGHT - EDG_Y - BUTTON_SPACING_Y - 3*BUTTON_R),
    
    "#",4,BLACK,PINK,
    EDG_X + 5*BUTTON_R + 2*BUTTON_SPACING_X,
    (int16_t)(SCREEN_HEIGHT - EDG_Y - BUTTON_SPACING_Y - 3*BUTTON_R),

    // 第5行（功能键）
    "end",2,BLACK,RED,
    EDG_X+BUTTON_R,
    (int16_t)(SCREEN_HEIGHT - EDG_Y/2 - BUTTON_R),  // 最底部，靠近屏幕边缘
    
    "call",2,BLACK,GREEN,
    EDG_X + 3*BUTTON_R + BUTTON_SPACING_X,
    (int16_t)(SCREEN_HEIGHT - EDG_Y/2 - BUTTON_R),
    
    "dele",2,BLACK,LIGHTGREY,
    EDG_X + 5*BUTTON_R + 2*BUTTON_SPACING_X,
    (int16_t)(SCREEN_HEIGHT - EDG_Y/2 - BUTTON_R),
};

// 全局变量
uint16_t px = 0, py = 0;
uint16_t text_x = 20, text_y = 30;  // 数字输入区域坐标（适配320宽度）
uint16_t text_x_add = 25;  // 数字输入间距（缩小，适配宽度）
uint16_t text_y_add = 30;
uint16_t n = 0;

// 检查触摸是否在按键区域内
boolean is_pressed(int16_t x1, int16_t y1, int16_t x2, int16_t y2, int16_t px, int16_t py)
{
    return (px > x1 && px < x2 && py > y1 && py < y2);
}

// 绘制所有UI元素
void show_menu(void)
{
    // 绘制背景
    my_lcd.fillSprite(BLUE);
    
    // 绘制按键
    for(uint8_t i = 0; i < sizeof(phone_button)/sizeof(button_info); i++)
    {
        // 绘制按键圆形
        my_lcd.fillCircle(phone_button[i].button_x, phone_button[i].button_y, 
                         BUTTON_R, phone_button[i].button_colour);
        // 绘制按键文字
        my_lcd.setTextColor(phone_button[i].button_name_colour);
        my_lcd.drawString(phone_button[i].button_name,
                         phone_button[i].button_x - (strlen(phone_button[i].button_name) * 
                         phone_button[i].button_name_size * 4) / 2,  // 文字水平居中
                         phone_button[i].button_y - (phone_button[i].button_name_size * 6) / 2,  // 文字垂直居中
                         phone_button[i].button_name_size);
    }
    
    // 绘制顶部输入框边框
    my_lcd.drawRect(10, 10, SCREEN_WIDTH-20, 40, BLACK);  // 适配320宽度
    my_lcd.drawRect(11, 11, SCREEN_WIDTH-22, 38, WHITE);
    
    // 刷新到屏幕
    my_lcd.pushSprite(0, 0);
    mylcd.Fill_Colors(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, (uint16_t *)my_lcd.getPointer());
}

// 初始化函数
void setup(void) 
{
    // 初始化显示屏（320*480）
    mylcd.Init();
    mylcd.Set_Rotation(0);  // 旋转方向与触摸匹配
    
    // 初始化触摸（适配320*480）
    my_touch.init();
    my_touch.Set_Rotation(0);  // 触摸旋转与屏幕一致
    
    // 初始化Sprite（匹配屏幕分辨率）
    my_lcd.createSprite(SCREEN_WIDTH, SCREEN_HEIGHT);
    my_lcd.setSwapBytes(true);  // 颜色格式适配
    
    // 绘制初始界面
    show_menu();
}

// 主循环
void loop(void)
{
    // 检测触摸
    if(my_touch.Get_Touch())
    {
        px = my_touch.touch.x[0];
        py = my_touch.touch.y[0];
        
        // 触摸坐标限制在屏幕范围内
        if(px >= SCREEN_WIDTH) px = SCREEN_WIDTH - 1;
        if(py >= SCREEN_HEIGHT) py = SCREEN_HEIGHT - 1;
        
        // 检查按键触摸
        for(uint8_t i = 0; i < sizeof(phone_button)/sizeof(button_info); i++)
        {
            // 按键触摸区域：圆形外切矩形
            int16_t x1 = phone_button[i].button_x - BUTTON_R;
            int16_t y1 = phone_button[i].button_y - BUTTON_R;
            int16_t x2 = phone_button[i].button_x + BUTTON_R;
            int16_t y2 = phone_button[i].button_y + BUTTON_R;
            
            if(is_pressed(x1, y1, x2, y2, px, py))
            {
                // 按键按下反馈（变暗）
                my_lcd.fillCircle(phone_button[i].button_x, phone_button[i].button_y, 
                                 BUTTON_R, DARKGREY);
                my_lcd.setTextColor(WHITE);
                my_lcd.drawString(phone_button[i].button_name,
                                 phone_button[i].button_x - (strlen(phone_button[i].button_name) * 
                                 phone_button[i].button_name_size * 4) / 2,
                                 phone_button[i].button_y - (phone_button[i].button_name_size * 6) / 2,
                                 phone_button[i].button_name_size);
                my_lcd.pushSprite(0, 0);
                mylcd.Fill_Colors(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, (uint16_t *)my_lcd.getPointer());
                delay(150);  // 按键反馈延迟
                
                // 恢复按键状态
                my_lcd.fillCircle(phone_button[i].button_x, phone_button[i].button_y, 
                                 BUTTON_R, phone_button[i].button_colour);
                my_lcd.setTextColor(phone_button[i].button_name_colour);
                my_lcd.drawString(phone_button[i].button_name,
                                 phone_button[i].button_x - (strlen(phone_button[i].button_name) * 
                                 phone_button[i].button_name_size * 4) / 2,
                                 phone_button[i].button_y - (phone_button[i].button_name_size * 6) / 2,
                                 phone_button[i].button_name_size);
                
                // 处理按键功能
                if(i < 12)  // 数字和符号键
                {
                    if(n < 10)  // 限制输入长度（适配320宽度）
                    {
                        my_lcd.setTextColor(GREENYELLOW);
                        my_lcd.drawString(phone_button[i].button_name, text_x, text_y, 
                                         phone_button[i].button_name_size);
                        text_x += text_x_add;
                        n++;
                    }
                }
                else if(i == 12)  // 结束通话
                {
                    my_lcd.fillRect(20, 55, SCREEN_WIDTH-40, 20, BLUE);  // 清除原状态
                    my_lcd.setTextColor(RED);
                    my_lcd.drawString("Call ended", SCREEN_WIDTH/2 - 40, 55, 2);
                } 
                else if(i == 13)  // 拨打电话
                {
                    my_lcd.fillRect(20, 55, SCREEN_WIDTH-40, 20, BLUE);
                    my_lcd.setTextColor(GREEN);
                    my_lcd.drawString("Calling...", SCREEN_WIDTH/2 - 40, 55, 2);
                }
                else if(i == 14)  // 删除键
                {
                    if(n > 0)
                    {
                        text_x -= text_x_add;
                        my_lcd.fillRect(text_x, text_y, text_x_add-5, text_y_add, BLUE);  // 清除单个数字
                        n--;
                    }
                }
                
                // 刷新显示
                my_lcd.pushSprite(0, 0);
                mylcd.Fill_Colors(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, (uint16_t *)my_lcd.getPointer());
                
                // 等待触摸释放
                while(my_touch.Get_Touch()) delay(10);
            }      
        }
    }
}