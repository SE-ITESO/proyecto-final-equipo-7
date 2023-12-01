/*
 * @file 	TRACKER.h
 *
 * @Authors Diego Delgado
 * 			Alberto Quintana
 *
 * @brief 	This file allows us to move the servo motors either vertically or horizontally.
 *
 *
 */

#ifndef TRACKER_H_
#define TRACKER_H_

#include "stdint.h"

uint8_t TRACKER_check_tol(int16_t delta);
void TRACKER_flag_on();
void TRACKER_handler();

#endif /* TRACKER_H_ */
