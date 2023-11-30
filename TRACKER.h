/*
 * TRACKER.h
 *
 *  Created on: 29 nov. 2023
 *      Author: HP}
 */

#ifndef TRACKER_H_
#define TRACKER_H_

#include "stdint.h"

uint8_t TRACKER_check_tol(int16_t delta);
void TRACKER_flag_on();
void TRACKER_handler();

#endif /* TRACKER_H_ */
