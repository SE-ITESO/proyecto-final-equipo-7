/*
 * PWM.h
 *
 *  Created on: 24 nov. 2023
 *      Author: HP}
 */

#ifndef PWM_H_
#define PWM_H_

#include "stdint.h"
#include "FTM.h"

void PWM_config();
void PWM_set_dutycycle(FTM_Type *ftm, uint8_t value);

#endif /* PWM_H_ */
