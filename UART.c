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
#include "I2C.h"
#include "MANAGER.h"

#include <stdbool.h>

uint8_t g_vt100_hide_cursor[] = "\033[?25l";
uint8_t g_vt100_show_cursor[] = "\033[?25h";

/*VT100 command for clearing the screen*/
uint8_t g_vt100_clear[] = "\033[2J";
/** VT100 command for setting the background*/
uint8_t g_vt100_color_1[] = "\033[0;43m";
/** VT100 command for setting the foreground*/
uint8_t g_vt100_color_2[] = "\033[34m";
/** VT100 command for setting the background and foreground*/
uint8_t g_vt100_color_4[] = "\033[0;33;44m";
/** VT100 command for positioning the cursor in x and y position*/
uint8_t g_vt100_xy_1[] = "\033[11;23H";

uint8_t g_vt100_strg_options1[] =  "1) Configurar\r";
uint8_t g_vt100_strg_options2[] =  "2) Log\r";

uint8_t g_vt100_strg_log1[] =  "1) Activar Log\r";
uint8_t g_vt100_strg_log2[] =  "2) Leer Log\r";

uint8_t g_vt100_strg_config1[] =  "1) Establecer Hora\r";

/** VT100 command for positioning the cursor in x and y position*/
uint8_t g_vt100_xy_2[] = "\033[12;23H";
uint8_t g_vt100_strg_config2[] =  "2) Establecer Fecha\r";

uint8_t g_vt100_xy_3[] = "\033[13;23H";
uint8_t g_vt100_strg_config3[] =  "3) Leer Hora\r";

uint8_t g_vt100_xy_4[] = "\033[14;23H";
uint8_t g_vt100_strg_config4[] =  "4) Leer Fecha\r";

uint8_t g_vt100_xy_5[] = "\033[15;23H";
uint8_t g_vt100_strg_config5[] =  "5) Leer Temperatura\r";

uint8_t g_vt100_xy_6[] = "\033[16;23H";
uint8_t g_vt100_strg_config6[] =  "6) Leer Humedad\r";

//
uint8_t g_vt100_strg_time1[] =  "La hora actual es:\r";
uint8_t g_vt100_strg_time2[] =  "00:00:00\r";

uint8_t g_vt100_strg_date1[] =  "La fecha actual es:\r";
uint8_t g_vt100_strg_date2[] =  "00/00/0000\r";

uint8_t g_vt100_strg_hum1[] =  "La humedad relativa es:\r";
uint8_t g_vt100_strg_hum2[] =  "00.00%\r";

uint8_t g_vt100_strg_temp1[] =  "La temperatura es:\r";
uint8_t g_vt100_strg_temp2[] =  "00.00 C\r";

uint8_t g_vt100_strg_timeset1[] =  "Introducir la hora actual en formato HH:MM:SS.\r";
uint8_t g_vt100_strg_timeset2[] =  "La hora ha sido cambiada...\r";

uint8_t g_vt100_strg_dateset1[] =  "Escribir fecha: en dd/mm/aa:\r";
uint8_t g_vt100_strg_dateset2[] =  "La fecha ha sido cambiada...\r";

uint8_t g_vt100_strg_logset1[] =  "Cual log deseas activar: 1 a 5?\r";
uint8_t g_vt100_strg_logset2[] =  "Desear confirmar la activacion?: Si-->y No-->n\r";
uint8_t g_vt100_strg_logset3[] =  "Introducir tiempo de captura\r";
uint8_t g_vt100_strg_logset4[] =  "Donde t puede tomar valores entre 3s y 10s. No es necesario intruducir la letra s, solo el valor numerico\r";
uint8_t g_vt100_strg_logset5[] =  "Introducir numero de capturas\r";
uint8_t g_vt100_strg_logset6[] =  "Donde c puede tomar valores entre 10 y 50.\r";
uint8_t g_vt100_strg_logset7[] =  "El log se ha cancelado...\r";
uint8_t g_vt100_strg_logset8[] =  "El log se ha configurado...\r";

uint8_t g_vt100_strg_logread1[] =  "Cual log deseas leer: 1 a 5?\r";
uint8_t g_vt100_strg_logread2[] =  "Temperatura: 00.00 C, Humedad 00.00%, 00:00:00 hrs 00/00/2000\r";

uint8_t g_vt100_strg_jump1[] = "\033[B";

uint8_t g_vt100_xy_7[] = "\033[18;23H";

uint8_t g_vt100_strg_busy[] = "Este recurso esta siendo usado por la otra terminal\r";
uint8_t g_vt100_strg_busy_log[] = "Este log sigue activo\r";

uint8_t g_vt100_strg_error_wdt[] = "Reinicio debido a error en el sistema\r";
uint8_t g_vt100_strg_error_i2c[] = "ERROR: Sin comunicacion con dispositivo I2C\r";


uint8_t g_uart_rx_time[6] = {0,0,0,0,0,0};

uart_mail_box_t g_mail_box_uart_0 = {0, 0};
uart_mail_box_t g_mail_box_uart_4 ={0, 0};

uint8_t g_uart_rx_date[6] = {0,0,0,0,0,0};

uint8_t g_uart_rx_hum[4] = {0,0,0,0};
uint8_t g_uart_rx_temp[4] = {0,0,0,0};

uint8_t g_uart_log_num = 0;

uint8_t g_vt100_xy_0[] = "\033[14;H";

uint8_t g_uart_data_log_1[20];
uint8_t g_uart_data_log_2[20];
uint8_t g_uart_data_log_3[20];
uint8_t g_uart_data_log_4[20];
uint8_t g_uart_data_log_5[20];

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

	uart0_clock = CLOCK_GetFreq(UART0_CLK_SRC);
	uart4_clock = CLOCK_GetFreq(UART4_CLK_SRC);

	UART_Init(UART0, &config, uart0_clock);
	UART_Init(UART4, &config, uart4_clock);
}

void UART_show_options(UART_Type *uart)
{
	UART_WriteBlocking(uart, g_vt100_clear, sizeof(g_vt100_clear) / sizeof(g_vt100_clear[0]));
	UART_WriteBlocking(uart, g_vt100_color_1, sizeof(g_vt100_color_1) / sizeof(g_vt100_color_1[0]));
	UART_WriteBlocking(uart, g_vt100_clear, sizeof(g_vt100_clear) / sizeof(g_vt100_clear[0]));
	UART_WriteBlocking(uart, g_vt100_xy_1, sizeof(g_vt100_xy_1) / sizeof(g_vt100_xy_1[0]));
	UART_WriteBlocking(uart, g_vt100_color_2, sizeof(g_vt100_color_2) / sizeof(g_vt100_color_2[0]));
	UART_WriteBlocking(uart, g_vt100_strg_options1, sizeof(g_vt100_strg_options1) / sizeof(g_vt100_strg_options1[0]));
	UART_WriteBlocking(uart, g_vt100_xy_2, sizeof(g_vt100_xy_2) / sizeof(g_vt100_xy_2[0]));
	UART_WriteBlocking(uart, g_vt100_strg_options2, sizeof(g_vt100_strg_options2) / sizeof(g_vt100_strg_options2[0]));
	UART_WriteBlocking(uart, g_vt100_hide_cursor, sizeof(g_vt100_hide_cursor) / sizeof(g_vt100_hide_cursor[0]));
}

void UART_show_log(UART_Type *uart)
{
	UART_WriteBlocking(uart, g_vt100_clear, sizeof(g_vt100_clear) / sizeof(g_vt100_clear[0]));
	UART_WriteBlocking(uart, g_vt100_color_1, sizeof(g_vt100_color_1) / sizeof(g_vt100_color_1[0]));
	UART_WriteBlocking(uart, g_vt100_clear, sizeof(g_vt100_clear) / sizeof(g_vt100_clear[0]));
	UART_WriteBlocking(uart, g_vt100_xy_1, sizeof(g_vt100_xy_1) / sizeof(g_vt100_xy_1[0]));
	UART_WriteBlocking(uart, g_vt100_color_2, sizeof(g_vt100_color_2) / sizeof(g_vt100_color_2[0]));
	UART_WriteBlocking(uart, g_vt100_strg_log1, sizeof(g_vt100_strg_log1) / sizeof(g_vt100_strg_log1[0]));
	UART_WriteBlocking(uart, g_vt100_xy_2, sizeof(g_vt100_xy_2) / sizeof(g_vt100_xy_2[0]));
	UART_WriteBlocking(uart, g_vt100_strg_log2, sizeof(g_vt100_strg_log2) / sizeof(g_vt100_strg_log2[0]));
}

void UART_show_config(UART_Type *uart){
	UART_WriteBlocking(uart, g_vt100_clear, sizeof(g_vt100_clear) / sizeof(g_vt100_clear[0]));
	UART_WriteBlocking(uart, g_vt100_color_1, sizeof(g_vt100_color_1) / sizeof(g_vt100_color_1[0]));
	UART_WriteBlocking(uart, g_vt100_clear, sizeof(g_vt100_clear) / sizeof(g_vt100_clear[0]));
	UART_WriteBlocking(uart, g_vt100_xy_1, sizeof(g_vt100_xy_1) / sizeof(g_vt100_xy_1[0]));
	UART_WriteBlocking(uart, g_vt100_color_2, sizeof(g_vt100_color_2) / sizeof(g_vt100_color_2[0]));
	UART_WriteBlocking(uart, g_vt100_strg_config1, sizeof(g_vt100_strg_config1) / sizeof(g_vt100_strg_config1[0]));
	UART_WriteBlocking(uart, g_vt100_xy_2, sizeof(g_vt100_xy_2) / sizeof(g_vt100_xy_2[0]));
	UART_WriteBlocking(uart, g_vt100_strg_config2, sizeof(g_vt100_strg_config2) / sizeof(g_vt100_strg_config2[0]));

	UART_WriteBlocking(uart, g_vt100_xy_3, sizeof(g_vt100_xy_3) / sizeof(g_vt100_xy_3[0]));
	UART_WriteBlocking(uart, g_vt100_strg_config3, sizeof(g_vt100_strg_config3) / sizeof(g_vt100_strg_config3[0]));
	UART_WriteBlocking(uart, g_vt100_xy_4, sizeof(g_vt100_xy_4) / sizeof(g_vt100_xy_4[0]));
	UART_WriteBlocking(uart, g_vt100_strg_config4, sizeof(g_vt100_strg_config4) / sizeof(g_vt100_strg_config4[0]));


	UART_WriteBlocking(uart, g_vt100_color_4, sizeof(g_vt100_color_4) / sizeof(g_vt100_color_4[0]));
}

void UART_show_time(UART_Type *uart){
	UART_WriteBlocking(uart, g_vt100_clear, sizeof(g_vt100_clear) / sizeof(g_vt100_clear[0]));
	UART_WriteBlocking(uart, g_vt100_color_1, sizeof(g_vt100_color_1) / sizeof(g_vt100_color_1[0]));
	UART_WriteBlocking(uart, g_vt100_clear, sizeof(g_vt100_clear) / sizeof(g_vt100_clear[0]));
	UART_WriteBlocking(uart, g_vt100_xy_1, sizeof(g_vt100_xy_1) / sizeof(g_vt100_xy_1[0]));
	UART_WriteBlocking(uart, g_vt100_color_2, sizeof(g_vt100_color_2) / sizeof(g_vt100_color_2[0]));
	UART_WriteBlocking(uart, g_vt100_strg_time1, sizeof(g_vt100_strg_time1) / sizeof(g_vt100_strg_time1[0]));
	UART_WriteBlocking(uart, g_vt100_xy_2, sizeof(g_vt100_xy_2) / sizeof(g_vt100_xy_2[0]));
	UART_WriteBlocking(uart, g_vt100_strg_time2, sizeof(g_vt100_strg_time2) / sizeof(g_vt100_strg_time2[0]));
	UART_WriteBlocking(uart, g_vt100_color_4, sizeof(g_vt100_color_4) / sizeof(g_vt100_color_4[0]));
}

void UART_show_date(UART_Type *uart){
	UART_WriteBlocking(uart, g_vt100_clear, sizeof(g_vt100_clear) / sizeof(g_vt100_clear[0]));
	UART_WriteBlocking(uart, g_vt100_color_1, sizeof(g_vt100_color_1) / sizeof(g_vt100_color_1[0]));
	UART_WriteBlocking(uart, g_vt100_clear, sizeof(g_vt100_clear) / sizeof(g_vt100_clear[0]));
	UART_WriteBlocking(uart, g_vt100_xy_1, sizeof(g_vt100_xy_1) / sizeof(g_vt100_xy_1[0]));
	UART_WriteBlocking(uart, g_vt100_color_2, sizeof(g_vt100_color_2) / sizeof(g_vt100_color_2[0]));
	UART_WriteBlocking(uart, g_vt100_strg_date1, sizeof(g_vt100_strg_date1) / sizeof(g_vt100_strg_date1[0]));
	UART_WriteBlocking(uart, g_vt100_xy_2, sizeof(g_vt100_xy_2) / sizeof(g_vt100_xy_2[0]));
	UART_WriteBlocking(uart, g_vt100_strg_date2, sizeof( g_vt100_strg_date2) / sizeof( g_vt100_strg_date2[0]));
	UART_WriteBlocking(uart, g_vt100_color_4, sizeof(g_vt100_color_4) / sizeof(g_vt100_color_4[0]));
}

void UART_show_hum(UART_Type *uart){
	UART_WriteBlocking(uart, g_vt100_clear, sizeof(g_vt100_clear) / sizeof(g_vt100_clear[0]));
	UART_WriteBlocking(uart, g_vt100_color_1, sizeof(g_vt100_color_1) / sizeof(g_vt100_color_1[0]));
	UART_WriteBlocking(uart, g_vt100_clear, sizeof(g_vt100_clear) / sizeof(g_vt100_clear[0]));
	UART_WriteBlocking(uart, g_vt100_xy_1, sizeof(g_vt100_xy_1) / sizeof(g_vt100_xy_1[0]));
	UART_WriteBlocking(uart, g_vt100_color_2, sizeof(g_vt100_color_2) / sizeof(g_vt100_color_2[0]));
	UART_WriteBlocking(uart, g_vt100_strg_hum1, sizeof(g_vt100_strg_hum1) / sizeof(g_vt100_strg_hum1[0]));
	UART_WriteBlocking(uart, g_vt100_xy_2, sizeof(g_vt100_xy_2) / sizeof(g_vt100_xy_2[0]));
	UART_WriteBlocking(uart, g_vt100_strg_hum2, sizeof(g_vt100_strg_hum2) / sizeof(g_vt100_strg_hum2[0]));
	UART_WriteBlocking(uart, g_vt100_color_4, sizeof(g_vt100_color_4) / sizeof(g_vt100_color_4[0]));
}

void UART_show_temp(UART_Type *uart){
	UART_WriteBlocking(uart, g_vt100_clear, sizeof(g_vt100_clear) / sizeof(g_vt100_clear[0]));
	UART_WriteBlocking(uart, g_vt100_color_1, sizeof(g_vt100_color_1) / sizeof(g_vt100_color_1[0]));
	UART_WriteBlocking(uart, g_vt100_clear, sizeof(g_vt100_clear) / sizeof(g_vt100_clear[0]));
	UART_WriteBlocking(uart, g_vt100_xy_1, sizeof(g_vt100_xy_1) / sizeof(g_vt100_xy_1[0]));
	UART_WriteBlocking(uart, g_vt100_color_2, sizeof(g_vt100_color_2) / sizeof(g_vt100_color_2[0]));
	UART_WriteBlocking(uart, g_vt100_strg_temp1, sizeof(g_vt100_strg_temp1) / sizeof(g_vt100_strg_temp1[0]));
	UART_WriteBlocking(uart, g_vt100_xy_2, sizeof(g_vt100_xy_2) / sizeof(g_vt100_xy_2[0]));
	UART_WriteBlocking(uart, g_vt100_strg_temp2, sizeof(g_vt100_strg_temp2) / sizeof(g_vt100_strg_temp2[0]));
	UART_WriteBlocking(uart, g_vt100_color_4, sizeof(g_vt100_color_4) / sizeof(g_vt100_color_4[0]));
}

void UART_change_seconds(UART_Type *uart)
{
	UART_WriteBlocking(uart, g_vt100_color_1, sizeof(g_vt100_color_1) / sizeof(g_vt100_color_1[0]));
	UART_WriteBlocking(uart, g_vt100_color_2, sizeof(g_vt100_color_2) / sizeof(g_vt100_color_2[0]));
	UART_WriteBlocking(uart, g_vt100_xy_2, sizeof(g_vt100_xy_2) / sizeof(g_vt100_xy_2[0]));
	UART_WriteBlocking(uart, g_vt100_strg_time2, sizeof(g_vt100_strg_time2) / sizeof(g_vt100_strg_time2[0]));
}


void UART_show_set_time(UART_Type *uart){
	UART_WriteBlocking(uart, g_vt100_clear, sizeof(g_vt100_clear) / sizeof(g_vt100_clear[0]));
	UART_WriteBlocking(uart, g_vt100_color_1, sizeof(g_vt100_color_1) / sizeof(g_vt100_color_1[0]));
	UART_WriteBlocking(uart, g_vt100_clear, sizeof(g_vt100_clear) / sizeof(g_vt100_clear[0]));
	UART_WriteBlocking(uart, g_vt100_xy_1, sizeof(g_vt100_xy_1) / sizeof(g_vt100_xy_1[0]));
	UART_WriteBlocking(uart, g_vt100_color_2, sizeof(g_vt100_color_2) / sizeof(g_vt100_color_2[0]));
	UART_WriteBlocking(uart, g_vt100_strg_timeset1, sizeof(g_vt100_strg_timeset1) / sizeof(g_vt100_strg_time1[0]));
	UART_WriteBlocking(uart, g_vt100_xy_2, sizeof(g_vt100_xy_2) / sizeof(g_vt100_xy_2[0]));
	UART_WriteBlocking(uart, g_vt100_show_cursor, sizeof(g_vt100_show_cursor) / sizeof(g_vt100_show_cursor[0]));

}

void UART_msg_set_time(UART_Type *uart)
{
	UART_WriteBlocking(uart, g_vt100_xy_3, sizeof(g_vt100_xy_3) / sizeof(g_vt100_xy_3[0]));
	UART_WriteBlocking(uart, g_vt100_strg_timeset2, sizeof(g_vt100_strg_timeset2) / sizeof(g_vt100_strg_timeset2[0]));
	UART_WriteBlocking(uart, g_vt100_hide_cursor, sizeof(g_vt100_hide_cursor) / sizeof(g_vt100_hide_cursor[0]));
}

void UART_msg_set_date(UART_Type *uart)
{
	UART_WriteBlocking(uart, g_vt100_xy_3, sizeof(g_vt100_xy_3) / sizeof(g_vt100_xy_3[0]));
	UART_WriteBlocking(uart, g_vt100_strg_dateset2, sizeof(g_vt100_strg_dateset2) / sizeof(g_vt100_strg_dateset2[0]));
	UART_WriteBlocking(uart, g_vt100_hide_cursor, sizeof(g_vt100_hide_cursor) / sizeof(g_vt100_hide_cursor[0]));
}

void UART_show_set_date(UART_Type *uart){
	UART_WriteBlocking(uart, g_vt100_clear, sizeof(g_vt100_clear) / sizeof(g_vt100_clear[0]));
	UART_WriteBlocking(uart, g_vt100_color_1, sizeof(g_vt100_color_1) / sizeof(g_vt100_color_1[0]));
	UART_WriteBlocking(uart, g_vt100_clear, sizeof(g_vt100_clear) / sizeof(g_vt100_clear[0]));
	UART_WriteBlocking(uart, g_vt100_xy_1, sizeof(g_vt100_xy_1) / sizeof(g_vt100_xy_1[0]));
	UART_WriteBlocking(uart, g_vt100_color_2, sizeof(g_vt100_color_2) / sizeof(g_vt100_color_2[0]));
	UART_WriteBlocking(uart, g_vt100_strg_dateset1, sizeof(g_vt100_strg_dateset1) / sizeof(g_vt100_strg_dateset1[0]));
	UART_WriteBlocking(uart, g_vt100_xy_2, sizeof(g_vt100_xy_2) / sizeof(g_vt100_xy_2[0]));
	UART_WriteBlocking(uart, g_vt100_show_cursor, sizeof(g_vt100_show_cursor) / sizeof(g_vt100_show_cursor[0]));
}

void UART_show_set_log(UART_Type *uart)
{
	UART_WriteBlocking(uart, g_vt100_clear, sizeof(g_vt100_clear) / sizeof(g_vt100_clear[0]));
	UART_WriteBlocking(uart, g_vt100_color_1, sizeof(g_vt100_color_1) / sizeof(g_vt100_color_1[0]));
	UART_WriteBlocking(uart, g_vt100_clear, sizeof(g_vt100_clear) / sizeof(g_vt100_clear[0]));
	UART_WriteBlocking(uart, g_vt100_xy_1, sizeof(g_vt100_xy_1) / sizeof(g_vt100_xy_1[0]));
	UART_WriteBlocking(uart, g_vt100_color_2, sizeof(g_vt100_color_2) / sizeof(g_vt100_color_2[0]));
	UART_WriteBlocking(uart, g_vt100_strg_logset1, sizeof(g_vt100_strg_logset1) / sizeof(g_vt100_strg_logset1[0]));
	UART_WriteBlocking(uart, g_vt100_xy_2, sizeof(g_vt100_xy_2) / sizeof(g_vt100_xy_2[0]));
	UART_WriteBlocking(uart, g_vt100_show_cursor, sizeof(g_vt100_show_cursor) / sizeof(g_vt100_show_cursor[0]));
}

void UART_msg_set_log_confirm(UART_Type *uart)
{
	UART_WriteBlocking(uart, g_vt100_xy_3, sizeof(g_vt100_xy_3) / sizeof(g_vt100_xy_3[0]));
	UART_WriteBlocking(uart, g_vt100_strg_logset2, sizeof(g_vt100_strg_logset2) / sizeof(g_vt100_strg_logset2[0]));
	UART_WriteBlocking(uart, g_vt100_xy_4, sizeof(g_vt100_xy_4) / sizeof(g_vt100_xy_4[0]));
	UART_WriteBlocking(uart, g_vt100_show_cursor, sizeof(g_vt100_show_cursor) / sizeof(g_vt100_show_cursor[0]));
}

void UART_show_set_log_time(UART_Type *uart)
{
	UART_WriteBlocking(uart, g_vt100_clear, sizeof(g_vt100_clear) / sizeof(g_vt100_clear[0]));
	UART_WriteBlocking(uart, g_vt100_color_1, sizeof(g_vt100_color_1) / sizeof(g_vt100_color_1[0]));
	UART_WriteBlocking(uart, g_vt100_clear, sizeof(g_vt100_clear) / sizeof(g_vt100_clear[0]));
	UART_WriteBlocking(uart, g_vt100_xy_1, sizeof(g_vt100_xy_1) / sizeof(g_vt100_xy_1[0]));
	UART_WriteBlocking(uart, g_vt100_color_2, sizeof(g_vt100_color_2) / sizeof(g_vt100_color_2[0]));
	UART_WriteBlocking(uart, g_vt100_strg_logset3, sizeof(g_vt100_strg_logset3) / sizeof(g_vt100_strg_logset3[0]));
	UART_WriteBlocking(uart, g_vt100_xy_4, sizeof(g_vt100_xy_4) / sizeof(g_vt100_xy_4[0]));
	UART_WriteBlocking(uart, g_vt100_strg_logset4, sizeof(g_vt100_strg_logset4) / sizeof(g_vt100_strg_logset4[0]));
	UART_WriteBlocking(uart, g_vt100_xy_2, sizeof(g_vt100_xy_2) / sizeof(g_vt100_xy_2[0]));

	UART_WriteBlocking(uart, g_vt100_show_cursor, sizeof(g_vt100_show_cursor) / sizeof(g_vt100_show_cursor[0]));
}

void UART_show_set_log_cap(UART_Type *uart)
{
	UART_WriteBlocking(uart, g_vt100_clear, sizeof(g_vt100_clear) / sizeof(g_vt100_clear[0]));
	UART_WriteBlocking(uart, g_vt100_color_1, sizeof(g_vt100_color_1) / sizeof(g_vt100_color_1[0]));
	UART_WriteBlocking(uart, g_vt100_clear, sizeof(g_vt100_clear) / sizeof(g_vt100_clear[0]));
	UART_WriteBlocking(uart, g_vt100_xy_1, sizeof(g_vt100_xy_1) / sizeof(g_vt100_xy_1[0]));
	UART_WriteBlocking(uart, g_vt100_color_2, sizeof(g_vt100_color_2) / sizeof(g_vt100_color_2[0]));
	UART_WriteBlocking(uart, g_vt100_strg_logset5, sizeof(g_vt100_strg_logset5) / sizeof(g_vt100_strg_logset5[0]));
	UART_WriteBlocking(uart, g_vt100_xy_3, sizeof(g_vt100_xy_3) / sizeof(g_vt100_xy_3[0]));
	UART_WriteBlocking(uart, g_vt100_strg_logset6, sizeof(g_vt100_strg_logset6) / sizeof(g_vt100_strg_logset6[0]));
	UART_WriteBlocking(uart, g_vt100_xy_2, sizeof(g_vt100_xy_2) / sizeof(g_vt100_xy_2[0]));

	UART_WriteBlocking(uart, g_vt100_show_cursor, sizeof(g_vt100_show_cursor) / sizeof(g_vt100_show_cursor[0]));
}

void UART_msg_set_log_done(UART_Type *uart)
{
	UART_WriteBlocking(uart, g_vt100_xy_5, sizeof(g_vt100_xy_5) / sizeof(g_vt100_xy_5[0]));
	UART_WriteBlocking(uart, g_vt100_strg_logset8, sizeof(g_vt100_strg_logset8) / sizeof(g_vt100_strg_logset8[0]));
	UART_WriteBlocking(uart, g_vt100_hide_cursor, sizeof(g_vt100_hide_cursor) / sizeof(g_vt100_hide_cursor[0]));
}

void UART_msg_set_log_cancel(UART_Type *uart)
{
	UART_WriteBlocking(uart, g_vt100_xy_5, sizeof(g_vt100_xy_5) / sizeof(g_vt100_xy_5[0]));
	UART_WriteBlocking(uart, g_vt100_strg_logset7, sizeof(g_vt100_strg_logset7) / sizeof(g_vt100_strg_logset2[0]));
	UART_WriteBlocking(uart, g_vt100_hide_cursor, sizeof(g_vt100_hide_cursor) / sizeof(g_vt100_hide_cursor[0]));
}
void UART_show_read_log(UART_Type *uart)
{
	UART_WriteBlocking(uart, g_vt100_clear, sizeof(g_vt100_clear) / sizeof(g_vt100_clear[0]));
	UART_WriteBlocking(uart, g_vt100_color_1, sizeof(g_vt100_color_1) / sizeof(g_vt100_color_1[0]));
	UART_WriteBlocking(uart, g_vt100_clear, sizeof(g_vt100_clear) / sizeof(g_vt100_clear[0]));
	UART_WriteBlocking(uart, g_vt100_xy_1, sizeof(g_vt100_xy_1) / sizeof(g_vt100_xy_1[0]));
	UART_WriteBlocking(uart, g_vt100_color_2, sizeof(g_vt100_color_2) / sizeof(g_vt100_color_2[0]));
	UART_WriteBlocking(uart, g_vt100_strg_logread1, sizeof(g_vt100_strg_logread1) / sizeof(g_vt100_strg_logread1[0]));
	UART_WriteBlocking(uart, g_vt100_xy_2, sizeof(g_vt100_xy_2) / sizeof(g_vt100_xy_2[0]));
	UART_WriteBlocking(uart, g_vt100_show_cursor, sizeof(g_vt100_show_cursor) / sizeof(g_vt100_show_cursor[0]));
}

void UART_msg_busy(UART_Type *uart){
	UART_WriteBlocking(uart, g_vt100_color_1, sizeof(g_vt100_color_1) / sizeof(g_vt100_color_1[0]));
	UART_WriteBlocking(uart, g_vt100_xy_7, sizeof(g_vt100_xy_7) / sizeof(g_vt100_xy_7[0]));
	UART_WriteBlocking(uart, g_vt100_color_2, sizeof(g_vt100_color_2) / sizeof(g_vt100_color_2[0]));
	UART_WriteBlocking(uart, g_vt100_strg_busy, sizeof(g_vt100_strg_busy) / sizeof(g_vt100_strg_busy[0]));
}

void UART_msg_busy_log(UART_Type *uart)
{
	UART_WriteBlocking(uart, g_vt100_xy_5, sizeof(g_vt100_xy_5) / sizeof(g_vt100_xy_5[0]));
	UART_WriteBlocking(uart, g_vt100_strg_busy_log, sizeof(g_vt100_strg_busy_log) / sizeof(g_vt100_strg_busy_log[0]));
	UART_WriteBlocking(uart, g_vt100_hide_cursor, sizeof(g_vt100_hide_cursor) / sizeof(g_vt100_hide_cursor[0]));
}


void UART_update_time() {

	char formattedString[TIME_STRING_LENGHT];

	// Convert tens and units to characters
	formattedString[0] = CHAR_TO_INT + g_uart_rx_time[0]; // Tens of hours
	formattedString[1] = CHAR_TO_INT + g_uart_rx_time[1]; // Units of hours
	formattedString[2] = COLON;
	formattedString[3] = CHAR_TO_INT + g_uart_rx_time[2]; // Tens of minutes
	formattedString[4] = CHAR_TO_INT + g_uart_rx_time[3]; // Units of minutes
	formattedString[5] = COLON;
	formattedString[6] = CHAR_TO_INT + g_uart_rx_time[4]; // Tens of seconds
	formattedString[7] = CHAR_TO_INT + g_uart_rx_time[5]; // Units of seconds
	formattedString[8] = '\0';

	// Copy the characters one by one to g_vt100_strg_time2
	for (int i = 0; i <TIME_STRING_LENGHT ; i++) {
	    g_vt100_strg_time2[i] = formattedString[i];
	}
}


void UART_update_date() {

	char formattedString[DATE_STRING_LENGHT];

	// Convert tens and units to characters
	formattedString[0] = CHAR_TO_INT + g_uart_rx_date[0]; // Tens of days
	formattedString[1] = CHAR_TO_INT + g_uart_rx_date[1]; // Units of days
	formattedString[2] = SLASH;
	formattedString[3] = CHAR_TO_INT + g_uart_rx_date[2]; // Tens of month
	formattedString[4] = CHAR_TO_INT + g_uart_rx_date[3]; // Units of month
	formattedString[5] = SLASH;
	formattedString[6] = '2';
	formattedString[7] = CHAR_TO_INT ;
	formattedString[8] = CHAR_TO_INT + g_uart_rx_date[4]; // Tens of year
	formattedString[9] = CHAR_TO_INT + g_uart_rx_date[5]; // Units of year

	formattedString[10] = '\0';

	// Copy the characters one by one to g_vt100_strg_date2
	for (int i = 0; i <DATE_STRING_LENGHT ; i++) {
		g_vt100_strg_date2[i] = formattedString[i];
	}
}

void UART_update_hum() {

	char formattedString[HUM_STRING_LENGHT];

	formattedString[0] = CHAR_TO_INT + g_uart_rx_hum[0];
	formattedString[1] = CHAR_TO_INT + g_uart_rx_hum[1];
	formattedString[2] = '.';
	formattedString[3] = CHAR_TO_INT + g_uart_rx_hum[2];
	formattedString[4] = CHAR_TO_INT + g_uart_rx_hum[3];
	formattedString[5] = '%';
	formattedString[6] = '\0';

	for (int i = 0; i <HUM_STRING_LENGHT ; i++) {
	    g_vt100_strg_hum2[i] = formattedString[i];
	}
}

void UART_update_temp() {

	char formattedString[TEMP_STRING_LENGHT];

	formattedString[0] = CHAR_TO_INT + g_uart_rx_temp[0];
	formattedString[1] = CHAR_TO_INT + g_uart_rx_temp[1];
	formattedString[2] = '.';
	formattedString[3] = CHAR_TO_INT + g_uart_rx_temp[2];
	formattedString[4] = CHAR_TO_INT + g_uart_rx_temp[3];
	formattedString[5] = ' ';
	formattedString[6] = 'C';
	formattedString[7] = '\0';

	for (int i = 0; i <TEMP_STRING_LENGHT ; i++) {
	    g_vt100_strg_temp2[i] = formattedString[i];
	}
}

uint8_t* UART_get_rx_time() {
    return g_uart_rx_time;
}

uint8_t* UART_get_rx_date() {
    return g_uart_rx_date;
}

uint8_t* UART_get_rx_hum() {
    return g_uart_rx_hum;
}

uint8_t* UART_get_rx_temp() {
    return g_uart_rx_temp;
}

uint8_t UART_get_log_num() {
    return g_uart_log_num;
}

void UART_set_log_num(uint8_t log) {
    g_uart_log_num = log;
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

uint8_t* UART_get_data_log(uint8_t log)
{
    switch (log) {
        case 1:
            return g_uart_data_log_1;
        case 2:
            return g_uart_data_log_2;
        case 3:
            return g_uart_data_log_3;
        case 4:
            return g_uart_data_log_4;
        case 5:
            return g_uart_data_log_5;
        default:
            return NULL;
    }
}

void UART_show_data_log(uint8_t log, UART_Type *uart)
{
	uint8_t* uart_data_log = UART_get_data_log(log);

	g_vt100_strg_logread2[13] = uart_data_log[0]+CHAR_TO_INT;
	g_vt100_strg_logread2[14] = uart_data_log[1]+CHAR_TO_INT;
	g_vt100_strg_logread2[16] = uart_data_log[2]+CHAR_TO_INT;
	g_vt100_strg_logread2[17] = uart_data_log[3]+CHAR_TO_INT;
	g_vt100_strg_logread2[30] = uart_data_log[4]+CHAR_TO_INT;
	g_vt100_strg_logread2[31] = uart_data_log[5]+CHAR_TO_INT;
	g_vt100_strg_logread2[33] = uart_data_log[6]+CHAR_TO_INT;
	g_vt100_strg_logread2[34] = uart_data_log[7]+CHAR_TO_INT;
	g_vt100_strg_logread2[38] = uart_data_log[8]+CHAR_TO_INT;
	g_vt100_strg_logread2[39] = uart_data_log[9]+CHAR_TO_INT;
	g_vt100_strg_logread2[41] = uart_data_log[10]+CHAR_TO_INT;
	g_vt100_strg_logread2[42] = uart_data_log[11]+CHAR_TO_INT;
	g_vt100_strg_logread2[44] = uart_data_log[12]+CHAR_TO_INT;
	g_vt100_strg_logread2[45] = uart_data_log[13]+CHAR_TO_INT;
	g_vt100_strg_logread2[51] = uart_data_log[14]+CHAR_TO_INT;
	g_vt100_strg_logread2[52] = uart_data_log[15]+CHAR_TO_INT;
	g_vt100_strg_logread2[54] = uart_data_log[16]+CHAR_TO_INT;
	g_vt100_strg_logread2[55] = uart_data_log[17]+CHAR_TO_INT;
	g_vt100_strg_logread2[59] = uart_data_log[18]+CHAR_TO_INT;
	g_vt100_strg_logread2[60] = uart_data_log[19]+CHAR_TO_INT;

	UART_WriteBlocking(uart, g_vt100_hide_cursor, sizeof(g_vt100_hide_cursor) / sizeof(g_vt100_hide_cursor[0]));
	UART_WriteBlocking(uart, g_vt100_strg_jump1, sizeof(g_vt100_strg_jump1) / sizeof(g_vt100_strg_jump1[0]));
	UART_WriteBlocking(uart, g_vt100_color_2, sizeof(g_vt100_color_2) / sizeof(g_vt100_color_2[0]));
	UART_WriteBlocking(uart, g_vt100_strg_logread2, sizeof(g_vt100_strg_logread2) / sizeof(g_vt100_strg_logread2[0]));
}

void UART_show_data_log_jump(UART_Type *uart)
{
	UART_WriteBlocking(uart, g_vt100_xy_0, sizeof(g_vt100_xy_0) / sizeof(g_vt100_xy_0[0]));
}

void UART_show_error_wdt(UART_Type *uart)
{
	UART_WriteBlocking(uart, g_vt100_color_1, sizeof(g_vt100_color_1) / sizeof(g_vt100_color_1[0]));
	UART_WriteBlocking(uart, g_vt100_xy_7, sizeof(g_vt100_xy_7) / sizeof(g_vt100_xy_7[0]));
	UART_WriteBlocking(uart, g_vt100_color_2, sizeof(g_vt100_color_2) / sizeof(g_vt100_color_2[0]));
	UART_WriteBlocking(uart, g_vt100_strg_error_wdt, sizeof(g_vt100_strg_error_wdt) / sizeof(g_vt100_strg_error_wdt[0]));
	UART_WriteBlocking(uart, g_vt100_hide_cursor, sizeof(g_vt100_hide_cursor) / sizeof(g_vt100_hide_cursor[0]));
}

void UART_show_error_i2c(UART_Type *uart)
{
	UART_WriteBlocking(uart, g_vt100_color_1, sizeof(g_vt100_color_1) / sizeof(g_vt100_color_1[0]));
	UART_WriteBlocking(uart, g_vt100_xy_7, sizeof(g_vt100_xy_7) / sizeof(g_vt100_xy_7[0]));
	UART_WriteBlocking(uart, g_vt100_color_2, sizeof(g_vt100_color_2) / sizeof(g_vt100_color_2[0]));
	UART_WriteBlocking(uart, g_vt100_strg_error_i2c, sizeof(g_vt100_strg_error_i2c) / sizeof(g_vt100_strg_error_i2c[0]));
	UART_WriteBlocking(uart, g_vt100_hide_cursor, sizeof(g_vt100_hide_cursor) / sizeof(g_vt100_hide_cursor[0]));
}


