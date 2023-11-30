/*
 * MOTOR.h
 *
 *  Created on: 26 nov. 2023
 *      Author: HP}
 */

#ifndef MOTOR_H_
#define MOTOR_H_

#include "stdint.h"

#define PI 3.1416
#define ENCODER_TO_MOTOR 816
#define RAD_CONVERSION 2*PI

void MOTOR_config();
void MOTOR_encoder_channelA();
void MOTOR_position();
void MOTOR_PID(float ref);
void MOTOR_PID_flag();

#endif /* MOTOR_H_ */
