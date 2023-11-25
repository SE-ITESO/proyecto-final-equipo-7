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
 */
#include "I2C.h"
#include "CONFIG.h"
#include "fsl_i2c.h"

i2c_master_transfer_t masterXfer;

uint8_t g_master_txBuff[I2C_DATA_LENGTH];
uint8_t g_master_rxBuff[I2C_DATA_LENGTH];

uint8_t g_transfer_success_flag;


void I2C_init()
{
		i2c_master_config_t masterConfig;
	    I2C_MasterGetDefaultConfig(&masterConfig);
	    masterConfig.baudRate_Bps = I2C_BAUDRATE;
	    uint32_t sourceClock = I2C_MASTER_CLK_FREQ;
	    I2C_MasterInit(I2C0, &masterConfig, sourceClock);
}


uint8_t I2C_get_transfer_success_flag()
{
    return g_transfer_success_flag;
}

void I2C_write(I2C_Device_t device)
{
	masterXfer.slaveAddress   = device.address;
	masterXfer.direction      = kI2C_Write;
	masterXfer.subaddress     = device.subaddress;
	masterXfer.subaddressSize = device.subaddressSize;
	masterXfer.data           = device.data;
	masterXfer.dataSize       = device.dataSize;
	masterXfer.flags          = kI2C_TransferDefaultFlag;

	I2C_MasterTransferBlocking(I2C0, &masterXfer);
	if (I2C_MasterTransferBlocking(I2C0, &masterXfer) == kStatus_Success) {
		g_transfer_success_flag=true;
	} else {
		g_transfer_success_flag=false;
	}
}

void I2C_read(I2C_Device_t device)
{
	masterXfer.slaveAddress   = device.address;
	masterXfer.direction      = kI2C_Read;
	masterXfer.subaddress     = device.subaddress;
	masterXfer.subaddressSize = device.subaddressSize;
	masterXfer.data           = device.data;
	masterXfer.dataSize       = device.dataSize;
	masterXfer.flags          = kI2C_TransferDefaultFlag;

	I2C_MasterTransferBlocking(I2C0, &masterXfer);
	if (I2C_MasterTransferBlocking(I2C0, &masterXfer) == kStatus_Success) {
		g_transfer_success_flag=true;
	} else {
		g_transfer_success_flag=false;
	}
}
