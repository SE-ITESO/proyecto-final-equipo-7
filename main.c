/*
 * Copyright 2016-2023 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/**
 * @file    FINAL.c
 * @brief   Application entry point.
 */
#include <stdio.h>
#include "board.h"
#include "peripherals.h"
#include "pin_mux.h"
#include "clock_config.h"
#include "MK64F12.h"
#include "fsl_debug_console.h"

#include "fsl_i2c.h"
#include "fsl_common.h"
#include "INA219.h"
#include "I2C.h"
#include "LCD_nokia.h"
#include "SPI.h"




#define CONFIG_REG 0x00
#define SHUNT_VOLTAGE_REG 0x01
#define BUS_VOLTAGE_REG 0x02

#define CONFIG_VALUE 0x019F
//funcion de prueba
void I2C_WriteRegister(uint8_t deviceAddress, uint8_t registerAddress, uint16_t data) {
    i2c_master_transfer_t masterXfer;
    uint8_t txBuff[3];

    txBuff[0] = registerAddress;
    txBuff[1] = (data >> 8) & 0xFF;
    txBuff[2] = data & 0xFF;

    masterXfer.slaveAddress   = deviceAddress;
    masterXfer.direction      = kI2C_Write;
    masterXfer.subaddress     = 0;
    masterXfer.subaddressSize = 0;
    masterXfer.data           = txBuff;
    masterXfer.dataSize       = sizeof(txBuff);
    masterXfer.flags          = kI2C_TransferDefaultFlag;

    I2C_MasterTransferBlocking(I2C0, &masterXfer);
}



int main(void) {

    BOARD_InitPins();
    BOARD_BootClockRUN();
    BOARD_InitDebugConsole();


    SPI_config();
    I2C_init();
    INA219_config();
    LCD_nokia_init();

    while (1) {
        //pruebas para leer corriente
        float current = READ_Current();
        LCD_nokia_show_current();




        SDK_DelayAtLeastUs(1000000, 21000000);
    }
}






