#include <TFT_eSPI.h> 
#include <SPI.h>
#include "ST77922.h"
#include "ST77922_Touch.h"

TFT_eSPI tft = TFT_eSPI();
TFT_eSprite my_lcd = TFT_eSprite(&tft);
ST77922 mylcd = ST77922();
ST77922_TOUCH my_touch = ST77922_TOUCH();

#define BLACK   0x0000
#define BLUE    0x001F
#define RED     0xF800
#define GREEN   0x07E0
#define CYAN    0x07FF
#define MAGENTA 0xF81F
#define YELLOW  0xFFE0
#define WHITE   0xFFFF

uint16_t lastX = 0xFFFF;
uint16_t lastY = 0xFFFF;

void LCD_Draw_Line(int16_t x1, int16_t y1, int16_t x2, int16_t y2, uint16_t Size,uint16_t colour)
{
  uint16_t t; 
  int xerr=0,yerr=0,delta_x,delta_y,distance; 
  int incx,incy,uRow,uCol; 
  if(x1<Size||x2<Size||y1<Size||y2<Size)
  {
    return;  
  }
  delta_x=x2-x1; //计算坐标增量 
  delta_y=y2-y1; 
  uRow=x1; 
  uCol=y1; 
  if(delta_x>0)incx=1; //设置单步方向 
  else if(delta_x==0)incx=0;//垂直线 
  else {incx=-1;delta_x=-delta_x;} 
  if(delta_y>0)incy=1; 
  else if(delta_y==0)incy=0;//水平线 
  else{incy=-1;delta_y=-delta_y;} 
  if( delta_x>delta_y)distance=delta_x; //选取基本增量坐标轴 
  else distance=delta_y; 
  for(t=0;t<=distance+1;t++ )//画线输出 
  {  
     my_lcd.fillCircle(uRow, uCol, Size,colour);
    xerr+=delta_x ; 
    yerr+=delta_y ; 
    if(xerr>distance) 
    { 
      xerr-=distance; 
      uRow+=incx; 
    } 
    if(yerr>distance) 
    { 
      yerr-=distance; 
      uCol+=incy; 
    } 
  }  
}

void setup(void) 
{
  Serial.begin(115200); //Set the serial port baud rate 115200
  mylcd.Init();
  mylcd.Set_Rotation(0);
  my_lcd.createSprite(mylcd.Get_Width(), mylcd.Get_Height());
  my_lcd.setSwapBytes(1);
  my_touch.init();
  my_touch.Set_Rotation(0);
  my_lcd.fillSprite(WHITE);
  my_lcd.setTextColor(BLUE);
  my_lcd.drawString("RST",my_lcd.width()-36,0,2);
  mylcd.Fill_Colors(0, 0, mylcd.Get_Width(), mylcd.Get_Height(), (uint16_t *)my_lcd.getPointer()); 
}

void loop()
{
  if(my_touch.Get_Touch()) 
  {
    uint16_t touchX = my_touch.touch.x[0];
    uint16_t touchY = my_touch.touch.y[0];
    
    if((touchX < mylcd.Get_Width()) && (touchY < mylcd.Get_Height())) 
    {
      if(lastX == 0xFFFF)
      {
        lastX = touchX;
        lastY = touchY;
      }
      
      if(touchX > (mylcd.Get_Width()-36) && (touchY < 16))
      {
        my_lcd.fillSprite(WHITE);
        my_lcd.setTextColor(BLUE);
        my_lcd.drawString("RST",mylcd.Get_Width()-36,0,2);
        mylcd.Fill_Colors(0, 0, mylcd.Get_Width(), mylcd.Get_Height(), (uint16_t *)my_lcd.getPointer()); 
      } 
      else
      {
        LCD_Draw_Line(lastX, lastY, touchX, touchY, 2, RED);
        mylcd.Fill_Colors(0, 0, mylcd.Get_Width(), mylcd.Get_Height(), (uint16_t *)my_lcd.getPointer());
        Serial.print("lastX:");Serial.print(lastX);
        Serial.print("lastY:");Serial.print(lastY);
        Serial.print("touchX:");Serial.print(touchX);
        Serial.print("touchY:");Serial.println(touchY);
      }          
      
      lastX = touchX;
      lastY = touchY;
    }
  }
  else
  {
    lastX = 0xFFFF;
    lastY = 0xFFFF;
  }
}