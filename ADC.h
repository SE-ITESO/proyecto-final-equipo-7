/*
 * @file 	ADC.h
 *
 * @Authors Diego Delgado
 * 			Alberto Quintana
 *
 * @brief  	This file initializes ADC, which will allow us to read values from LDR'S
 *
 *
 */

#ifndef ADC_H_
#define ADC_H_

#include "stdint.h"

void ADC_init();
uint16_t ADC_set_conversion(uint8_t channel);

#endif /* ADC_H_ */
