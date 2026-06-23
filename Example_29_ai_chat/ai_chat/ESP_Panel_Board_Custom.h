/*
 * SPDX-FileCopyrightText: 2023-2024 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#pragma once

//ESP32-S3 IO Pin define
#define AP_ENABLE 1
#define SD_SCK  5
#define SD_CMD 4
#define SD_D0 6
#define SD_D1 7
#define SD_D2 2
#define SD_D3 3

//I2S IO Pin define
#define I2S_MCK   GPIO_NUM_17
#define I2S_BCK   GPIO_NUM_18
#define I2S_DINT  GPIO_NUM_16
#define I2S_DOUT  GPIO_NUM_15
#define I2S_WS    GPIO_NUM_21
#define I2S_NUM   I2S_NUM_1

#define I2C_SCL           39       /*!< GPIO number used for I2C master clock */
#define I2C_SDA           38       /*!< GPIO number used for I2C master data  */
#define I2C_NUM           I2C_NUM_0       /*!< I2C master i2c port number, the number of i2c peripheral interfaces available will depend on the chip */
#define I2C_SPEED         400000          /*!< I2C master clock frequency */