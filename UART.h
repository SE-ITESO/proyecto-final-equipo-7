/*
 * UART.h
 *
 * 	Created on: 10/10/2023
 * 		Author: Diego Delgado, Alberto Quintana
 *
 *  @brief
 *  This is the source file for the UART device driver.
 *  It contains the implementation of configuration functions and runtime functions for UART communication.
 *
 *  @details
 *  The UART module is configured with VT100 commands for various display manipulations.
 *  It provides functions to display menus, show time, change seconds, show settings, and update time.
 *
 *
 */
#ifndef UART_H_
#define UART_H_

#include "stdint.h"
#include "MK64F12.h"
#include "CONFIG.h"

void UART_init();
uint8_t UART_get_mail_box(UART_Num_t uart);
uint8_t UART_get_mail_flag(UART_Num_t uart);
void UART_clear_mail_flag(UART_Num_t uart);

#endif /* UART_H_ */
