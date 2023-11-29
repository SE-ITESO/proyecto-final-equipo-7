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
#include "RTC.h"
#include "INA219.h"
#include "CONFIG.h"
#include "MOTOR.h"
#include "PIT.h"
#include "LDR.h"
#include "CORE_config.h"
#include "TRACKER.h"

void delay(uint32_t delay)
{
	volatile int counter, counter_2;

		for(counter_2 = 16; counter_2 > 0; counter_2--)
		{
			for(counter = delay; counter > 0; counter--);

		}
}

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

	TERMINAL_show_options(UART0);
	TERMINAL_show_options(UART4);

    UART_EnableInterrupts(UART0, kUART_RxDataRegFullInterruptEnable | kUART_RxOverrunInterruptEnable);
    UART_EnableInterrupts(UART4, kUART_RxDataRegFullInterruptEnable | kUART_RxOverrunInterruptEnable);

    NVIC_set_basepri_threshold(PRIORITY_7);
    NVIC_enable_interrupt_and_priority(UART0_IRQ, PRIORITY_2);
    NVIC_enable_interrupt_and_priority(UART4_IRQ, PRIORITY_2);
    NVIC_enable_interrupt_and_priority(PORTE_IRQ, PRIORITY_5);
        NVIC_enable_interrupt_and_priority(PIT_CH0_IRQ,PRIORITY_1);

    NVIC_global_enable_interrupts;

	GPIOE_callback_init(MANAGER_handler_flag);

    PIT_timer_period(PIT_CHANNEL_0, 10000);
    PIT_callback_init(PIT_CHANNEL_0, TRACKER_flag_on);
    PIT_start(PIT_CHANNEL_0);


    while (1)
    {
    	MANAGER_handler_log();
    	SM_state_machine();
    	TRACKER_handler();
    }
}

