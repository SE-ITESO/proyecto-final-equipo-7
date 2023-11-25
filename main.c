/*
 * main.c
 *
 * 	Created on: 09/10/2023
 * 		Author: Diego Delgado, Alberto Quintana
 *
 *  @brief
 *  This is the main source file for the application.
 *  It initializes and manages the communication between I2C and UART modules for time-related operations.
 *
 *  @details
 *  The main function configures I2C and UART modules, and then enters a loop to handle user input and manage time-related operations.
 *  It provides options to display real-time and set time using UART commands.
 *
 */

#include <stdbool.h>
#include "fsl_uart.h"
#include "NVIC.h"
#include "GPIO.h"
#include "UART.h"
#include "I2C.h"
#include "TERMINAL.h"
#include "MANAGER.h"
#include "SM.h"
#include "FTM.h"
#include "PWM.h"
#include "RTC.h"
#include "INA219.h"
#include "CONFIG.h"


int main(void)
{

	FTM_init();
	PWM_config();
	I2C_init();
	UART_init();
	RTC_config();
	INA219_config();

	TERMINAL_show_options(UART0);
	TERMINAL_show_options(UART4);

    UART_EnableInterrupts(UART0, kUART_RxDataRegFullInterruptEnable | kUART_RxOverrunInterruptEnable);
    UART_EnableInterrupts(UART4, kUART_RxDataRegFullInterruptEnable | kUART_RxOverrunInterruptEnable);

    NVIC_set_basepri_threshold(PRIORITY_7);
    NVIC_enable_interrupt_and_priority(UART0_IRQ, PRIORITY_2);
    NVIC_enable_interrupt_and_priority(UART4_IRQ, PRIORITY_2);
    NVIC_enable_interrupt_and_priority(PORTD_IRQ, PRIORITY_5);
    NVIC_global_enable_interrupts;

	GPIOD_callback_init(MANAGER_handler_flag);

    while (1)
    {
    	MANAGER_handler_log();
    	SM_state_machine();
    }
}

