/*
 * TERMINAL.h
 *
 *  Created on: 20 nov. 2023
 *      Author: HP}
 */

#ifndef TERMINAL_H_
#define TERMINAL_H_

#include "stdint.h"
#include "UART.h"



void TERMINAL_show_options(UART_Type *uart);
void TERMINAL_show_log(UART_Type *uart);
void TERMINAL_show_config(UART_Type *uart);

void TERMINAL_show_time(UART_Type *uart);
void TERMINAL_change_seconds(UART_Type *uart);
void TERMINAL_update_time();

void TERMINAL_show_set_time(UART_Type *uart);
void TERMINAL_msg_set_time(UART_Type *uart);

uint8_t* TERMINAL_get_time();
uint8_t* TERMINAL_get_date();
uint8_t* TERMINAL_get_curr();

void TERMINAL_show_set_date();
void TERMINAL_msg_set_date();
void TERMINAL_update_date();
void TERMINAL_show_date();

void TERMINAL_show_curr();
void TERMINAL_update_curr();

void TERMINAL_show_set_log(UART_Type *uart);
void TERMINAL_msg_set_log_confirm(UART_Type *uart);
void TERMINAL_show_set_log_time(UART_Type *uart);
void TERMINAL_show_set_log_cap(UART_Type *uart);
void TERMINAL_msg_set_log_done(UART_Type *uart);
void TERMINAL_msg_set_log_cancel(UART_Type *uart);
void TERMINAL_msg_busy(UART_Type *uart);
void TERMINAL_msg_busy_log(UART_Type *uart);
void TERMINAL_show_read_log(UART_Type *uart);
uint8_t TERMINAL_get_log_num();
uint8_t* TERMINAL_get_data_log(uint8_t log);
void TERMINAL_show_data_log(uint8_t log, UART_Type *uart);
void TERMINAL_show_data_log_jump(UART_Type *uart);
void TERMINAL_set_log_num(uint8_t log);

void TERMINAL_show_error_wdt(UART_Type *uart);
void TERMINAL_show_error_i2c(UART_Type *uart);

#endif /* TERMINAL_H_ */
