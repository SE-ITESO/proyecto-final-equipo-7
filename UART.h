
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
