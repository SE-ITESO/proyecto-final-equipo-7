
#ifndef MANAGER_H_
#define MANAGER_H_

#include "stdint.h"
#include "UART.h"

void MANAGER_RTC_to_TERMINAL_time();
void MANAGER_TERMINAL_to_RTC_time();
void MANAGER_RTC_to_TERMINAL_date();
void MANAGER_TERMINAL_to_RTC_date();
void MANAGER_INA_to_TERMINAL_curr();
void MANAGER_update_seconds_terminal();
void MANAGER_update_seconds_terminal1_flag();
void MANAGER_update_seconds_terminal2_flag();
void MANAGER_update_seconds_terminal1_flag_off();
void MANAGER_update_seconds_terminal2_flag_off();
void MANAGER_set_log(uint8_t log, uint8_t time, uint8_t caps1, uint8_t caps2);
void MANAGER_handler_flag();
void MANAGER_handler_log();
void MANAGER_LOGGER_to_TERMINAL(uint8_t log, UART_Type *uart);
void MANAGER_data_update_seconds_lcd();
void MANAGER_RTC_to_LCD();


#endif /* MANAGER_H_ */
