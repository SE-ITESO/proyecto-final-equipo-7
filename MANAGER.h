
#ifndef MANAGER_H_
#define MANAGER_H_

void MANAGER_I2C_to_UART_time();
void MANAGER_UART_to_I2C_time();

void MANAGER_I2C_to_UART_date();
void MANAGER_UART_to_I2C_date();

void MANAGER_I2C_to_UART_hum();
void MANAGER_I2C_to_UART_temp();

void MANAGER_update_seconds_uart();

void MANAGER_set_log(uint8_t num, uint8_t time, uint8_t caps1, uint8_t caps2);
void MANAGER_handler_log();
void MANAGER_I2C_to_UART_log(uint8_t log, UART_Type *uart);
void MANAGER_handler_flag();
void MANAGER_update_seconds_uart0_flag();
void MANAGER_update_seconds_uart4_flag();
void MANAGER_update_seconds_uart4_flag_off();
void MANAGER_update_seconds_uart0_flag_off();

#endif /* MANAGER_H_ */
