/*
 * UART.c
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

#include "fsl_uart.h"
#include "fsl_port.h"
#include "UART.h"
#include "CONFIG.h"

uart_mail_box_t g_mail_box_uart_0 = {0, 0};
uart_mail_box_t g_mail_box_uart_4 ={0, 0};

void UART_init(){

	uart_config_t config;
	uint32_t uart0_clock;
	uint32_t uart4_clock;

	CLOCK_EnableClock(kCLOCK_PortB);
	CLOCK_EnableClock(kCLOCK_PortC);

	PORT_SetPinMux(PORTB, PIN16, kPORT_MuxAlt3);
	PORT_SetPinMux(PORTB, PIN17, kPORT_MuxAlt3);
	PORT_SetPinMux(PORTC, PIN14, kPORT_MuxAlt3);
	PORT_SetPinMux(PORTC, PIN15, kPORT_MuxAlt3);

	UART_GetDefaultConfig(&config);
	config.baudRate_Bps = UART_BAUDRATE;
	config.enableTx     = true;
	config.enableRx     = true;
	config.parityMode	= kUART_ParityEven;
	config.stopBitCount	= kUART_TwoStopBit;

	uart0_clock = CLOCK_GetFreq(UART0_CLK_SRC);
	uart4_clock = CLOCK_GetFreq(UART4_CLK_SRC);

	UART_Init(UART0, &config, uart0_clock);
	UART_Init(UART4, &config, uart4_clock);
}

void UART0_RX_TX_IRQHandler(void)
{
    /* If new data arrived. */
    if ((kUART_RxDataRegFullFlag | kUART_RxOverrunFlag) & UART_GetStatusFlags(UART0))
    {
    	g_mail_box_uart_0.mail_box = UART_ReadByte(UART0);
    	g_mail_box_uart_0.flag = true;
    }
    SDK_ISR_EXIT_BARRIER;
}
void UART4_RX_TX_IRQHandler(void)
{
    if ((kUART_RxDataRegFullFlag | kUART_RxOverrunFlag) & UART_GetStatusFlags(UART4))
    {
    	g_mail_box_uart_4.mail_box = UART_ReadByte(UART4);
    	g_mail_box_uart_4.flag = true;
    }
    SDK_ISR_EXIT_BARRIER;
}

uint8_t UART_get_mail_box(UART_Num_t uart) {

	uint8_t mail_box = 0;
    switch (uart) {
        case UART0_num:
            mail_box = g_mail_box_uart_0.mail_box;
            break;

        case UART4_num:
            mail_box = g_mail_box_uart_4.mail_box;
            break;

        default:
            break;
    }
    return mail_box;
}

uint8_t UART_get_mail_flag(UART_Num_t uart) {

	uint8_t flag = 0;
    switch (uart) {
        case UART0_num:
            flag = g_mail_box_uart_0.flag;
            break;

        case UART4_num:
            flag = g_mail_box_uart_4.flag;
            break;

        default:
            break;
    }
    return flag;
}

void UART_clear_mail_flag(UART_Num_t uart)
{
	switch (uart) {
		case UART0_num:
			g_mail_box_uart_0.flag = false;
			break;

		case UART4_num:
			g_mail_box_uart_4.flag = false;
			break;

		default:
			break;
	}
}
