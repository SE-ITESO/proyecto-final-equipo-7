/*
 * TERMINAL.c
 *
 *  Created on: 20 nov. 2023
 *      Author: HP}
 */

#include "TERMINAL.h"
#include "CONFIG.h"
#include "UART.h"
#include "fsl_uart.h"

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
uint8_t g_vt100_xy_2[] = "\033[12;23H";
uint8_t g_vt100_xy_3[] = "\033[13;23H";
uint8_t g_vt100_xy_4[] = "\033[14;23H";
uint8_t g_vt100_xy_5[] = "\033[15;23H";
uint8_t g_vt100_xy_7[] = "\033[18;23H";
uint8_t g_vt100_xy_jump1[] = "\033[B";

uint8_t g_vt100_strg_options1[] =  "1) Configurar\r";
uint8_t g_vt100_strg_options2[] =  "2) Log\r";

uint8_t g_vt100_strg_log1[] =  "1) Activar Log\r";
uint8_t g_vt100_strg_log2[] =  "2) Leer Log\r";

uint8_t g_vt100_strg_config1[] =  "1) Establecer Hora\r";
uint8_t g_vt100_strg_config2[] =  "2) Establecer Fecha\r";
uint8_t g_vt100_strg_config3[] =  "3) Leer Hora\r";
uint8_t g_vt100_strg_config4[] =  "4) Leer Fecha\r";
uint8_t g_vt100_strg_config5[] =  "5) Leer Corriente\r";

uint8_t g_vt100_strg_time1[] =  "La hora actual es:\r";
uint8_t g_vt100_strg_time2[] =  "00:00:00\r";

uint8_t g_vt100_strg_date1[] =  "La fecha actual es:\r";
uint8_t g_vt100_strg_date2[] =  "00/00/0000\r";

uint8_t g_vt100_strg_curr1[] = "Corriente actual:\r";
uint8_t g_vt100_strg_curr2[] = "00.0 mA";

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
uint8_t g_vt100_strg_logread2[] =  "Corriente: 00.00 mA, Hora: 00:00:00, Fecha: 00/00/2000\r";

uint8_t g_vt100_strg_busy[] = "Este recurso esta siendo usado por la otra terminal\r";
uint8_t g_vt100_strg_busy_log[] = "Este log sigue activo\r";

uint8_t g_vt100_strg_error_wdt[] = "Reinicio debido a error en el sistema\r";
uint8_t g_vt100_strg_error_i2c[] = "ERROR: Sin comunicacion con dispositivo I2C\r";

uint8_t g_terminal_time[TIME_FORMAT_LENGTH] = {0,0,0,0,0,0};
uint8_t g_terminal_date[DATE_FORMAT_LENGHT] = {0,0,0,0,0,0};
uint8_t g_terminal_curr[CURR_FORMAT_LENGHT] = {0,0,0,0};

uint8_t g_terminal_log_num = 0;

uint8_t g_vt100_xy_0[] = "\033[14;H";

uint8_t g_terminal_data_log_1[LOG_DATA_FORMAT_LENGHT];
uint8_t g_terminal_data_log_2[LOG_DATA_FORMAT_LENGHT];
uint8_t g_terminal_data_log_3[LOG_DATA_FORMAT_LENGHT];
uint8_t g_terminal_data_log_4[LOG_DATA_FORMAT_LENGHT];
uint8_t g_terminal_data_log_5[LOG_DATA_FORMAT_LENGHT];


void TERMINAL_show_options(UART_Type *uart)
{
    UART_WriteBlocking(uart, g_vt100_clear, sizeof(g_vt100_clear));
    UART_WriteBlocking(uart, g_vt100_color_1, sizeof(g_vt100_color_1));
    UART_WriteBlocking(uart, g_vt100_clear, sizeof(g_vt100_clear));
    UART_WriteBlocking(uart, g_vt100_xy_1, sizeof(g_vt100_xy_1));
    UART_WriteBlocking(uart, g_vt100_color_2, sizeof(g_vt100_color_2));
    UART_WriteBlocking(uart, g_vt100_strg_options1, sizeof(g_vt100_strg_options1));
    UART_WriteBlocking(uart, g_vt100_xy_2, sizeof(g_vt100_xy_2));
    UART_WriteBlocking(uart, g_vt100_strg_options2, sizeof(g_vt100_strg_options2));
    UART_WriteBlocking(uart, g_vt100_hide_cursor, sizeof(g_vt100_hide_cursor));
}

void TERMINAL_show_log(UART_Type *uart)
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

void TERMINAL_show_config(UART_Type *uart){
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

	UART_WriteBlocking(uart, g_vt100_xy_5, sizeof(g_vt100_xy_5) / sizeof(g_vt100_xy_5[0]));
	UART_WriteBlocking(uart, g_vt100_strg_config5, sizeof(g_vt100_strg_config5) / sizeof(g_vt100_strg_config5[0]));

	UART_WriteBlocking(uart, g_vt100_color_4, sizeof(g_vt100_color_4) / sizeof(g_vt100_color_4[0]));
}

void TERMINAL_show_time(UART_Type *uart){
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

void TERMINAL_show_date(UART_Type *uart){
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

void TERMINAL_show_curr(UART_Type *uart){
	UART_WriteBlocking(uart, g_vt100_clear, sizeof(g_vt100_clear) / sizeof(g_vt100_clear[0]));
	UART_WriteBlocking(uart, g_vt100_color_1, sizeof(g_vt100_color_1) / sizeof(g_vt100_color_1[0]));
	UART_WriteBlocking(uart, g_vt100_clear, sizeof(g_vt100_clear) / sizeof(g_vt100_clear[0]));
	UART_WriteBlocking(uart, g_vt100_xy_1, sizeof(g_vt100_xy_1) / sizeof(g_vt100_xy_1[0]));
	UART_WriteBlocking(uart, g_vt100_color_2, sizeof(g_vt100_color_2) / sizeof(g_vt100_color_2[0]));
	UART_WriteBlocking(uart, g_vt100_strg_curr1, sizeof(g_vt100_strg_curr1) / sizeof(g_vt100_strg_curr1[0]));
	UART_WriteBlocking(uart, g_vt100_xy_2, sizeof(g_vt100_xy_2) / sizeof(g_vt100_xy_2[0]));
	UART_WriteBlocking(uart, g_vt100_strg_curr2, sizeof(g_vt100_strg_curr2) / sizeof(g_vt100_strg_curr2[0]));
	UART_WriteBlocking(uart, g_vt100_color_4, sizeof(g_vt100_color_4) / sizeof(g_vt100_color_4[0]));
}


void TERMINAL_change_seconds(UART_Type *uart)
{
	UART_WriteBlocking(uart, g_vt100_color_1, sizeof(g_vt100_color_1) / sizeof(g_vt100_color_1[0]));
	UART_WriteBlocking(uart, g_vt100_color_2, sizeof(g_vt100_color_2) / sizeof(g_vt100_color_2[0]));
	UART_WriteBlocking(uart, g_vt100_xy_2, sizeof(g_vt100_xy_2) / sizeof(g_vt100_xy_2[0]));
	UART_WriteBlocking(uart, g_vt100_strg_time2, sizeof(g_vt100_strg_time2) / sizeof(g_vt100_strg_time2[0]));
}


void TERMINAL_show_set_time(UART_Type *uart){
	UART_WriteBlocking(uart, g_vt100_clear, sizeof(g_vt100_clear) / sizeof(g_vt100_clear[0]));
	UART_WriteBlocking(uart, g_vt100_color_1, sizeof(g_vt100_color_1) / sizeof(g_vt100_color_1[0]));
	UART_WriteBlocking(uart, g_vt100_clear, sizeof(g_vt100_clear) / sizeof(g_vt100_clear[0]));
	UART_WriteBlocking(uart, g_vt100_xy_1, sizeof(g_vt100_xy_1) / sizeof(g_vt100_xy_1[0]));
	UART_WriteBlocking(uart, g_vt100_color_2, sizeof(g_vt100_color_2) / sizeof(g_vt100_color_2[0]));
	UART_WriteBlocking(uart, g_vt100_strg_timeset1, sizeof(g_vt100_strg_timeset1) / sizeof(g_vt100_strg_time1[0]));
	UART_WriteBlocking(uart, g_vt100_xy_2, sizeof(g_vt100_xy_2) / sizeof(g_vt100_xy_2[0]));
	UART_WriteBlocking(uart, g_vt100_show_cursor, sizeof(g_vt100_show_cursor) / sizeof(g_vt100_show_cursor[0]));

}

void TERMINAL_msg_set_time(UART_Type *uart)
{
	UART_WriteBlocking(uart, g_vt100_xy_3, sizeof(g_vt100_xy_3) / sizeof(g_vt100_xy_3[0]));
	UART_WriteBlocking(uart, g_vt100_strg_timeset2, sizeof(g_vt100_strg_timeset2) / sizeof(g_vt100_strg_timeset2[0]));
	UART_WriteBlocking(uart, g_vt100_hide_cursor, sizeof(g_vt100_hide_cursor) / sizeof(g_vt100_hide_cursor[0]));
}

void TERMINAL_msg_set_date(UART_Type *uart)
{
	UART_WriteBlocking(uart, g_vt100_xy_3, sizeof(g_vt100_xy_3) / sizeof(g_vt100_xy_3[0]));
	UART_WriteBlocking(uart, g_vt100_strg_dateset2, sizeof(g_vt100_strg_dateset2) / sizeof(g_vt100_strg_dateset2[0]));
	UART_WriteBlocking(uart, g_vt100_hide_cursor, sizeof(g_vt100_hide_cursor) / sizeof(g_vt100_hide_cursor[0]));
}

void TERMINAL_show_set_date(UART_Type *uart){
	UART_WriteBlocking(uart, g_vt100_clear, sizeof(g_vt100_clear) / sizeof(g_vt100_clear[0]));
	UART_WriteBlocking(uart, g_vt100_color_1, sizeof(g_vt100_color_1) / sizeof(g_vt100_color_1[0]));
	UART_WriteBlocking(uart, g_vt100_clear, sizeof(g_vt100_clear) / sizeof(g_vt100_clear[0]));
	UART_WriteBlocking(uart, g_vt100_xy_1, sizeof(g_vt100_xy_1) / sizeof(g_vt100_xy_1[0]));
	UART_WriteBlocking(uart, g_vt100_color_2, sizeof(g_vt100_color_2) / sizeof(g_vt100_color_2[0]));
	UART_WriteBlocking(uart, g_vt100_strg_dateset1, sizeof(g_vt100_strg_dateset1) / sizeof(g_vt100_strg_dateset1[0]));
	UART_WriteBlocking(uart, g_vt100_xy_2, sizeof(g_vt100_xy_2) / sizeof(g_vt100_xy_2[0]));
	UART_WriteBlocking(uart, g_vt100_show_cursor, sizeof(g_vt100_show_cursor) / sizeof(g_vt100_show_cursor[0]));
}

void TERMINAL_show_set_log(UART_Type *uart)
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

void TERMINAL_msg_set_log_confirm(UART_Type *uart)
{
	UART_WriteBlocking(uart, g_vt100_xy_3, sizeof(g_vt100_xy_3) / sizeof(g_vt100_xy_3[0]));
	UART_WriteBlocking(uart, g_vt100_strg_logset2, sizeof(g_vt100_strg_logset2) / sizeof(g_vt100_strg_logset2[0]));
	UART_WriteBlocking(uart, g_vt100_xy_4, sizeof(g_vt100_xy_4) / sizeof(g_vt100_xy_4[0]));
	UART_WriteBlocking(uart, g_vt100_show_cursor, sizeof(g_vt100_show_cursor) / sizeof(g_vt100_show_cursor[0]));
}

void TERMINAL_show_set_log_time(UART_Type *uart)
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

void TERMINAL_show_set_log_cap(UART_Type *uart)
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

void TERMINAL_msg_set_log_done(UART_Type *uart)
{
    UART_WriteBlocking(uart, g_vt100_xy_5, sizeof(g_vt100_xy_5) / sizeof(g_vt100_xy_5[0]));
    UART_WriteBlocking(uart, g_vt100_strg_logset8, sizeof(g_vt100_strg_logset8) / sizeof(g_vt100_strg_logset8[0]));
    UART_WriteBlocking(uart, g_vt100_hide_cursor, sizeof(g_vt100_hide_cursor) / sizeof(g_vt100_hide_cursor[0]));
}

void TERMINAL_msg_set_log_cancel(UART_Type *uart)
{
    UART_WriteBlocking(uart, g_vt100_xy_5, sizeof(g_vt100_xy_5) / sizeof(g_vt100_xy_5[0]));
    UART_WriteBlocking(uart, g_vt100_strg_logset7, sizeof(g_vt100_strg_logset7) / sizeof(g_vt100_strg_logset2[0]));
    UART_WriteBlocking(uart, g_vt100_hide_cursor, sizeof(g_vt100_hide_cursor) / sizeof(g_vt100_hide_cursor[0]));
}

void TERMINAL_show_read_log(UART_Type *uart)
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

void TERMINAL_msg_busy(UART_Type *uart)
{
    UART_WriteBlocking(uart, g_vt100_color_1, sizeof(g_vt100_color_1) / sizeof(g_vt100_color_1[0]));
    UART_WriteBlocking(uart, g_vt100_xy_7, sizeof(g_vt100_xy_7) / sizeof(g_vt100_xy_7[0]));
    UART_WriteBlocking(uart, g_vt100_color_2, sizeof(g_vt100_color_2) / sizeof(g_vt100_color_2[0]));
    UART_WriteBlocking(uart, g_vt100_strg_busy, sizeof(g_vt100_strg_busy) / sizeof(g_vt100_strg_busy[0]));
}

void TERMINAL_msg_busy_log(UART_Type *uart)
{
    UART_WriteBlocking(uart, g_vt100_xy_5, sizeof(g_vt100_xy_5) / sizeof(g_vt100_xy_5[0]));
    UART_WriteBlocking(uart, g_vt100_strg_busy_log, sizeof(g_vt100_strg_busy_log) / sizeof(g_vt100_strg_busy_log[0]));
    UART_WriteBlocking(uart, g_vt100_hide_cursor, sizeof(g_vt100_hide_cursor) / sizeof(g_vt100_hide_cursor[0]));
}


void TERMINAL_update_time() {

    char formattedString[TIME_STRING_LENGHT];

    // Convert tens and units to characters
    formattedString[0] = CHAR_TO_INT + g_terminal_time[0]; // Tens of hours
    formattedString[1] = CHAR_TO_INT + g_terminal_time[1]; // Units of hours
    formattedString[2] = COLON;
    formattedString[3] = CHAR_TO_INT + g_terminal_time[2]; // Tens of minutes
    formattedString[4] = CHAR_TO_INT + g_terminal_time[3]; // Units of minutes
    formattedString[5] = COLON;
    formattedString[6] = CHAR_TO_INT + g_terminal_time[4]; // Tens of seconds
    formattedString[7] = CHAR_TO_INT + g_terminal_time[5]; // Units of seconds
    formattedString[8] = '\0';

    // Copy the characters one by one to g_vt100_strg_time2
    for (int i = 0; i < TIME_STRING_LENGHT; i++) {
        g_vt100_strg_time2[i] = formattedString[i];
    }
}

void TERMINAL_update_date() {

    char formattedString[DATE_STRING_LENGHT];

    // Convert tens and units to characters
    formattedString[0] = CHAR_TO_INT + g_terminal_date[0]; // Tens of days
    formattedString[1] = CHAR_TO_INT + g_terminal_date[1]; // Units of days
    formattedString[2] = SLASH;
    formattedString[3] = CHAR_TO_INT + g_terminal_date[2]; // Tens of month
    formattedString[4] = CHAR_TO_INT + g_terminal_date[3]; // Units of month
    formattedString[5] = SLASH;
    formattedString[6] = '2';
    formattedString[7] = CHAR_TO_INT;
    formattedString[8] = CHAR_TO_INT + g_terminal_date[4]; // Tens of year
    formattedString[9] = CHAR_TO_INT + g_terminal_date[5]; // Units of year

    formattedString[10] = '\0';

    // Copy the characters one by one to g_vt100_strg_date2
    for (int i = 0; i < DATE_STRING_LENGHT; i++) {
        g_vt100_strg_date2[i] = formattedString[i];
    }
}

void TERMINAL_update_curr()
{
	char formattedString[CURR_STRING_LENGHT];

	formattedString[0] = CHAR_TO_INT + g_terminal_curr[0]; // Tens of days
	formattedString[1] = CHAR_TO_INT + g_terminal_curr[1]; // Units of days
	formattedString[2] = DOT;
	formattedString[3] = CHAR_TO_INT + g_terminal_curr[2]; // Tens of month
	formattedString[4] = CHAR_TO_INT + g_terminal_curr[3]; // Tens of month

    // Copy the characters one by one to g_vt100_strg_date2
    for (int i = 0; i < CURR_STRING_LENGHT; i++) {
        g_vt100_strg_curr2[i] = formattedString[i];
    }
}

uint8_t* TERMINAL_get_time() {
    return g_terminal_time;
}

uint8_t* TERMINAL_get_date() {
    return g_terminal_date;
}

uint8_t* TERMINAL_get_curr() {
    return g_terminal_curr;
}

uint8_t TERMINAL_get_log_num() {
    return g_terminal_log_num;
}

void TERMINAL_set_log_num(uint8_t log) {
    g_terminal_log_num = log;
}

uint8_t* TERMINAL_get_data_log(uint8_t log)
{
    switch (log) {
        case 1:
            return g_terminal_data_log_1;
        case 2:
            return g_terminal_data_log_2;
        case 3:
            return g_terminal_data_log_3;
        case 4:
            return g_terminal_data_log_4;
        case 5:
            return g_terminal_data_log_5;
        default:
            return NULL;
    }
}

void TERMINAL_show_data_log(uint8_t log, UART_Type *uart)
{
	uint8_t* terminal_data_log = TERMINAL_get_data_log(log);

	g_vt100_strg_logread2[13] = terminal_data_log[0]+CHAR_TO_INT;
	g_vt100_strg_logread2[14] = terminal_data_log[1]+CHAR_TO_INT;
	g_vt100_strg_logread2[16] = terminal_data_log[2]+CHAR_TO_INT;
	g_vt100_strg_logread2[17] = terminal_data_log[3]+CHAR_TO_INT;
	g_vt100_strg_logread2[30] = terminal_data_log[4]+CHAR_TO_INT;
	g_vt100_strg_logread2[31] = terminal_data_log[5]+CHAR_TO_INT;
	g_vt100_strg_logread2[33] = terminal_data_log[6]+CHAR_TO_INT;
	g_vt100_strg_logread2[34] = terminal_data_log[7]+CHAR_TO_INT;
	g_vt100_strg_logread2[38] = terminal_data_log[8]+CHAR_TO_INT;
	g_vt100_strg_logread2[39] = terminal_data_log[9]+CHAR_TO_INT;
	g_vt100_strg_logread2[41] = terminal_data_log[10]+CHAR_TO_INT;
	g_vt100_strg_logread2[42] = terminal_data_log[11]+CHAR_TO_INT;
	g_vt100_strg_logread2[44] = terminal_data_log[12]+CHAR_TO_INT;
	g_vt100_strg_logread2[45] = terminal_data_log[13]+CHAR_TO_INT;
	g_vt100_strg_logread2[51] = terminal_data_log[14]+CHAR_TO_INT;
	g_vt100_strg_logread2[52] = terminal_data_log[15]+CHAR_TO_INT;
	g_vt100_strg_logread2[54] = terminal_data_log[16]+CHAR_TO_INT;
	g_vt100_strg_logread2[55] = terminal_data_log[17]+CHAR_TO_INT;
	g_vt100_strg_logread2[59] = terminal_data_log[18]+CHAR_TO_INT;
	g_vt100_strg_logread2[60] = terminal_data_log[19]+CHAR_TO_INT;

	UART_WriteBlocking(uart, g_vt100_hide_cursor, sizeof(g_vt100_hide_cursor) / sizeof(g_vt100_hide_cursor[0]));
	UART_WriteBlocking(uart, g_vt100_xy_jump1, sizeof(g_vt100_xy_jump1) / sizeof(g_vt100_xy_jump1[0]));
	UART_WriteBlocking(uart, g_vt100_color_2, sizeof(g_vt100_color_2) / sizeof(g_vt100_color_2[0]));
	UART_WriteBlocking(uart, g_vt100_strg_logread2, sizeof(g_vt100_strg_logread2) / sizeof(g_vt100_strg_logread2[0]));
}

void TERMINAL_show_data_log_jump(UART_Type *uart)
{
	UART_WriteBlocking(uart, g_vt100_xy_0, sizeof(g_vt100_xy_0) / sizeof(g_vt100_xy_0[0]));
}

void TERMINAL_show_error_wdt(UART_Type *uart)
{
	UART_WriteBlocking(uart, g_vt100_color_1, sizeof(g_vt100_color_1) / sizeof(g_vt100_color_1[0]));
	UART_WriteBlocking(uart, g_vt100_xy_7, sizeof(g_vt100_xy_7) / sizeof(g_vt100_xy_7[0]));
	UART_WriteBlocking(uart, g_vt100_color_2, sizeof(g_vt100_color_2) / sizeof(g_vt100_color_2[0]));
	UART_WriteBlocking(uart, g_vt100_strg_error_wdt, sizeof(g_vt100_strg_error_wdt) / sizeof(g_vt100_strg_error_wdt[0]));
	UART_WriteBlocking(uart, g_vt100_hide_cursor, sizeof(g_vt100_hide_cursor) / sizeof(g_vt100_hide_cursor[0]));
}

void TERMINAL_show_error_i2c(UART_Type *uart)
{
	UART_WriteBlocking(uart, g_vt100_color_1, sizeof(g_vt100_color_1) / sizeof(g_vt100_color_1[0]));
	UART_WriteBlocking(uart, g_vt100_xy_7, sizeof(g_vt100_xy_7) / sizeof(g_vt100_xy_7[0]));
	UART_WriteBlocking(uart, g_vt100_color_2, sizeof(g_vt100_color_2) / sizeof(g_vt100_color_2[0]));
	UART_WriteBlocking(uart, g_vt100_strg_error_i2c, sizeof(g_vt100_strg_error_i2c) / sizeof(g_vt100_strg_error_i2c[0]));
	UART_WriteBlocking(uart, g_vt100_hide_cursor, sizeof(g_vt100_hide_cursor) / sizeof(g_vt100_hide_cursor[0]));
}
