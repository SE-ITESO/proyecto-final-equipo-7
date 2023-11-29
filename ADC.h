/*
 * ADC.h
 *
 *  Created on: 26 nov. 2023
 *      Author: HP}
 */

#ifndef ADC_H_
#define ADC_H_

#include "stdint.h"

void ADC_init();
uint16_t ADC_set_conversion(uint8_t channel);

#endif /* ADC_H_ */
