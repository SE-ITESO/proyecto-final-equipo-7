/*
 * I2C.h
 *
 *  Created on: Nov 8, 2023
 *      Author: alber
 */

#ifndef I2C_H_
#define I2C_H_

#include "stdint.h"

void I2C_init(void);


void I2C_rtc_squarewave();
void I2C_init();
void I2C_write_rtc_time();
void I2C_read_rtc_time();
void I2C_write_rtc_date();
void I2C_read_rtc_date();

uint8_t* I2C_get_rx_time();
uint8_t* I2C_get_rx_date();
uint8_t* I2C_get_rx_hum();
uint8_t* I2C_get_rx_temp();
uint8_t I2C_get_transfer_success_flag();

void I2C_init_dht();
void I2C_read_dht();

void I2C_write_data_log(uint8_t log, uint8_t cap);
void I2C_read_log(uint8_t log, uint8_t cap);
uint8_t* I2C_get_data_log(uint8_t log);
uint8_t I2C_get_caps_log(uint8_t log);
void I2C_write_caps_log(uint8_t log, uint8_t caps);



#endif /* I2C_H_ */
