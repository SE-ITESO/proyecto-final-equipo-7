/*
 * INA219.h
 *
 *  Created on: Nov 8, 2023
 *      Author: alber
 */

#ifndef INA219_H_
#define INA219_H_


#include "stdint.h"

#define INA219_I2C_ADDRESS 0x40 // Dirección I2C del INA219


void INA219_config();
float READ_Current();
void INA219_calibration_reg();

#endif /* INA219_H_ */
