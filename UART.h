
#ifndef UART_H_
#define UART_H_

#include "stdint.h"
#include "MK64F12.h"
#include "CONFIG.h"
#include "fsl_uart.h"

void UART_init();
void UART_show_options(UART_Type *uart);
void UART_show_log(UART_Type *uart);
void UART_show_config(UART_Type *uart);

void UART_show_time(UART_Type *uart);
void UART_change_seconds(UART_Type *uart);
void UART_update_time();

void UART_show_set_time(UART_Type *uart);
void UART_msg_set_time(UART_Type *uart);

uint8_t* UART_get_rx_time();
uint8_t* UART_get_rx_hum();
uint8_t* UART_get_rx_temp();
uint8_t* UART_get_rx_date();

uint8_t UART_get_mail_box(UART_Num_t uart);
uint8_t UART_get_mail_flag(UART_Num_t uart);
void UART_clear_mail_flag(UART_Num_t uart);

void UART_show_set_date();
void UART_msg_set_date();
void UART_update_date();
void UART_show_date();

void UART_show_hum(UART_Type *uart);
void UART_show_temp(UART_Type *uart);
void UART_update_hum();
void UART_update_temp();

void UART_show_set_log(UART_Type *uart);
void UART_msg_set_log_confirm(UART_Type *uart);
void UART_show_set_log_time(UART_Type *uart);
void UART_show_set_log_cap(UART_Type *uart);
void UART_msg_set_log_done(UART_Type *uart);
void UART_msg_set_log_cancel(UART_Type *uart);
void UART_msg_busy(UART_Type *uart);
void UART_msg_busy_log(UART_Type *uart);
void UART_show_read_log(UART_Type *uart);
uint8_t UART_get_log_num();
uint8_t* UART_get_data_log(uint8_t log);
void UART_show_data_log(uint8_t log, UART_Type *uart);
void UART_show_data_log_jump(UART_Type *uart);
void UART_set_log_num(uint8_t log);

void UART_show_error_wdt(UART_Type *uart);
void UART_show_error_i2c(UART_Type *uart);
#endif /* UART_H_ */
