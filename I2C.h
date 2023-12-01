/*
 * I2C.h
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
#ifndef I2C_H_
#define I2C_H_


#include "stdint.h"
#include "CONFIG.h"

void I2C_init();
void I2C_write(I2C_Device_t device);
void I2C_read(I2C_Device_t device);
uint8_t I2C_get_transfer_success_flag();


#endif /* I2C_H_ */
