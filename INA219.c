/*
 * INA219.c
 *
 *  Created on: 24 nov. 2023
 *      Author: HP}
 */

#include "INA219.h"
#include "CONFIG.h"
#include "I2C.h"

uint8_t g_ina219_curr[2];

I2C_Device_t ina;

void INA219_set_device()
{
	ina.address = INA219_ADDRESS;
	ina.subaddress = INA219_CURR_SUBADDRESS;
	ina.subaddressSize = 1;
	ina.data = g_ina219_curr;
	ina.dataSize = CURR_FORMAT_LENGHT;
}

void INA219_config()
{
	uint32_t config=0|0x0000 |0x0700|0x0078|0x07;
	uint8_t configBytes[2];
	configBytes[0] = (config >> 8) & 0xFF;
	configBytes[1] = config & 0xFF;

	ina.address = INA219_ADDRESS;
	ina.subaddress = 0x00;
	ina.subaddressSize = 1;
	ina.data = configBytes;
	ina.dataSize = 2;
	I2C_write(ina);
}
void INA219_calib()
{
	uint32_t calibrationValue=8192;
	uint8_t calibrationBytes[2];
	calibrationBytes[0] = (calibrationValue >> 8) & 0xFF;
	calibrationBytes[1] = calibrationValue & 0xFF;

	ina.address = INA219_ADDRESS;
	ina.subaddress = 0x05;
	ina.subaddressSize = 1;
	ina.data = calibrationBytes;
	ina.dataSize = 2;
	I2C_write(ina);

}

void INA219_init()
{
	INA219_config();
	INA219_calib();
}
void INA219_read_curr()
{
	INA219_set_device();
	I2C_read(ina);

}

uint8_t* INA219_get_curr()
{
	return g_ina219_curr;
}
