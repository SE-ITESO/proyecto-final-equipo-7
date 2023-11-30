/*
 * LDR.h
 *
 *  Created on: 26 nov. 2023
 *      Author: HP}
 */

#ifndef LDR_H_
#define LDR_H_

#include "stdint.h"

void LDR_config();
uint16_t LDR_read(uint8_t ldr);
void LDR_read_all();

#endif /* LDR_H_ */
