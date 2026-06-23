//How to use a timer to control LED lights in this program
//pin usage as follow:
//                   LED_STRIP  VCC    GND    
//ESP32-WROOM-32E:   42        5V     GND  

// Low level  -- turn on LED light (for NeoPixel, it's controlled by the library functions)
// High level -- turn off LED light (for NeoPixel, it's controlled by the library functions) 

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

#include <Adafruit_NeoPixel.h>
#include <Arduino.h>

// Which pin on the Arduino is connected to the NeoPixels?
#define LED_PIN    40

// How many NeoPixels are attached to the Arduino?
#define LED_COUNT  60

// NeoPixel brightness, 0 (min) to 255 (max)
#define BRIGHTNESS 50 // Set BRIGHTNESS to about 1/5 (max = 255)

Adafruit_NeoPixel strip(LED_COUNT, LED_PIN, NEO_GRBW + NEO_KHZ800);


hw_timer_t * timer = NULL;

// Declare our NeoPixel strip object:

// Argument 1 = Number of pixels in NeoPixel strip
// Argument 2 = Arduino pin number (most are valid)
// Argument 3 = Pixel type flags, add together as needed:
//   NEO_KHZ800  800 KHz bitstream (most NeoPixel products w/WS2812 LEDs)
//   NEO_KHZ400  400 KHz (classic 'v1' (not v2) FLORA pixels, WS2811 drivers)
//   NEO_GRB     Pixels are wired for GRB bitstream (most NeoPixel products)
//   NEO_RGB     Pixels are wired for RGB bitstream (v1 FLORA pixels, not v2)
//   NEO_RGBW    Pixels are wired for RGBW bitstream (NeoPixel RGBW products)

void IRAM_ATTR onTimer() {

colorWipe(strip.Color(0,255,  0), 5); // 绿色

}

void setup() {
#if defined(__AVR_ATtiny85__) && (F_CPU == 16000000)
  clock_prescale_set(clock_div_1);
#endif

  Serial.begin(115200);
  strip.begin();
  strip.show();
  strip.setBrightness(BRIGHTNESS);

  /*  1/(80MHZ/80) = 1us  */
  timer = timerBegin(1000000);

  /* Attach the onTimer function to our timer */
  timerAttachInterrupt(timer, &onTimer);

  /* *Set the alarm clock to call the onTimer function every second 1 tick is 1us => 1 second is 1000000us * / 
  / *Repeat alarm (third parameter)*/
  timerAlarm(timer, 1000000, true, 0);

  Serial.println("start timer");
}

void loop() {
  delay(10);
}

// Fill strip pixels one after another with a color. 
// Modified to use the existing colorWipe function from original code
void colorWipe(uint32_t color, int wait) {
  for(int i=0; i<strip.numPixels(); i++) { // For each pixel in strip...
    strip.setPixelColor(i, color);         //  Set pixel's color (in RAM)
    strip.show();                          //  Update strip to match
    delay(wait);                           //  Pause for a moment
  }
}