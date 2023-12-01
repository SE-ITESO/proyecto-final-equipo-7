/*
 * @file 	LDR.h
 *
 * @Authors Diego Delgado
 * 			Alberto Quintana
 *
 * @brief 	This file allows us to configure 4 ADC's to allow us to obtain values from each of the LDR's individually.
 *
 *
 */


#ifndef LDR_H_
#define LDR_H_

#include "stdint.h"

void LDR_config();
uint16_t LDR_read(uint8_t ldr);
void LDR_read_all();

#endif /* LDR_H_ */
