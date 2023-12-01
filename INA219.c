/*
 * @file 	INA219.c
 *
 * @Authors Diego Delgado
 * 			Alberto Quintana
 *
 * @brief  This file allows us to configure and calibrate our I2C current sensor.
 * 		   In our case we use a small range since our panel provides a maximum of 160mA.
 *
 *
 */

#include "INA219.h"
#include "CONFIG.h"
#include "I2C.h"

#define INA_CONFIG 0|0x0000 |0x0700|0x0078|0x07

uint8_t g_ina219_curr[2];

I2C_Device_t ina;


//SETS INA slave address
void INA219_set_device()
{
	ina.address = INA219_ADDRESS;
	ina.subaddress = INA219_CURR_SUBADDRESS;
	ina.subaddressSize = 1;
	ina.data = g_ina219_curr;
	ina.dataSize = CURR_FORMAT_LENGHT;
}

//CONFIGURATION

void INA219_config()
{
	uint32_t config = INA_CONFIG;
	uint8_t configBytes[2];
	configBytes[0] = (config >> 8) & CONFIG_MASK;
	configBytes[1] = config & CONFIG_MASK;

	ina.address = INA219_ADDRESS;
	ina.subaddress = INA219_CONFIG_SUBADDRESS;
	ina.subaddressSize = 1;
	ina.data = configBytes;
	ina.dataSize = 2;
	I2C_write(ina);
}

//INA calibration value, determines our current range that will be used.
void INA219_calib()
{
	uint32_t calibrationValue=8192;
	uint8_t calibrationBytes[2];
	calibrationBytes[0] = (calibrationValue >> 8) & CONFIG_MASK;
	calibrationBytes[1] = calibrationValue & CONFIG_MASK;

	ina.address = INA219_ADDRESS;
	ina.subaddress = INA219_CALIB_SUBADDRESS;
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
