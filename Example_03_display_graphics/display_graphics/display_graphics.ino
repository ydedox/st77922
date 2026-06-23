//This program is a demo of how to display picture and 
//how to use rotate function to display string.

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
#include <SPI.h>
#include "ST77922.h"

#define CENTER 240


TFT_eSPI tft = TFT_eSPI();
TFT_eSprite my_lcd = TFT_eSprite(&tft);
ST77922 mylcd = ST77922(); 

//define some colour values
#define BLACK   0x0000
#define BLUE    0x001F
#define RED     0xF800
#define GREEN   0x07E0
#define CYAN    0x07FF
#define MAGENTA 0xF81F
#define YELLOW  0xFFE0
#define WHITE   0xFFFF

//clear screen
void fill_screen_test()
{
  my_lcd.fillSprite(WHITE);
  mylcd.Fill_Colors(0, 0, mylcd.Get_Width(), mylcd.Get_Height(), (uint16_t *)my_lcd.getPointer()); 
  delay(500);  
  my_lcd.fillSprite(RED);
  mylcd.Fill_Colors(0, 0, mylcd.Get_Width(), mylcd.Get_Height(), (uint16_t *)my_lcd.getPointer());
  delay(500); 
  my_lcd.fillSprite(GREEN);
  mylcd.Fill_Colors(0, 0, mylcd.Get_Width(), mylcd.Get_Height(), (uint16_t *)my_lcd.getPointer());
  delay(500);
  my_lcd.fillSprite(BLUE);
  mylcd.Fill_Colors(0, 0, mylcd.Get_Width(), mylcd.Get_Height(), (uint16_t *)my_lcd.getPointer());
  delay(500); 
  my_lcd.fillSprite(BLACK);
  mylcd.Fill_Colors(0, 0, mylcd.Get_Width(), mylcd.Get_Height(), (uint16_t *)my_lcd.getPointer());
  delay(500); 
}

//display some strings
void text_test()
{
  my_lcd.fillSprite(BLACK); 
  my_lcd.setTextColor(WHITE);
  my_lcd.drawString("Hello World!", 2,2,1);
  
  my_lcd.setTextColor(YELLOW);
  my_lcd.drawFloat(1234.56, 3, 2, 10,2);

  my_lcd.setTextColor(RED);
  my_lcd.drawNumber(0xDEADBEF, 0, 24,4);

  my_lcd.setTextColor(BLUE);
  my_lcd.drawString("apmp", 2,46,6);
  my_lcd.setTextColor(GREEN);
  my_lcd.drawString("I implore thee,", 2,96,2);

  my_lcd.drawString("my foonting turlingdromes.", 2, 112,1);
  my_lcd.drawString("And hooptiously drangle me", 2, 120,1);
  my_lcd.drawString("with crinkly bindlewurdles,", 2, 128,1);
  my_lcd.drawString("Or I will rend thee", 2, 136,1);
  my_lcd.drawString("in the gobberwarts", 2, 144,1);
  my_lcd.drawString("with my blurglecruncheon,", 2, 152,1);
  my_lcd.drawString("see if I don't!", 2, 160,1);
  mylcd.Fill_Colors(0, 0, mylcd.Get_Width(), mylcd.Get_Height(), (uint16_t *)my_lcd.getPointer());
}

//draw some oblique lines
void lines_test(void)
{
    int i=0;
    my_lcd.fillSprite(BLACK); 
    for(i = 0; i< my_lcd.width();i+=5)
    {
       my_lcd.drawLine(0, 0, i, my_lcd.height()-1,GREEN);
     }
     for(i = my_lcd.height()-1; i>= 0;i-=5)
     {
       my_lcd.drawLine(0, 0, my_lcd.width()-1, i,GREEN);
     }
     mylcd.Fill_Colors(0, 0, mylcd.Get_Width(), mylcd.Get_Height(),(uint16_t*)my_lcd.getPointer());

     my_lcd.fillSprite(BLACK); 
    for(i = my_lcd.width() -1; i>=0;i-=5)
    {
      my_lcd.drawLine(my_lcd.width()-1, 0, i, my_lcd.height()-1,RED);
     }
    for(i = my_lcd.height()-1; i>=0;i-=5)
    {
      my_lcd.drawLine(my_lcd.width()-1, 0, 0, i,RED);
     }
     mylcd.Fill_Colors(0, 0, mylcd.Get_Width(), mylcd.Get_Height(), (uint16_t*)my_lcd.getPointer());

     my_lcd.fillSprite(BLACK); 
     for(i = 0; i < my_lcd.width();i+=5)
    {
      my_lcd.drawLine(0, my_lcd.height()-1, i, 0,GREEN);
     }
     for(i = 0; i < my_lcd.height();i+=5)
    {
      my_lcd.drawLine(0, my_lcd.height()-1, my_lcd.width()-1, i,GREEN);
     }
mylcd.Fill_Colors(0, 0, mylcd.Get_Width(), mylcd.Get_Height(), (uint16_t*)my_lcd.getPointer());

      my_lcd.fillSprite(BLACK); 
     for(i = my_lcd.width()-1; i >=0;i-=5)
    {
      my_lcd.drawLine(my_lcd.width()-1, my_lcd.height()-1, i, 0,YELLOW);
     }
     for(i = 0; i<my_lcd.height();i+=5)
    {
      my_lcd.drawLine(my_lcd.width()-1, my_lcd.height()-1, 0, i,YELLOW);
      
     }
     mylcd.Fill_Colors(0, 0, mylcd.Get_Width(), mylcd.Get_Height(), (uint16_t*)my_lcd.getPointer());
}

//draw some vertical lines and horizontal lines
void h_l_lines_test(void)
{
    int i=0;
    my_lcd.fillSprite(BLACK);
    for(i =0;i<my_lcd.height();i+=5)
    {
      my_lcd.drawFastHLine(0,i,my_lcd.width(),GREEN);
      delay(1);
    }
    mylcd.Fill_Colors(0, 0, mylcd.Get_Width(), mylcd.Get_Height(), (uint16_t*)my_lcd.getPointer());
     for(i =0;i<my_lcd.width();i+=5)
    {
      my_lcd.drawFastVLine(i,0,my_lcd.height(),BLUE);
      delay(1);
    }
    mylcd.Fill_Colors(0, 0, mylcd.Get_Width(), mylcd.Get_Height(), (uint16_t*)my_lcd.getPointer());
}

//draw some rectangles
void rectangle_test(void)
{
  int i = 0;
   my_lcd.fillSprite(BLACK);
   for(i = 0;i<my_lcd.width()/2;i+=4)
   {
      my_lcd.drawRect(i,(my_lcd.height()-my_lcd.width())/2+i,my_lcd.width()-2*i,my_lcd.width()-2*i,GREEN); 
      delay(1);
   }
    mylcd.Fill_Colors(0, 0, mylcd.Get_Width(), mylcd.Get_Height(),(uint16_t*)my_lcd.getPointer());
}

//draw some filled rectangles
void fill_rectangle_test(void)
{
  int i = 0;
   my_lcd.fillSprite(BLACK);
   my_lcd.fillRect(0,(my_lcd.height()-my_lcd.width())/2,my_lcd.width(),my_lcd.width(),YELLOW);
   for(i = 0;i<my_lcd.width()/2;i+=4)
   {
      my_lcd.drawRect(i,(my_lcd.height()-my_lcd.width())/2+i,my_lcd.width()-2*i,my_lcd.width()-2*i,MAGENTA);
      delay(1);
   }
   mylcd.Fill_Colors(0, 0, mylcd.Get_Width(), mylcd.Get_Height(),(uint16_t*)my_lcd.getPointer());
   for(i = 0;i<my_lcd.width()/2;i+=4)
   {
      my_lcd.fillRect(i,(my_lcd.height()-my_lcd.width())/2+i,my_lcd.width()-2*i,my_lcd.width()-2*i,random(0xFFFF));  
      delay(1);
   }
   mylcd.Fill_Colors(0, 0, mylcd.Get_Width(), mylcd.Get_Height(),(uint16_t*)my_lcd.getPointer());
}

//draw some filled circles
void fill_circles_test(void)
{
  int r=10,i=0,j=0;
  my_lcd.fillSprite(BLACK);
  for(i=r;i<my_lcd.width();i+=2*r)
  {
    for(j=r;j<my_lcd.height();j+=2*r)
    {
      my_lcd.fillCircle(i, j, r,MAGENTA);
    }
    mylcd.Fill_Colors(0, 0, mylcd.Get_Width(), mylcd.Get_Height(),(uint16_t*)my_lcd.getPointer());
  }
}

//draw some circles
void circles_test(void)
{
  int r=10,i=0,j=0;
  for(i=0;i<my_lcd.width()+r;i+=2*r)
  {
    for(j=0;j<my_lcd.height()+r;j+=2*r)
    {
      my_lcd.drawCircle(i, j, r,GREEN);
      
    }
    mylcd.Fill_Colors(0, 0, mylcd.Get_Width(), mylcd.Get_Height(),(uint16_t*)my_lcd.getPointer());
  }  
}

//draw some triangles
void triangles_test(void)
{
   int i = 0;
   my_lcd.fillSprite(BLACK);
   for(i=0;i<my_lcd.width()/2;i+=5)
   {
      my_lcd.drawTriangle(my_lcd.width()/2-1,my_lcd.height()/2-1-i,
                    my_lcd.width()/2-1-i,my_lcd.height()/2-1+i,
                    my_lcd.width()/2-1+i,my_lcd.height()/2-1+i,my_lcd.color565(0, i+64, i+64));  
                       
   }
   mylcd.Fill_Colors(0, 0, mylcd.Get_Width(), mylcd.Get_Height(),(uint16_t*)my_lcd.getPointer());
}

//draw some filled triangles
void fill_triangles_test(void)
{
   int i = 0;
   my_lcd.fillSprite(BLACK);
    for(i=my_lcd.width()/2-1;i>0;i-=5)
   {
      my_lcd.fillTriangle(my_lcd.width()/2-1,my_lcd.height()/2-1-i,
                    my_lcd.width()/2-1-i,my_lcd.height()/2-1+i,
                    my_lcd.width()/2-1+i,my_lcd.height()/2-1+i,my_lcd.color565(0, i+64, i+64));                   
      my_lcd.fillTriangle(my_lcd.width()/2-1,my_lcd.height()/2-1-i,
                    my_lcd.width()/2-1-i,my_lcd.height()/2-1+i,
                    my_lcd.width()/2-1+i,my_lcd.height()/2-1+i,my_lcd.color565(i, 0, i));  
                       
   }
   mylcd.Fill_Colors(0, 0, mylcd.Get_Width(), mylcd.Get_Height(),(uint16_t*)my_lcd.getPointer());
}

//draw some round rectangles
void round_rectangle(void)
{
   int i = 0;
   my_lcd.fillSprite(BLACK);
     for(i = 0;i<my_lcd.width()/2;i+=4)
   {
      my_lcd.fillRoundRect(i,(my_lcd.height()-my_lcd.width())/2+i,my_lcd.width()-2*i,my_lcd.width()-2*i,8,my_lcd.color565(255-i,0,160-i));
      delay(1);
   } 
   mylcd.Fill_Colors(0, 0, mylcd.Get_Width(), mylcd.Get_Height(),(uint16_t*)my_lcd.getPointer());
}

//draw some filled round rectangles
void fill_round_rectangle(void)
{
   int i = 0;
   my_lcd.fillSprite(BLACK);
   for(i = 0;i<my_lcd.width()/2;i+=4)
   {
      my_lcd.fillRoundRect(i,(my_lcd.height()-my_lcd.width())/2+i,my_lcd.width()-2*i,my_lcd.width()-2*i,8,my_lcd.color565(255-i,160-i,0));
        delay(1);
   }
    mylcd.Fill_Colors(0, 0, mylcd.Get_Width(), mylcd.Get_Height(),(uint16_t*)my_lcd.getPointer());
}

void setup() 
{
  mylcd.Init();
  mylcd.Set_Rotation(1);
  my_lcd.createSprite(mylcd.Get_Width(), mylcd.Get_Height());
  my_lcd.setSwapBytes(1);
}

void loop() 
{
  my_lcd.setRotation(0);
  fill_screen_test();
  delay(500);
  text_test();
  delay(500);
  lines_test();
  delay(500);
  h_l_lines_test();
  delay(500);
  rectangle_test();
  delay(500);
  fill_rectangle_test();
  delay(500);
  fill_circles_test();
  delay(500);
  circles_test();
  delay(500);
  triangles_test();
  delay(500);
  fill_triangles_test();
  delay(500);
  round_rectangle();
  delay(500);
  fill_round_rectangle();
  delay(500);
 for(uint8_t rotation=0; rotation<4; rotation++) 
 {
    my_lcd.setRotation(rotation);
    text_test();
    delay(1500);
 }
}
