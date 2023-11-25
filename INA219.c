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

void INA219_config()
{

}

void INA219_set_device()
{
	ina.address = INA219_ADDRESS;
	ina.subaddress = INA219_CURR_SUBADDRESS;
	ina.subaddressSize = 1;
	ina.data = g_ina219_curr;
	ina.dataSize = CURR_FORMAT_LENGHT;
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
