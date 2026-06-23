#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
 #include <avr/power.h> // Required for 16 MHz Adafruit Trinket
#endif

// LED和按键引脚定义
#define LED_PIN    40
#define BUTTON_PIN  0   // 连接按键的引脚
#define LED_COUNT  60
#define BRIGHTNESS 50

Adafruit_NeoPixel strip(LED_COUNT, LED_PIN, NEO_GRBW + NEO_KHZ800);

// 定义颜色数组
enum Colors { RED, GREEN, BLUE, WHITE, NUM_COLORS };
uint32_t colors[NUM_COLORS] = {
  strip.Color(255, 0, 0),     // 红色
  strip.Color(0, 255, 0),     // 绿色
  strip.Color(0, 0, 255),     // 蓝色
  strip.Color(0, 0, 0, 255)   // 白色 (RGBW)
};


int currentColor = 0;         // 当前选中的颜色索引
bool lastButtonState = HIGH;  // 上次按键状态
bool currentButtonState = HIGH; // 当前按键状态
unsigned long lastDebounceTime = 0; // 上次检测到按键变化的时间
unsigned long debounceDelay = 2;   // 消抖延迟时间（毫秒）

void setup() {
#if defined(__AVR_ATtiny85__) && (F_CPU == 16000000)
  clock_prescale_set(clock_div_1);
#endif

  strip.begin();
  strip.show();
  strip.setBrightness(BRIGHTNESS);
  
  // 初始化按键引脚
  pinMode(BUTTON_PIN, INPUT_PULLUP); // 使用内部上拉电阻
  
  // 初始显示无色
  colorWipe(colors[3], 0);
}

void loop() {
  // 读取按键状态并进行消抖处理
  int reading = digitalRead(BUTTON_PIN);
  
  // 如果状态变化，重置去抖动计时器
  if (reading != lastButtonState) {
    lastDebounceTime = millis();
  }
  
  // 经过消抖时间后确认状态变化
  if ((millis() - lastDebounceTime) > debounceDelay) {
    if (reading != currentButtonState) {
      currentButtonState = reading;
      
      // 检测到按键按下（下降沿）
      if (currentButtonState == LOW) {
        // 切换到下一种颜色
        currentColor = (currentColor + 1) % NUM_COLORS;
        colorWipe(colors[currentColor-1], 0); // 显示新颜色，带过渡效果
      }
    }
  }
  
  lastButtonState = reading;
}

// Fill strip pixels one after another with a color. 
// Modified to use the existing colorWipe function from original code
void colorWipe(uint32_t color, int wait) {
  for(int i=0; i<strip.numPixels(); i++) { 
    strip.setPixelColor(i, color);         
    strip.show();                          
    delay(wait);                           
  }
}