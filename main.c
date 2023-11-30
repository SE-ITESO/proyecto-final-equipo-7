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
#include "fsl_gpio.h"
#include "fsl_port.h"
#include "UART.h"
#include "I2C.h"
#include "ADC.h"
#include "TERMINAL.h"
#include "MANAGER.h"
#include "SM.h"
#include "FTM.h"
#include "SPI.h"
#include "RTC.h"
#include "INA219.h"
#include "CONFIG.h"
#include "PIT.h"
#include "LDR.h"
#include "CORE_config.h"
#include "TRACKER.h"
#include "LCD_nokia.h"

int main(void)
{
	FTM_init();
	CORE_config_set();
	PIT_init();
	I2C_init();
	RTC_config();
	UART_init();
	ADC_init();
	INA219_init();
	SM_init_flags();
	LDR_config();
	SPI_config();
	LCD_nokia_init();

	TERMINAL_show_options(UART0);
	TERMINAL_show_options(UART4);

    UART_EnableInterrupts(UART0, kUART_RxDataRegFullInterruptEnable | kUART_RxOverrunInterruptEnable);
    UART_EnableInterrupts(UART4, kUART_RxDataRegFullInterruptEnable | kUART_RxOverrunInterruptEnable);

    NVIC_set_basepri_threshold(PRIORITY_7);
    NVIC_enable_interrupt_and_priority(UART0_IRQ, PRIORITY_3);
    NVIC_enable_interrupt_and_priority(UART4_IRQ, PRIORITY_4);
    NVIC_enable_interrupt_and_priority(PORTB_IRQ, PRIORITY_1);
    NVIC_enable_interrupt_and_priority(PIT_CH0_IRQ,PRIORITY_2);
    NVIC_global_enable_interrupts;
	GPIOB_callback_init(MANAGER_handler_flag);
	PIT_timer_period(PIT_CHANNEL_1,WELCOME_TIME);
	PIT_callback_init(PIT_CHANNEL_1,LCD_nokia_clr_welcome_flag);
	PIT_start(PIT_CHANNEL_1);
    PIT_timer_period(PIT_CHANNEL_0, 10000);
    PIT_callback_init(PIT_CHANNEL_0, TRACKER_flag_on);
    PIT_start(PIT_CHANNEL_0);
    LCD_nokia_show_welcome();
    while (1)
    {
    	if(FALSE==LCD_nokia_get_welcome_flag())
    	{
			MANAGER_handler_log();
			SM_state_machine();
			TRACKER_handler();
    	}
    }
}

