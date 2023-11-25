/*
 * RTC.h
 *
 *  Created on: 20 nov. 2023
 *      Author: HP}
 */

#ifndef RTC_H_
#define RTC_H_

#include "stdint.h"

void RTC_set_device_squarewave();
void RTC_set_device_time();
void RTC_set_device_date();
void RTC_set_squarewave();
void RTC_config();
void RTC_write_time();
void RTC_read_time();
void RTC_write_date();
void RTC_read_date();
uint8_t* RTC_get_time();
uint8_t* RTC_get_date();

#endif /* RTC_H_ */
