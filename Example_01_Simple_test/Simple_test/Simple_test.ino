//This application does not rely on any libraries and it is for ST7796

//This program is a demo of clearing screen.

//when using the BREAKOUT BOARD only and using these hardware spi lines to the LCD,
//the SDA pin and SCK pin is defined by the system and can't be modified.
//if you don't need to control the LED pin,you can set it to 3.3V and set the pin definition to -1.
//other pins can be defined by youself,for example
//pin usage as follow:
//            CS  DC/RS  RESET  SDI/MOSI  SCK  SDO/MISO  LED    VCC     GND    
//ESP32-S3:   10    2      15      11      12      13     21     5V     GND                  

//Remember to set the pins to suit your display module!
/********************************************************************************
* @attention
*
* THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
* WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
* TIME. AS A RESULT, QD electronic SHALL NOT BE HELD LIABLE FOR ANY
* DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
* FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE 
* CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
**********************************************************************************/
#include <Arduino.h>
#include <SPI.h>
#include <Print.h>
#include "spi_dev.h"
#include "driver/spi_master.h"

uint16_t width = LCD_WIDTH;
uint16_t height = LCD_HEIGHT;
/*
static const lcd_init_cmd st77922_lcd_init[] = {
    {0xF1, (uint8_t []){0x00}, 1, 0},
    {0x60, (uint8_t []){0x00, 0x00, 0x00}, 3, 0},
    {0x65, (uint8_t []){0x80}, 1, 0},
    {0x79, (uint8_t []){0x06}, 1, 0},
    {0x7B, (uint8_t []){0x00, 0x08, 0x08}, 3, 0},
    {0x80, (uint8_t []){0x55, 0x62, 0x2F, 0x17, 0xF0, 0x52, 0x70, 0xD2, 0x52, 0x62, 0xEA}, 11, 0},
    {0x81, (uint8_t []){0x26, 0x52, 0x72, 0x27}, 4, 0},
    {0x84, (uint8_t []){0x92, 0x25}, 2, 0},
    {0x87, (uint8_t []){0x10, 0x10, 0x58, 0x00, 0x02, 0x3A}, 6, 0},
    {0x88, (uint8_t []){0x00, 0x00, 0x2C, 0x10, 0x04, 0x00, 0x00, 0x00, 0x01, 0x01, 0x01, 0x01, 0x01, 0x00, 0x06}, 15, 0},
    {0x89, (uint8_t []){0x00, 0x00, 0x00}, 3, 0},
    {0x8A, (uint8_t []){0x13, 0x00, 0x2C, 0x00, 0x00, 0x2C, 0x10, 0x10, 0x00, 0x3E, 0x19}, 11, 0},
    {0x8B, (uint8_t []){0x15, 0xB1, 0xB1, 0x44, 0x96, 0x2C, 0x10, 0x97, 0x8E}, 9, 0},
    {0x8C, (uint8_t []){0x1D, 0xB1, 0xB1, 0x44, 0x96, 0x2C, 0x10, 0x50, 0x0F, 0x01, 0xC5, 0x12, 0x09}, 13, 0},
    {0x8D, (uint8_t []){0x0C}, 1, 0},
    {0x8E, (uint8_t []){0x33, 0x01, 0x0C, 0x13, 0x01, 0x01}, 6, 0},
    {0xB3, (uint8_t []){0x00, 0x30}, 2, 0},
    {0xF1, (uint8_t []){0x00}, 1, 0},
    {0x71, (uint8_t []){0xC0}, 1, 0},
    {0x66, (uint8_t []){0x02, 0x3F}, 2,  0},
    {0xBE, (uint8_t []){0x1E, 0x00, 0x9D}, 3, 0},
    {0x70, (uint8_t []){0x01, 0xA6, 0x11, 0x40, 0xE0, 0x00, 0x11, 0x60, 0x11, 0x00, 0x00, 0x1A}, 12, 0},
    {0x90, (uint8_t []){0x04, 0x04, 0x55, 0x74, 0x00, 0x40, 0x43, 0x2D, 0x2D}, 9, 0},
    {0x91, (uint8_t []){0x04, 0x04, 0x55, 0x75, 0x00, 0x40, 0x42, 0x2D, 0x2D}, 9, 0},
    {0x92, (uint8_t []){0x04, 0x44, 0x55, 0xC0, 0x06, 0x00, 0x07, 0x05, 0x90, 0x2D}, 10, 0},
    {0x93, (uint8_t []){0x04, 0x43, 0x11, 0x00, 0x00, 0x00, 0x00, 0x05, 0x90, 0x2D}, 10, 0},
    {0x94, (uint8_t []){0x00, 0x00, 0x00, 0x00, 0x00, 0x00}, 6, 0},
    {0x95, (uint8_t []){0x96, 0x16, 0x00, 0x00, 0xFF}, 5, 0},
    {0x96, (uint8_t []){0x44, 0x53, 0x03, 0x12, 0x23, 0x24, 0x06, 0x05, 0x9A, 0x2D, 0x00, 0x44}, 12, 0},
    {0x97, (uint8_t []){0x44, 0x53, 0x47, 0x56, 0x20, 0x20, 0x02, 0x01, 0x9A, 0x2D, 0x00, 0x44}, 12, 0},
    {0xBA, (uint8_t []){0x55, 0x9A, 0x2D, 0x9A, 0x2D}, 5, 0},
    {0x9A, (uint8_t []){0x40, 0x00, 0x06, 0x00, 0x00, 0x00, 0x00}, 7, 0},
    {0x9B, (uint8_t []){0x00, 0x00, 0x06, 0x00, 0x00, 0x00, 0x00}, 7, 0},
    {0x9C, (uint8_t []){0x5C, 0x12, 0x00, 0x00, 0x10, 0x12, 0x00, 0x00, 0x10, 0x02, 0x00, 0x00, 0x00}, 13, 0},
    {0x9D, (uint8_t []){0x8A, 0x51, 0x00, 0x00, 0x00, 0x80, 0x1E, 0x01}, 8, 0},
    {0x9E, (uint8_t []){0x51, 0x00, 0x00, 0x00, 0x80, 0x1E, 0x01}, 7, 0},
    {0xB4, (uint8_t []){0x1D, 0x1C, 0x1E, 0x0B, 0x14, 0x02, 0x13, 0x09, 0x1E, 0x00, 0x1E, 0x10}, 12, 0},
    {0xB5, (uint8_t []){0x1D, 0x1C, 0x1E, 0x0A, 0x15, 0x03, 0x11, 0x08, 0x1E, 0x01, 0x1E, 0x12}, 12, 0},
    {0xB6, (uint8_t []){0x77, 0x77, 0x00, 0x0A, 0xFF, 0x0A, 0xFF}, 7, 0},
    {0x86, (uint8_t []){0xC6, 0x04, 0xB1, 0x02, 0x58, 0x12, 0x58, 0x0C, 0x13, 0x01, 0xA5, 0x00, 0xA5, 0xA5}, 14, 0},
    {0xB7, (uint8_t []){0x07, 0x0A, 0x0E, 0x06, 0x05, 0x03, 0x2B, 0x03, 0x03, 0x42, 0x07, 0x10, 0x10, 0x2E, 0x3F, 0x0D}, 16, 0},
    {0xB8, (uint8_t []){0x07, 0x0A, 0x0D, 0x05, 0x05, 0x02, 0x2B, 0x02, 0x03, 0x42, 0x06, 0x10, 0x0F, 0x2E, 0x3F, 0x0D}, 16, 0},
    {0xB9, (uint8_t []){0x23, 0x23}, 2, 0},
    {0xBF, (uint8_t []){0x10, 0x14, 0x14, 0x0B, 0x0B, 0x0B}, 6, 0},
    {0xF2, (uint8_t []){0x00}, 1, 0},
    {0x73, (uint8_t []){0x04, 0xDA, 0x12, 0x54, 0x47}, 5, 0},
    {0x77, (uint8_t []){0x6B, 0x5B, 0xFD, 0xC3, 0xC5}, 5, 0},
    {0x7A, (uint8_t []){0x15, 0x27}, 2, 0},
    {0x7B, (uint8_t []){0x04, 0x57}, 2, 0},
    {0x7E, (uint8_t []){0x01, 0x0E}, 2, 0},
    {0xBF, (uint8_t []){0x36}, 1, 0},
    {0xE3, (uint8_t []){0x40, 0x40}, 2, 0},
    {0xF0, (uint8_t []){0x00}, 1, 0},
    {0xD0, (uint8_t []){0x00}, 1, 0},
    {0x2A, (uint8_t []){0x00, 0x00, 0x01, 0x3F}, 4, 0},
    {0x2B, (uint8_t []){0x00, 0x00, 0x01, 0xDF}, 4, 0},
    {0x21, (uint8_t []){0x00}, 0, 0},
    {0x11, (uint8_t []){0x00}, 0, 120},
    {0x29, (uint8_t []){0x00}, 0, 0},
    {0x2C, (uint8_t []){0x00}, 0, 0},
    {0x3A, (uint8_t []){0x01}, 1, 0},
    {0x36, (uint8_t []){0x00}, 1, 0},
    {0x35, (uint8_t []){0x01}, 1, 20},
};
*/

static const lcd_init_cmd st77922_lcd_init[] = {
    {0xF1, (uint8_t []){0x00}, 1, 0},
    {0x60, (uint8_t []){0x00, 0x00, 0x00}, 3, 0},
    {0x65, (uint8_t []){0x80}, 1, 0},
    {0x79, (uint8_t []){0x06}, 1, 0},
    {0x7B, (uint8_t []){0x00, 0x08, 0x08}, 3, 0},
    {0x80, (uint8_t []){0x55, 0x62, 0x2F, 0x17, 0xF0, 0x52, 0x70, 0xD2, 0x52, 0x62, 0xEA}, 11, 0},
    {0x81, (uint8_t []){0x26, 0x52, 0x72, 0x27}, 4, 0},
    {0x84, (uint8_t []){0x92, 0x25}, 2, 0},
    {0x87, (uint8_t []){0x10, 0x10, 0x58, 0x00, 0x02, 0x3A}, 6, 0},
    {0x88, (uint8_t []){0x00, 0x00, 0x2C, 0x10, 0x04, 0x00, 0x00, 0x00, 0x01, 0x01, 0x01, 0x01, 0x01, 0x00, 0x06}, 15, 0},
    {0x89, (uint8_t []){0x00, 0x00, 0x00}, 3, 0},
    {0x8A, (uint8_t []){0x13, 0x00, 0x2C, 0x00, 0x00, 0x2C, 0x10, 0x10, 0x00, 0x3E, 0x19}, 11, 0},
    {0x8B, (uint8_t []){0x15, 0xB1, 0xB1, 0x44, 0x96, 0x2C, 0x10, 0x97, 0x8E}, 9, 0},
    {0x8C, (uint8_t []){0x1D, 0xB1, 0xB1, 0x44, 0x96, 0x2C, 0x10, 0x50, 0x0F, 0x01, 0xC5, 0x12, 0x09}, 13, 0},
    {0x8D, (uint8_t []){0x0C}, 1, 0},
    {0x8E, (uint8_t []){0x33, 0x01, 0x0C, 0x13, 0x01, 0x01}, 6, 0},
    {0xB3, (uint8_t []){0x00, 0x30}, 2, 0},
    {0xF1, (uint8_t []){0x00}, 1, 0},
    {0x71, (uint8_t []){0xD0}, 1, 0},
    {0x66, (uint8_t []){0x02, 0x3F}, 2,  0},
    {0xBE, (uint8_t []){0x26, 0x00, 0x9D}, 3, 0},
    {0x70, (uint8_t []){0x01, 0xA0, 0x11, 0x40, 0xE0, 0x00, 0x11, 0x69, 0x11, 0x00, 0x00, 0x1A}, 12, 0},
    {0x90, (uint8_t []){0x04, 0x04, 0x55, 0x74, 0x00, 0x40, 0x43, 0x27, 0x27}, 9, 0},
    {0x91, (uint8_t []){0x04, 0x04, 0x55, 0x75, 0x00, 0x40, 0x42, 0x27, 0x27}, 9, 0},
    {0x92, (uint8_t []){0x04, 0x44, 0x55, 0xC0, 0x06, 0x00, 0x07, 0x05, 0x90, 0x27}, 10, 0},
    {0x93, (uint8_t []){0x04, 0x43, 0x11, 0x00, 0x00, 0x00, 0x00, 0x05, 0x90, 0x27}, 10, 0},
    {0x94, (uint8_t []){0x00, 0x00, 0x00, 0x00, 0x00, 0x00}, 6, 0},
    {0x95, (uint8_t []){0x96, 0x16, 0x00, 0x00, 0xFF}, 5, 0},
    {0x96, (uint8_t []){0x44, 0x53, 0x03, 0x12, 0x23, 0x24, 0x06, 0x05, 0x94, 0x27, 0x00, 0x44}, 12, 0},
    {0x97, (uint8_t []){0x44, 0x53, 0x47, 0x56, 0x20, 0x20, 0x02, 0x01, 0x94, 0x27, 0x00, 0x44}, 12, 0},
    {0xBA, (uint8_t []){0x55, 0x94, 0x2D, 0x94, 0x27}, 5, 0},
    {0x9A, (uint8_t []){0x40, 0x00, 0x06, 0x00, 0x00, 0x00, 0x00}, 7, 0},
    {0x9B, (uint8_t []){0x00, 0x00, 0x06, 0x00, 0x00, 0x00, 0x00}, 7, 0},
    {0x9C, (uint8_t []){0x5C, 0x12, 0x00, 0x00, 0x10, 0x12, 0x00, 0x00, 0x10, 0x02, 0x00, 0x00, 0x00}, 13, 0},
    {0x9D, (uint8_t []){0x8A, 0x51, 0x00, 0x00, 0x00, 0x80, 0x1E, 0x01}, 8, 0},
    {0x9E, (uint8_t []){0x51, 0x00, 0x00, 0x00, 0x80, 0x1E, 0x01}, 7, 0},
    {0xB4, (uint8_t []){0x1D, 0x1C, 0x1E, 0x0B, 0x14, 0x02, 0x13, 0x09, 0x1E, 0x00, 0x1E, 0x10}, 12, 0},
    {0xB5, (uint8_t []){0x1D, 0x1C, 0x1E, 0x0A, 0x15, 0x03, 0x11, 0x08, 0x1E, 0x01, 0x1E, 0x12}, 12, 0},
    {0xB6, (uint8_t []){0x77, 0x77, 0x00, 0x0A, 0xFF, 0x0A, 0xFF}, 7, 0},
    {0x86, (uint8_t []){0xCD, 0x04, 0xB1, 0x02, 0x58, 0x12, 0x58, 0x0C, 0x13, 0x01, 0xA5, 0x00, 0xA5, 0xA5}, 14, 0},
    {0xB7, (uint8_t []){0x07, 0x0A, 0x0E, 0x06, 0x05, 0x03, 0x2B, 0x03, 0x03, 0x42, 0x07, 0x10, 0x10, 0x2E, 0x3F, 0x0D}, 16, 0},
    {0xB8, (uint8_t []){0x07, 0x0A, 0x0D, 0x05, 0x05, 0x02, 0x2B, 0x02, 0x03, 0x42, 0x06, 0x10, 0x0F, 0x2E, 0x3F, 0x0D}, 16, 0},
    {0xB9, (uint8_t []){0x23, 0x23}, 2, 0},
    {0xBF, (uint8_t []){0x10, 0x14, 0x14, 0x0B, 0x0B, 0x0B}, 6, 0},
    {0xF2, (uint8_t []){0x00}, 1, 0},
    {0x73, (uint8_t []){0x04, 0xDA, 0x12, 0x54, 0x47}, 5, 0},
    {0x77, (uint8_t []){0x6B, 0x5B, 0xFD, 0xC3, 0xC5}, 5, 0},
    {0x7A, (uint8_t []){0x15, 0x27}, 2, 0},
    {0x7B, (uint8_t []){0x04, 0x57}, 2, 0},
    {0x7E, (uint8_t []){0x01, 0x0E}, 2, 0},
    {0xBF, (uint8_t []){0x36}, 1, 0},
    {0xE3, (uint8_t []){0x40, 0x40}, 2, 0},
    {0xF0, (uint8_t []){0x00}, 1, 0},
    {0xD0, (uint8_t []){0x00}, 1, 0},
    {0x2A, (uint8_t []){0x00, 0x00, 0x01, 0x3F}, 4, 0},
    {0x2B, (uint8_t []){0x00, 0x00, 0x01, 0xDF}, 4, 0},
    {0x21, (uint8_t []){0x00}, 0, 0},
    {0x11, (uint8_t []){0x00}, 0, 120},
    {0x29, (uint8_t []){0x00}, 0, 0},
    {0x2C, (uint8_t []){0x00}, 0, 0},
    {0x3A, (uint8_t []){0x01}, 1, 0},
    {0x36, (uint8_t []){0x00}, 1, 0},
    {0x35, (uint8_t []){0x01}, 1, 20},
};

static spi_device_handle_t qspi;

void QSPI_Init(void)
{
    const spi_bus_config_t buscfg = 
    {
        .data0_io_num = QSPI_D0,
        .data1_io_num = QSPI_D1,
        .sclk_io_num = QSPI_SCLK,
        .data2_io_num = QSPI_D2,
        .data3_io_num = QSPI_D3,
        .max_transfer_sz = (TX_LEN*16)+8,
        .flags = SPICOMMON_BUSFLAG_MASTER | SPICOMMON_BUSFLAG_IOMUX_PINS |SPICOMMON_BUSFLAG_QUAD,
    };
    ESP_ERROR_CHECK(spi_bus_initialize(QSPI_PORT, &buscfg, SPI_DMA_CH_AUTO));
    spi_device_interface_config_t devcfg = {
        .command_bits = 0,
        .address_bits = 0,
        .mode = QSPI_MODE,
        .clock_speed_hz = QSPI_FREQUENCY,
        .spics_io_num = LCD_CS,
        .flags = SPI_DEVICE_HALFDUPLEX ,
        .queue_size = 17,
    };
    ESP_ERROR_CHECK(spi_bus_add_device(QSPI_PORT, &devcfg, &qspi));
}

void LCD_Write_Reg(uint32_t cmd, void *data, uint8_t len)
{
    LCD_CS_LOW; 
    spi_transaction_ext_t qspit;
    memset(&qspit, 0, sizeof(qspit));
    qspit.base.flags = (SPI_TRANS_VARIABLE_CMD | SPI_TRANS_VARIABLE_ADDR);
    qspit.base.cmd = 0x02;
    qspit.base.addr = cmd << 8;
    qspit.command_bits = 8;
    qspit.address_bits = 24;
    if(len != 0)
    {
        qspit.base.tx_buffer = data;
        qspit.base.length = 8 * len;
    }
    else
    {
        qspit.base.tx_buffer = NULL;
        qspit.base.length = 0;        
    }
    spi_device_polling_transmit(qspi, (spi_transaction_t *)&qspit);
    LCD_CS_HIGH;
}

void LCD_Write_Data(uint16_t *data, uint32_t len)
{
    bool flag = 1;
    size_t tx_len;
    size_t qlen = len;
    uint16_t* tx_addr = data;
    spi_transaction_ext_t espit = {0};
    
    LCD_CS_LOW;
     do
     {
         if(flag)
        {
            espit.base.flags = (SPI_TRANS_MODE_QIO | SPI_TRANS_VARIABLE_CMD | SPI_TRANS_VARIABLE_ADDR);
            espit.base.cmd = 0x32;
            espit.base.addr = 0x3C << 8;
            espit.command_bits = 8;
            espit.address_bits = 24;
            flag = 0;
        }
        else
        {
            espit.base.flags = (SPI_TRANS_MODE_QIO | SPI_TRANS_VARIABLE_CMD | SPI_TRANS_VARIABLE_ADDR | SPI_TRANS_VARIABLE_DUMMY);
        }
        tx_len = (qlen>TX_LEN)?TX_LEN:qlen;
        espit.base.tx_buffer = tx_addr;
        espit.base.length = tx_len * 16;
        spi_device_polling_transmit(qspi, (spi_transaction_t *)&espit);
        qlen -= tx_len;
        tx_addr += tx_len;
  }while(qlen>0);
  LCD_CS_HIGH;
}

void LCD_Set_Windows(uint16_t sx, uint16_t sy, uint16_t ex, uint16_t ey)
{
    LCD_Write_Reg(SET_X_CMD, (uint8_t []){(uint8_t)(sx >> 8),(uint8_t)(sx & 0xFF),(uint8_t)((ex - 1) >> 8),(uint8_t)((ex - 1) & 0xFF)}, 4);
    LCD_Write_Reg(SET_Y_CMD, (uint8_t []){(uint8_t)(sy >> 8),(uint8_t)(sy & 0xFF),(uint8_t)((ey - 1) >> 8),(uint8_t)((ey - 1) & 0xFF)}, 4);
}

void LCD_Set_Rotation(uint8_t r)
{
    uint8_t value;
    switch(r)
    {
        case 0:
            value = (0<<3)|(0<<6);
            width = LCD_WIDTH;
            height = LCD_HEIGHT;
            break;
        case 1:
            value = (0<<3)|(1<<2)|(1<<6);
            width = LCD_HEIGHT;
            height = LCD_WIDTH;
            break;
        case 2:
            value = (0<<3)|(1<<6)|(1<<7);
            width = LCD_WIDTH;
            height = LCD_HEIGHT;
            break;
        case 3:
            value = (0<<3)|(1<<5)|(1<<7);
            width = LCD_HEIGHT;
            height = LCD_WIDTH;
            break;
        default:
            break;
    }
    LCD_Write_Reg(MADCTL_CMD, &value, 1);
}

void LCD_Init(void)
{
    uint16_t i = 0;
    for(i=0; i<sizeof(st77922_lcd_init)/sizeof(lcd_init_cmd); i++)
    {
        LCD_Write_Reg(st77922_lcd_init[i].cmd, st77922_lcd_init[i].data, st77922_lcd_init[i].len);
        delay(st77922_lcd_init[i].delay_ms);
    }
}

void LCD_Fill(uint16_t sx, uint16_t sy, uint16_t w, uint16_t h, uint16_t color)
{
    //uint16_t tmp = 0;
    bool flag = true;
    uint32_t i = 0;
    uint16_t color_swap;
    uint16_t *cbuf;
    size_t total = 0;
    size_t fill_len, tx_len;
    spi_transaction_ext_t espit = {0};
    if((sx >= LCD_WIDTH) || (sy >= LCD_HEIGHT))
        return;
    if(((sx + w) > LCD_WIDTH) || ((sy + h) > LCD_HEIGHT))
        return;
    if(((w < 1) || (w > LCD_WIDTH)) || ((h < 1) || (h > LCD_HEIGHT)))
        return;
    if(sx%4)
    {
        sx -= (sx%4);
    }
    if(w%4)
    {
        w -= (w%4);
    }
    //tmp = sx;
    //sx = sy;
   // sy = tmp;
    total = w*h;
    fill_len = (total>TX_LEN)?TX_LEN:total;
    color_swap = (color>>8)|(color<<8);
    cbuf = (uint16_t *)ps_malloc(sizeof(uint16_t)*fill_len);
    for(i=0; i<fill_len; i++)
    {
        cbuf[i] = color_swap;
    }
    LCD_Set_Windows(sx, sy, sx + w, sy + h);
    LCD_CS_LOW;
     do
     {
        if(flag)
        {
            espit.base.flags = (SPI_TRANS_MODE_QIO | SPI_TRANS_VARIABLE_CMD | SPI_TRANS_VARIABLE_ADDR);
            espit.base.cmd = 0x32;
            espit.base.addr = 0x3C << 8;
            espit.command_bits = 8;
            espit.address_bits = 24;
            flag = false;
        }
        else
        {
            espit.base.flags = (SPI_TRANS_MODE_QIO | SPI_TRANS_VARIABLE_CMD | SPI_TRANS_VARIABLE_ADDR | SPI_TRANS_VARIABLE_DUMMY);
        }
        tx_len = (total>TX_LEN)?TX_LEN:total;
        espit.base.tx_buffer = cbuf;
        espit.base.length = tx_len * 16;
        spi_device_polling_transmit(qspi, (spi_transaction_t *)&espit);
        total -= tx_len;
  }while(total>0);
  free(cbuf);
  LCD_CS_HIGH;
}

void LCD_Clear(uint16_t color)
{
    bool flag = true;
    size_t tx_len;
    uint32_t i = 0;
    uint16_t color_swap = (color>>8)|(color<<8);
    uint16_t *cbuf;
    size_t total = LCD_WIDTH*LCD_HEIGHT;
    spi_transaction_ext_t espit = {0};
    cbuf = (uint16_t *)ps_malloc(sizeof(uint16_t)*TX_LEN);
    for(i=0; i<TX_LEN; i++)
    {
        cbuf[i] = color_swap;
    }
    LCD_Set_Windows(0, 0, LCD_WIDTH, LCD_HEIGHT);
    LCD_CS_LOW;
     do
     {
        if(flag)
        {
            espit.base.flags = (SPI_TRANS_MODE_QIO | SPI_TRANS_VARIABLE_CMD | SPI_TRANS_VARIABLE_ADDR);
            espit.base.cmd = 0x32;
            espit.base.addr = 0x3C << 8;
            espit.command_bits = 8;
            espit.address_bits = 24;
            flag = false;
        }
        else
        {
            espit.base.flags = (SPI_TRANS_MODE_QIO | SPI_TRANS_VARIABLE_CMD | SPI_TRANS_VARIABLE_ADDR | SPI_TRANS_VARIABLE_DUMMY);
        }
        tx_len = (total>TX_LEN)?TX_LEN:total;
        espit.base.tx_buffer = cbuf;
        espit.base.length = tx_len * 16;
        spi_device_polling_transmit(qspi, (spi_transaction_t *)&espit);
        total -= tx_len;
  }while(total>0);
  free(cbuf);
  LCD_CS_HIGH;
}

void LCD_Draw_Point(uint16_t x, uint16_t y, uint16_t color)
{
    LCD_Fill(x, y, 4, 1, color);
}

void LCD_Fill_Colors(uint16_t sx, uint16_t sy, uint16_t w, uint16_t h, uint16_t* color)
{
    size_t total = 0;
    if((sx >= LCD_WIDTH) || (sy >= LCD_HEIGHT))
        return;
    if(((sx + w) > LCD_WIDTH) || ((sy + h) > LCD_HEIGHT))
        return;
    if(((w < 1) || (w > LCD_WIDTH)) || ((h < 1) || (h > LCD_HEIGHT)))
        return;
    if(sx%4)
    {
        sx -= (sx%4);
    }
    if(w%4)
    {
        w -= (w%4);
    }
    total = w*h;

    LCD_Write_Data(color, total);
}

void setup()
{
    Serial.begin(115200);
  pinMode(LCD_CS, OUTPUT);
  digitalWrite(LCD_CS, HIGH);
  pinMode(LCD_BL, OUTPUT);
  digitalWrite(LCD_BL, HIGH);
  QSPI_Init();
  LCD_Init();
  LCD_Set_Rotation(1);
  LCD_Clear(GRAY);
 // LCD_Draw_Point(10, 10, WHITE);
//   LCD_Clear(RED);
  //LCD_Clear_Screen(10, 10, 100, 100, RED);delay(500);
}

void loop()
{  
    LCD_Clear(RED);
    delay(3000);
    LCD_Clear(GREEN);
    delay(3000);
    LCD_Clear(BLUE);
    delay(3000);
    LCD_Clear(WHITE);
    delay(3000);
    LCD_Clear(BLACK);
    delay(3000);
}
