/*
 * I2C.c
 *
 * 	Created on: 13/10/2023
 * 		Author: Diego Delgado, Alberto Quintana
 *
 *  @brief
 *  This is the source file for the I2C device driver.
 *  It contains the implementation of configuration functions and runtime functions for I2C communication.
 *
 *  @details
 *  The I2C module is configured to communicate with an RTC DS1307 module to read and write time values.
 *  It provides functions for configuring I2C, writing data, reading data, and obtaining the received time values.
 *
 *//*
 * I2C Module
 * This module provides functions to initialize and perform I2C transactions.
 */

#include "I2C.h"
#include "CONFIG.h"
#include "fsl_i2c.h"
#include "fsl_port.h"
#include "fsl_gpio.h"

// Configuration structures and global variables
i2c_master_config_t masterConfig;
uint32_t sourceClock;
i2c_master_transfer_t masterXfer;

uint8_t g_master_txBuff[I2C_DATA_LENGTH];
uint8_t g_master_rxBuff[I2C_DATA_LENGTH];

uint8_t g_transfer_success_flag;

/*

 * Initializes I2C peripheral and pins.
 * Configures I2C master with default settings, sets baud rate, and initializes I2C1.
 */
void I2C_init()
{

    CLOCK_EnableClock(kCLOCK_PortC);


    PORT_SetPinMux(PORTC, PIN10, kPORT_MuxAlt2);
    PORT_SetPinMux(PORTC, PIN11, kPORT_MuxAlt2);

    I2C_MasterGetDefaultConfig(&masterConfig);
    masterConfig.baudRate_Bps = I2C_BAUDRATE;
    sourceClock = I2C_MASTER_CLK_FREQ;
    I2C_MasterInit(I2C1, &masterConfig, sourceClock);
}


uint8_t I2C_get_transfer_success_flag()
{
    return g_transfer_success_flag;
}


void I2C_write(I2C_Device_t device)
{
    // Set data to be transmitted
    g_master_txBuff[0] = device.data[0];
    g_master_txBuff[1] = device.data[1];
    g_master_txBuff[2] = device.data[2];

    // Configure I2C transfer structure for write operation
    masterXfer.slaveAddress   = device.address;
    masterXfer.direction      = kI2C_Write;
    masterXfer.subaddress     = device.subaddress;
    masterXfer.subaddressSize = device.subaddressSize;
    masterXfer.data           = g_master_txBuff;
    masterXfer.dataSize       = device.dataSize;
    masterXfer.flags          = kI2C_TransferDefaultFlag;

    I2C_MasterTransferBlocking(I2C1, &masterXfer);
    if (I2C_MasterTransferBlocking(I2C1, &masterXfer) == kStatus_Success) {
        g_transfer_success_flag = true;
    } else {
        g_transfer_success_flag = false;
    }
}

/*
 * Reads data from an I2C device.
 * Configures the I2C transfer structure, performs a read operation, updates the received data, and updates the transfer success flag.
 /
  *
  */
void I2C_read(I2C_Device_t device)
{
    // Configure I2C transfer structure for read operation
    masterXfer.slaveAddress   = device.address;
    masterXfer.direction      = kI2C_Read;
    masterXfer.subaddress     = device.subaddress;
    masterXfer.subaddressSize = device.subaddressSize;
    masterXfer.data           = g_master_rxBuff;
    masterXfer.dataSize       = device.dataSize;
    masterXfer.flags          = kI2C_TransferDefaultFlag;

    // Perform I2C read operation, update received data, and update the transfer success flag
    I2C_MasterTransferBlocking(I2C1, &masterXfer);
    device.data[0] = g_master_rxBuff[0];
    device.data[1] = g_master_rxBuff[1];
    device.data[2] = g_master_rxBuff[2];
    if (I2C_MasterTransferBlocking(I2C1, &masterXfer) == kStatus_Success) {
        g_transfer_success_flag = true;
    } else {
        g_transfer_success_flag = false;
    }
}
