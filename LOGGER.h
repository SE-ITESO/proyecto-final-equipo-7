/*
 * LOGGER.h
 *
 *  Created on: 20 nov. 2023
 *      Author: HP}
 */

#ifndef LOGGER_H_
#define LOGGER_H_

#include "stdint.h"

void LOGGER_set_device();
void LOGGER_write_data(uint8_t log, uint8_t cap);
void LOGGER_read_data(uint8_t log, uint8_t cap);
uint8_t* LOGGER_get_data(uint8_t log);
void LOGGER_write_caps(uint8_t log, uint8_t caps);
uint8_t LOGGER_read_caps(uint8_t log);

#endif /* LOGGER_H_ */
