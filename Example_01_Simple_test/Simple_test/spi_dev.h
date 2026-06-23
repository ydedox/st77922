#ifndef __SPI_DEV_H__
#define __SPI_DEV_H__
#include <stdint.h>
#include "hal/gpio_ll.h"

#define RED 0xF800
#define GREEN 0x07E0
#define BLUE 0x001F
#define WHITE 0xFFFF
#define BLACK 0x0
#define GRAY 0x8430

#define LCD_WIDTH 320
#define LCD_HEIGHT 480

#define WR_RAM_CMD 0x2C
#define RD_RAM_CMD 0x2E
#define SET_X_CMD  0x2A
#define SET_Y_CMD  0x2B
#define MADCTL_CMD 0x36

#define TX_LEN (0x4000)

#define QSPI_PORT SPI2_HOST  //FSPI
#define QSPI_FREQUENCY 80000000
#define QSPI_MODE SPI_MODE0

//pin define for ESP32-S3, include lcd pin
#define LCD_CS  10
#define LCD_BL  41
//below is QSPI pin
#define QSPI_SCLK 12
#define QSPI_D0   11
#define QSPI_D1   13
#define QSPI_D2   14
#define QSPI_D3   9

//Pin operation
#if ((LCD_CS>=0) && (LCD_CS<32))
    #define LCD_CS_LOW  GPIO.out_w1tc = (1 << LCD_CS)
    #define LCD_CS_HIGH GPIO.out_w1ts = (1 << LCD_CS)
#elif (LCD_CS>=32)
    #define LCD_CS_LOW  GPIO.out1_w1tc.val = (1 << (LCD_CS - 32))
    #define LCD_CS_HIGH GPIO.out1_w1ts.val = (1 << (LCD_CS - 32))
#endif

#if ((LCD_BL>=0) && (LCD_BL<32))
    #define LCD_BL_LOW  GPIO.out_w1tc = (1 << LCD_BL)
    #define LCD_BL_HIGH GPIO.out_w1ts = (1 << LCD_BL)
#elif (LCD_BL>=32)
    #define LCD_BL_LOW  GPIO.out1_w1tc.val = (1 << (LCD_BL - 32))
    #define LCD_BL_HIGH GPIO.out1_w1ts.val = (1 << (LCD_BL - 32))
#endif

typedef struct {
    uint8_t cmd;       
    void *data;      
    uint8_t len;     
    uint32_t delay_ms; 
}lcd_init_cmd;

#endif
