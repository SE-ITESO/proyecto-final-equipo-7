/*
 * INA219.h
 *
 *  Created on: 24 nov. 2023
 *      Author: HP}
 */

#ifndef INA219_H_
#define INA219_H_

#include "stdint.h"

void INA219_config();
void INA219_set_device();
void INA219_read_curr();
uint8_t* INA219_get_curr();

#endif /* INA219_H_ */
