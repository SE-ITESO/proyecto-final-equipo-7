/*
 * MANAGER.c
 *
 * 	Created on: 13/10/2023
 * 		Author: Diego Delgado, Alberto Quintana
 *
 *  @brief
 *  This is the source file for the MANAGER module.
 *  It contains the implementation of functions for managing data between I2C and UART modules.
 *
 *  @details
 *  The MANAGER module orchestrates the communication between I2C and UART modules to read and update time values.
 *  It provides functions for converting data between I2C and UART formats and updating time values.
 *
 */

#include "UART.h"
#include "I2C.h"
#include "MANAGER.h"
#include "CONFIG.h"
#include "fsl_wdog.h"
#include "stdbool.h"
#include "LCD_nokia.h"

uint8_t g_handler_flag=0;
uint8_t g_handler_uart0_flag=0;
uint8_t g_handler_uart4_flag=0;

void MANAGER_I2C_to_UART_time(){
    uint8_t* uart_rx_time = UART_get_rx_time();
    uint8_t* i2c_rx_time = I2C_get_rx_time();

    // Extract seconds, minutes, and hours from I2C data
    uint8_t seconds = i2c_rx_time[I2C_SECONDS_INDEX];
    uint8_t minutes = i2c_rx_time[I2C_MINUTES_INDEX];
    uint8_t hours = i2c_rx_time[I2C_HOURS_INDEX];

    // Convert and store hours, minutes, and seconds in UART buffer
    uart_rx_time[UART_TENS_HOURS_INDEX]   = (hours >> 4) & UNITS_MASK;
    uart_rx_time[UART_UNITS_HOURS_INDEX]  = hours & UNITS_MASK;
    uart_rx_time[UART_TENS_MINUTES_INDEX] = (minutes >> 4) & UNITS_MASK;
    uart_rx_time[UART_UNITS_MINUTES_INDEX] = minutes & UNITS_MASK;
    uart_rx_time[UART_TENS_SECONDS_INDEX] = (seconds >> 4) & UNITS_MASK;
    uart_rx_time[UART_UNITS_SECONDS_INDEX] = seconds & UNITS_MASK;
}

void MANAGER_UART_to_I2C_time() {
    uint8_t* uart_rx_time = UART_get_rx_time();
    uint8_t* i2c_rx_time = I2C_get_rx_time();

    // Extract tens and units for hours, minutes, and seconds from UART data
    uint8_t tensHours = uart_rx_time[UART_TENS_HOURS_INDEX];
    uint8_t unitsHours = uart_rx_time[UART_UNITS_HOURS_INDEX];
    uint8_t tensMinutes = uart_rx_time[UART_TENS_MINUTES_INDEX];
    uint8_t unitsMinutes = uart_rx_time[UART_UNITS_MINUTES_INDEX];
    uint8_t tensSeconds = uart_rx_time[UART_TENS_SECONDS_INDEX];
    uint8_t unitsSeconds = uart_rx_time[UART_UNITS_SECONDS_INDEX];

    // Combine tens and units to form hours, minutes, and seconds in I2C buffer
    i2c_rx_time[I2C_HOURS_INDEX] = ((tensHours & UNITS_MASK) << 4) | (unitsHours & UNITS_MASK);
    i2c_rx_time[I2C_MINUTES_INDEX] = ((tensMinutes & UNITS_MASK) << 4) | (unitsMinutes & UNITS_MASK);
    i2c_rx_time[I2C_SECONDS_INDEX] = ((tensSeconds & UNITS_MASK) << 4) | (unitsSeconds & UNITS_MASK);
}

void MANAGER_I2C_to_UART_date(){

	uint8_t* uart_rx_date = UART_get_rx_date();
	uint8_t* i2c_rx_date = I2C_get_rx_date();

    uint16_t year =  i2c_rx_date[I2C_YEAR_INDEX];
    uint8_t month = i2c_rx_date[I2C_MONTH_INDEX];
    uint8_t day = i2c_rx_date[I2C_DAY_INDEX];

    uart_rx_date[UART_TENS_DAY_INDEX]   = (day >> 4) & UNITS_MASK;
    uart_rx_date[UART_UNITS_DAY_INDEX]  = day & UNITS_MASK;
    uart_rx_date[UART_TENS_MONTH_INDEX] = (month >> 4) & UNITS_MASK;
    uart_rx_date[UART_UNITS_MONTH_INDEX] = month & UNITS_MASK;
    uart_rx_date[UART_TENS_YEAR_INDEX] = (year >> 4) & UNITS_MASK;
    uart_rx_date[UART_UNITS_YEAR_INDEX] =year & UNITS_MASK;
}
//////////////////////////////////////////////////
void MANAGER_I2C_to_LCD_nokia_date() {
    uint8_t formattedDate[6];

    uint8_t* uart_rx_date = UART_get_rx_date();
    	uint8_t* i2c_rx_date = I2C_get_rx_date();

    	uint16_t year =  i2c_rx_date[I2C_YEAR_INDEX];
    	    uint8_t month = i2c_rx_date[I2C_MONTH_INDEX];
    	    uint8_t day = i2c_rx_date[I2C_DAY_INDEX];

    formattedDate[0] = (day >> 4) & UNITS_MASK;       // Tens of Day
    formattedDate[1] = day & UNITS_MASK;              // Units of Day
    formattedDate[2] = (month >> 4) & UNITS_MASK;     // Tens of Month
    formattedDate[3] = month & UNITS_MASK;            // Units of Month
    formattedDate[4] = (year >> 4) & UNITS_MASK;      // Tens of Year
    formattedDate[5] = year & UNITS_MASK;             // Units of Year


    LCD_nokia_goto_xy(2, 0);
    LCD_nokia_send_char(formattedDate[0] + '0');
    LCD_nokia_send_char(formattedDate[1] + '0');
    LCD_nokia_send_char('/');
    LCD_nokia_send_char(formattedDate[2] + '0');
    LCD_nokia_send_char(formattedDate[3] + '0');
    LCD_nokia_send_char('/');
    LCD_nokia_send_char('2');
    LCD_nokia_send_char('0');
    LCD_nokia_send_char(formattedDate[4] + '0');
    LCD_nokia_send_char(formattedDate[5] + '0');
}


void MANAGER_I2C_to_LCD_nokia_time(){
	 uint8_t formattedTime[6];

	      uint8_t* i2c_rx_time = I2C_get_rx_time();

	            uint8_t seconds = i2c_rx_time[I2C_SECONDS_INDEX];
	             uint8_t minutes = i2c_rx_time[I2C_MINUTES_INDEX];
	             uint8_t hours = i2c_rx_time[I2C_HOURS_INDEX];


	    formattedTime[0] = ((hours >> 4) & UNITS_MASK);       // Tens of Hours
	    formattedTime[1] = hours & UNITS_MASK;                // Units of Hours
	    formattedTime[2] = ((minutes >> 4) & UNITS_MASK);     // Tens of Minutes
	    formattedTime[3] = minutes & UNITS_MASK;              // Units of Minutes
	    formattedTime[4] = ((seconds >> 4) & UNITS_MASK);     // Tens of Seconds
	    formattedTime[5] = seconds & UNITS_MASK;              // Units of Seconds


	    LCD_nokia_goto_xy(2, 1);
	    LCD_nokia_send_char(formattedTime[0] + '0');
	    LCD_nokia_send_char(formattedTime[1] + '0');
	    LCD_nokia_send_char(':');
	    LCD_nokia_send_char(formattedTime[2] + '0');
	    LCD_nokia_send_char(formattedTime[3] + '0');
	    LCD_nokia_send_char(':');
	    LCD_nokia_send_char(formattedTime[4] + '0');
	    LCD_nokia_send_char(formattedTime[5] + '0');
}




void MANAGER_UART_to_I2C_date() {
    uint8_t* uart_rx_date = UART_get_rx_date();
    uint8_t* i2c_rx_date = I2C_get_rx_date();

    uint8_t tensDay = uart_rx_date[UART_TENS_DAY_INDEX];
    uint8_t unitsDay = uart_rx_date[UART_UNITS_DAY_INDEX];
    uint8_t tensMonth = uart_rx_date[UART_TENS_MONTH_INDEX];
    uint8_t unitsMonth = uart_rx_date[UART_UNITS_MONTH_INDEX];
    uint8_t tensYear = uart_rx_date[UART_TENS_YEAR_INDEX];
    uint8_t unitsYear = uart_rx_date[UART_UNITS_YEAR_INDEX];

    i2c_rx_date[I2C_DAY_INDEX] = ((tensDay & UNITS_MASK) << 4) | (unitsDay & UNITS_MASK);
    i2c_rx_date[I2C_MONTH_INDEX] = ((tensMonth & UNITS_MASK) << 4) | (unitsMonth & UNITS_MASK);
    i2c_rx_date[I2C_YEAR_INDEX] = ((tensYear& UNITS_MASK) << 4) | (unitsYear & UNITS_MASK);
}

void MANAGER_I2C_to_UART_hum()
{
    uint8_t* uart_rx_hum = UART_get_rx_hum();
    uint8_t* i2c_rx_hum = I2C_get_rx_hum();

    uint64_t concatenatedValue = 0;

	concatenatedValue |= i2c_rx_hum[0] << 12;
	concatenatedValue |= i2c_rx_hum[1] << 4;
	concatenatedValue |= i2c_rx_hum[2] >> 4;

	concatenatedValue = (concatenatedValue * 10000) >> 20;

    uint32_t digit4 = concatenatedValue % 10;
    uint32_t digit3 = (concatenatedValue / 10) % 10;
    uint32_t digit2 = (concatenatedValue / 100) % 10;
    uint32_t digit1 = (concatenatedValue / 1000) % 10;

    uart_rx_hum[0] = digit1;
    uart_rx_hum[1] = digit2;
    uart_rx_hum[2] = digit3;
    uart_rx_hum[3] = digit4;
}


void MANAGER_update_seconds_uart()
{
	if((g_handler_uart0_flag)&&(g_handler_flag)){
		MANAGER_I2C_to_UART_time();
		UART_update_time();
		UART_change_seconds(UART0);
	}
}


////////////////////////////////////
void MANAGER_data_update_seconds_lcd()
{
	//if((g_handler_uart0_flag)&&(g_handler_flag)){
		MANAGER_I2C_to_LCD_nokia_time();
		MANAGER_I2C_to_LCD_nokia_date();
		//LCD_nokia_goto_xy(2, 1);
		//LCD_nokia_send_string('Corriente: ');



		//UART_update_time();
	//	UART_change_seconds(UART0);
	//}
}
///////////////////////////////////////

void MANAGER_update_seconds_uart0_flag(){
	g_handler_uart0_flag=1;
}

void MANAGER_update_seconds_uart4_flag(){
	g_handler_uart4_flag=1;
}

void MANAGER_update_seconds_uart0_flag_off()
{
	g_handler_uart0_flag=0;
}


void MANAGER_update_seconds_uart4_flag_off()
{
	g_handler_uart4_flag=0;
}

LOG_Data_t dataLog1;
LOG_Data_t dataLog2;
LOG_Data_t dataLog3;
LOG_Data_t dataLog4;
LOG_Data_t dataLog5;


void MANAGER_set_log(uint8_t log, uint8_t time, uint8_t caps1, uint8_t caps2)
{
	log -= CHAR_TO_INT;
	time -= CHAR_TO_INT;
	caps1 -= CHAR_TO_INT;
	caps2 -= CHAR_TO_INT;

	switch(log)
	{
		case LOG_1:
			dataLog1.flag = true;
			dataLog1.time = time;
			dataLog1.caps = (caps1*10)+caps2;
			I2C_write_caps_log(LOG_1,dataLog1.caps);

			break;
		case LOG_2:
			dataLog2.flag = true;
			dataLog2.time = time;
			dataLog2.caps = (caps1*10)+caps2;
			I2C_write_caps_log(LOG_2,dataLog2.caps);
			break;
		case LOG_3:
			dataLog3.flag = true;
			dataLog3.time = time;
			dataLog3.caps = (caps1*10)+caps2;
			I2C_write_caps_log(LOG_3,dataLog3.caps);
			break;
		case LOG_4:
			dataLog4.flag = true;
			dataLog4.time = time;
			dataLog4.caps = (caps1*10)+caps2;
			I2C_write_caps_log(LOG_4,dataLog4.caps);
			break;
		case LOG_5:
			dataLog5.flag = true;
			dataLog5.time = time;
			dataLog5.caps = (caps1*10)+caps2;
			I2C_write_caps_log(LOG_5,dataLog5.caps);
			break;
		default:
			break;
	}
}


void MANAGER_handler_flag(){
	g_handler_flag=1;
}
void MANAGER_handler_log()
{
	if(g_handler_flag){

		MANAGER_update_seconds_uart();

		g_handler_flag=0;

		I2C_read_rtc_time();
		I2C_read_rtc_date();

		if(dataLog1.flag){
			if(dataLog1.time == dataLog1.time_count){
				I2C_write_data_log(LOG_1,dataLog1.caps_count);
				dataLog1.caps_count++;
				dataLog1.time_count = 0;
			}
			dataLog1.time_count++;
			if(I2C_get_caps_log(LOG_1)== dataLog1.caps_count){
				dataLog1.time_count = 0;
				dataLog1.caps_count = 0;
				dataLog1.flag = false;
			}
		}

		if(dataLog2.flag){
			if(dataLog2.time == dataLog2.time_count){
				I2C_write_data_log(LOG_2,dataLog2.caps_count);
				dataLog2.caps_count++;
				dataLog2.time_count = 0;
			}
			dataLog2.time_count++;
			if(I2C_get_caps_log(LOG_2) == dataLog2.caps_count){
				dataLog2.time_count = 0;
				dataLog2.caps_count = 0;
				dataLog2.flag = false;
			}
		}

		if(dataLog3.flag){
			if(dataLog3.time == dataLog3.time_count){
				I2C_write_data_log(LOG_3,dataLog3.caps_count);
				dataLog3.caps_count++;
				dataLog3.time_count = 0;
			}
			dataLog3.time_count++;
			if(I2C_get_caps_log(LOG_3) == dataLog3.caps_count){
				dataLog3.time_count = 0;
				dataLog3.caps_count = 0;
				dataLog3.flag = false;
			}
		}

		if(dataLog4.flag){
			if(I2C_get_caps_log(LOG_4) == dataLog4.time_count){
				I2C_write_data_log(LOG_4,dataLog4.caps_count);
				dataLog4.caps_count++;
				dataLog4.time_count = 0;
			}
			dataLog4.time_count++;
			if(I2C_get_caps_log(LOG_4) == dataLog4.caps_count){
				dataLog4.time_count = 0;
				dataLog4.caps_count = 0;
				dataLog4.flag = false;
			}
		}

		if(dataLog5.flag){
			if(dataLog5.time == dataLog5.time_count){
				I2C_write_data_log(LOG_5,dataLog5.caps_count);
				dataLog5.caps_count++;
				dataLog5.time_count = 0;
			}
			dataLog5.time_count++;
			if(I2C_get_caps_log(LOG_5) == dataLog5.caps_count){
				dataLog5.time_count = 0;
				dataLog5.caps_count = 0;
				dataLog5.flag = false;
			}
		}
	WDOG_Refresh(WDOG);
	}
}

void MANAGER_I2C_to_UART_log(uint8_t log, UART_Type *uart)
{
	uint8_t caps;
	uint64_t concatenatedValue = 0;
	log -= CHAR_TO_INT;
	switch(log)
	{
		case LOG_1:
			if (dataLog1.flag == true) {
				UART_msg_busy_log(uart);
				caps = 0;
			} else {
				caps = I2C_get_caps_log(LOG_1);
			}
			break;

		case LOG_2:
			if (dataLog2.flag == true) {
				UART_msg_busy_log(uart);
				caps = 0;
			} else {
				caps = I2C_get_caps_log(LOG_2);
			}
			break;

		case LOG_3:
			if (dataLog3.flag == true) {
				UART_msg_busy_log(uart);
				caps = 0;
			} else {
				caps = I2C_get_caps_log(LOG_3);;
			}
			break;

		case LOG_4:
			if (dataLog4.flag == true) {
				UART_msg_busy_log(uart);
				caps = 0;
			} else {
				caps = I2C_get_caps_log(LOG_4);;
			}
			break;

		case LOG_5:
			if (dataLog5.flag == true) {
				UART_msg_busy_log(uart);
				caps = 0;
			} else {
				caps = I2C_get_caps_log(LOG_5);;
			}
			break;

		default:
			break;
	}

	UART_show_data_log_jump(uart);

	for (uint8_t i = 0; i < caps; i++) {

		I2C_read_log(log,i);
		uint8_t* i2c_data_log = I2C_get_data_log(log);
		uint8_t* uart_data_log = UART_get_data_log(log);

		concatenatedValue |= (i2c_data_log[3] & 0x0F) << 16;
		concatenatedValue |= i2c_data_log[4] << 8;
		concatenatedValue |= i2c_data_log[5];

		concatenatedValue = concatenatedValue * 20000;
		concatenatedValue = concatenatedValue >> 20;
		concatenatedValue = concatenatedValue -5000;

		uart_data_log[3]  = concatenatedValue % 10;
		uart_data_log[2] = (concatenatedValue / 10) % 10;
		uart_data_log[1] = (concatenatedValue / 100) % 10;
		uart_data_log[0] = (concatenatedValue / 1000) % 10;

	    concatenatedValue = 0;

	    concatenatedValue |= i2c_data_log[0] << 12;
		concatenatedValue |= i2c_data_log[1] << 4;
		concatenatedValue |= i2c_data_log[2] >> 4;

		concatenatedValue = (concatenatedValue * 10000) >> 20;

		uart_data_log[7]  = concatenatedValue % 10;
		uart_data_log[6] = (concatenatedValue / 10) % 10;
		uart_data_log[5] = (concatenatedValue / 100) % 10;
		uart_data_log[4] = (concatenatedValue / 1000) % 10;

	    uint8_t seconds = i2c_data_log[6];
	    uint8_t minutes = i2c_data_log[7];
	    uint8_t hours = i2c_data_log[8];

	    uart_data_log[8]  = (hours >> 4) & UNITS_MASK;
	    uart_data_log[9]  = hours & UNITS_MASK;
	    uart_data_log[10] = (minutes >> 4) & UNITS_MASK;
	    uart_data_log[11] = minutes & UNITS_MASK;
	    uart_data_log[12] = (seconds >> 4) & UNITS_MASK;
	    uart_data_log[13] = seconds & UNITS_MASK;

	    uint16_t year =  i2c_data_log[11];
	    uint8_t month = i2c_data_log[10];
	    uint8_t day = i2c_data_log[9];

	    uart_data_log[14] = (day >> 4) & UNITS_MASK;
	    uart_data_log[15] = day & UNITS_MASK;
	    uart_data_log[16] = (month >> 4) & UNITS_MASK;
	    uart_data_log[17] = month & UNITS_MASK;
	    uart_data_log[18] = (year >> 4) & UNITS_MASK;
	    uart_data_log[19] = year & UNITS_MASK;

	    UART_show_data_log(log, uart);
	}

}


