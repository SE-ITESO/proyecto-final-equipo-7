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

#ifndef INA219_H_
#define INA219_H_

#include "stdint.h"

void INA219_config();
void INA219_calib();
void INA219_init();
void INA219_set_device();
void INA219_read_curr();
uint8_t* INA219_get_curr();

#endif /* INA219_H_ */
