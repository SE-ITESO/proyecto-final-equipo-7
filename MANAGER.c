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

#include "MANAGER.h"
#include "CONFIG.h"
#include "stdbool.h"
#include "TERMINAL.h"
#include "RTC.h"
#include "LOGGER.h"

uint8_t g_handler_flag=0;
uint8_t g_handler_terminal1_flag=0;
uint8_t g_handler_terminal2_flag=0;

void MANAGER_RTC_to_TERMINAL_time(){
    uint8_t* terminal_time = TERMINAL_get_time();
    uint8_t* rtc_time = RTC_get_time();

    // Extract seconds, minutes, and hours from I2C data
    uint8_t seconds = rtc_time[RTC_SECONDS_INDEX];
    uint8_t minutes = rtc_time[RTC_MINUTES_INDEX];
    uint8_t hours = rtc_time[RTC_HOURS_INDEX];

    // Convert and store hours, minutes, and seconds in UART buffer
    terminal_time[TERMINAL_TENS_HOURS_INDEX]   = (hours >> 4) & UNITS_MASK;
    terminal_time[TERMINAL_UNITS_HOURS_INDEX]  = hours & UNITS_MASK;
    terminal_time[TERMINAL_TENS_MINUTES_INDEX] = (minutes >> 4) & UNITS_MASK;
    terminal_time[TERMINAL_UNITS_MINUTES_INDEX] = minutes & UNITS_MASK;
    terminal_time[TERMINAL_TENS_SECONDS_INDEX] = (seconds >> 4) & UNITS_MASK;
    terminal_time[TERMINAL_UNITS_SECONDS_INDEX] = seconds & UNITS_MASK;
}

void MANAGER_TERMINAL_to_RTC_time() {
    uint8_t* terminal_time = TERMINAL_get_time();
    uint8_t* rtc_time = RTC_get_time();

    // Extract tens and units for hours, minutes, and seconds from UART data
    uint8_t tensHours = terminal_time[TERMINAL_TENS_HOURS_INDEX];
    uint8_t unitsHours = terminal_time[TERMINAL_UNITS_HOURS_INDEX];
    uint8_t tensMinutes = terminal_time[TERMINAL_TENS_MINUTES_INDEX];
    uint8_t unitsMinutes = terminal_time[TERMINAL_UNITS_MINUTES_INDEX];
    uint8_t tensSeconds = terminal_time[TERMINAL_TENS_SECONDS_INDEX];
    uint8_t unitsSeconds = terminal_time[TERMINAL_UNITS_SECONDS_INDEX];

    // Combine tens and units to form hours, minutes, and seconds in RTC buffer
    rtc_time[RTC_HOURS_INDEX] = ((tensHours & UNITS_MASK) << 4) | (unitsHours & UNITS_MASK);
    rtc_time[RTC_MINUTES_INDEX] = ((tensMinutes & UNITS_MASK) << 4) | (unitsMinutes & UNITS_MASK);
    rtc_time[RTC_SECONDS_INDEX] = ((tensSeconds & UNITS_MASK) << 4) | (unitsSeconds & UNITS_MASK);
}

void MANAGER_RTC_to_TERMINAL_date(){

	uint8_t* terminal_date = TERMINAL_get_date();
	uint8_t* rtc_date = RTC_get_date();

    uint16_t year =  rtc_date[RTC_YEAR_INDEX];
    uint8_t month = rtc_date[RTC_MONTH_INDEX];
    uint8_t day = rtc_date[RTC_DAY_INDEX];

    terminal_date[TERMINAL_TENS_DAY_INDEX]   = (day >> 4) & UNITS_MASK;
    terminal_date[TERMINAL_UNITS_DAY_INDEX]  = day & UNITS_MASK;
    terminal_date[TERMINAL_TENS_MONTH_INDEX] = (month >> 4) & UNITS_MASK;
    terminal_date[TERMINAL_UNITS_MONTH_INDEX] = month & UNITS_MASK;
    terminal_date[TERMINAL_TENS_YEAR_INDEX] = (year >> 4) & UNITS_MASK;
    terminal_date[TERMINAL_UNITS_YEAR_INDEX] =year & UNITS_MASK;
}

void MANAGER_TERMINAL_to_RTC_date() {
    uint8_t* terminal_date = TERMINAL_get_date();
    uint8_t* rtc_date = RTC_get_date();

    uint8_t tensDay = terminal_date[TERMINAL_TENS_DAY_INDEX];
    uint8_t unitsDay = terminal_date[TERMINAL_UNITS_DAY_INDEX];
    uint8_t tensMonth = terminal_date[TERMINAL_TENS_MONTH_INDEX];
    uint8_t unitsMonth = terminal_date[TERMINAL_UNITS_MONTH_INDEX];
    uint8_t tensYear = terminal_date[TERMINAL_TENS_YEAR_INDEX];
    uint8_t unitsYear = terminal_date[TERMINAL_UNITS_YEAR_INDEX];

    rtc_date[RTC_DAY_INDEX] = ((tensDay & UNITS_MASK) << 4) | (unitsDay & UNITS_MASK);
    rtc_date[RTC_MONTH_INDEX] = ((tensMonth & UNITS_MASK) << 4) | (unitsMonth & UNITS_MASK);
    rtc_date[RTC_YEAR_INDEX] = ((tensYear& UNITS_MASK) << 4) | (unitsYear & UNITS_MASK);
}

void MANAGER_INA_to_TERMINAL_curr()
{
	uint8_t* terminal_curr = TERMINAL_get_curr();
	uint8_t* ina_curr = INA219_get_curr();


}

void MANAGER_update_seconds_terminal()
{
	if((g_handler_terminal1_flag)&&(g_handler_flag)){
		MANAGER_RTC_to_TERMINAL_time();
		TERMINAL_update_time();
		TERMINAL_change_seconds(TERMINAL_1);
	}

	else if((g_handler_terminal2_flag)&&(g_handler_flag)){
		RTC_read_time();
		MANAGER_RTC_to_TERMINAL_time();
		TERMINAL_update_time();
		TERMINAL_change_seconds(TERMINAL_2);
	}
}
void MANAGER_update_seconds_terminal1_flag(){
	g_handler_terminal1_flag=1;
}

void MANAGER_update_seconds_terminal2_flag(){
	g_handler_terminal2_flag=1;
}

void MANAGER_update_seconds_terminal1_flag_off()
{
	g_handler_terminal1_flag=0;
}


void MANAGER_update_seconds_terminal2_flag_off()
{
	g_handler_terminal2_flag=0;
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
			LOGGER_write_caps(LOG_1,dataLog1.caps);

			break;
		case LOG_2:
			dataLog2.flag = true;
			dataLog2.time = time;
			dataLog2.caps = (caps1*10)+caps2;
			LOGGER_write_caps(LOG_2,dataLog2.caps);
			break;
		case LOG_3:
			dataLog3.flag = true;
			dataLog3.time = time;
			dataLog3.caps = (caps1*10)+caps2;
			LOGGER_write_caps(LOG_3,dataLog3.caps);
			break;
		case LOG_4:
			dataLog4.flag = true;
			dataLog4.time = time;
			dataLog4.caps = (caps1*10)+caps2;
			LOGGER_write_caps(LOG_4,dataLog4.caps);
			break;
		case LOG_5:
			dataLog5.flag = true;
			dataLog5.time = time;
			dataLog5.caps = (caps1*10)+caps2;
			LOGGER_write_caps(LOG_5,dataLog5.caps);
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

		g_handler_flag=0;
		RTC_read_time();
		RTC_read_date();
		MANAGER_update_seconds_terminal();

		if(dataLog1.flag){
			if(dataLog1.time == dataLog1.time_count){
				LOGGER_write_data(LOG_1,dataLog1.caps_count);
				dataLog1.caps_count++;
				dataLog1.time_count = 0;
			}
			dataLog1.time_count++;
			if(LOGGER_read_caps(LOG_1)== dataLog1.caps_count){
				dataLog1.time_count = 0;
				dataLog1.caps_count = 0;
				dataLog1.flag = false;
			}
		}

		if(dataLog2.flag){
			if(dataLog2.time == dataLog2.time_count){
				LOGGER_write_data(LOG_2,dataLog2.caps_count);
				dataLog2.caps_count++;
				dataLog2.time_count = 0;
			}
			dataLog2.time_count++;
			if(LOGGER_read_caps(LOG_2) == dataLog2.caps_count){
				dataLog2.time_count = 0;
				dataLog2.caps_count = 0;
				dataLog2.flag = false;
			}
		}

		if(dataLog3.flag){
			if(dataLog3.time == dataLog3.time_count){
				LOGGER_write_data(LOG_3,dataLog3.caps_count);
				dataLog3.caps_count++;
				dataLog3.time_count = 0;
			}
			dataLog3.time_count++;
			if(LOGGER_read_caps(LOG_3) == dataLog3.caps_count){
				dataLog3.time_count = 0;
				dataLog3.caps_count = 0;
				dataLog3.flag = false;
			}
		}

		if(dataLog4.flag){
			if(LOGGER_read_caps(LOG_4) == dataLog4.time_count){
				LOGGER_write_data(LOG_4,dataLog4.caps_count);
				dataLog4.caps_count++;
				dataLog4.time_count = 0;
			}
			dataLog4.time_count++;
			if(LOGGER_read_caps(LOG_4) == dataLog4.caps_count){
				dataLog4.time_count = 0;
				dataLog4.caps_count = 0;
				dataLog4.flag = false;
			}
		}

		if(dataLog5.flag){
			if(dataLog5.time == dataLog5.time_count){
				LOGGER_write_data(LOG_5,dataLog5.caps_count);
				dataLog5.caps_count++;
				dataLog5.time_count = 0;
			}
			dataLog5.time_count++;
			if(LOGGER_read_caps(LOG_5) == dataLog5.caps_count){
				dataLog5.time_count = 0;
				dataLog5.caps_count = 0;
				dataLog5.flag = false;
			}
		}
	}
}

void MANAGER_LOGGER_to_TERMINAL(uint8_t log, UART_Type *uart)
{
	uint8_t caps;
	uint64_t concatenatedValue = 0;
	log -= CHAR_TO_INT;
	switch(log)
	{
		case LOG_1:
			if (dataLog1.flag == true) {
				TERMINAL_msg_busy_log(uart);
				caps = 0;
			} else {
				caps = LOGGER_read_caps(LOG_1);
			}
			break;

		case LOG_2:
			if (dataLog2.flag == true) {
				TERMINAL_msg_busy_log(uart);
				caps = 0;
			} else {
				caps = LOGGER_read_caps(LOG_2);
			}
			break;

		case LOG_3:
			if (dataLog3.flag == true) {
				TERMINAL_msg_busy_log(uart);
				caps = 0;
			} else {
				caps = LOGGER_read_caps(LOG_3);;
			}
			break;

		case LOG_4:
			if (dataLog4.flag == true) {
				TERMINAL_msg_busy_log(uart);
				caps = 0;
			} else {
				caps = LOGGER_read_caps(LOG_4);;
			}
			break;

		case LOG_5:
			if (dataLog5.flag == true) {
				TERMINAL_msg_busy_log(uart);
				caps = 0;
			} else {
				caps = LOGGER_read_caps(LOG_5);;
			}
			break;

		default:
			break;
	}

	TERMINAL_show_data_log_jump(uart);

	for (uint8_t i = 0; i < caps; i++) {

		LOGGER_read_data(log,i);
		uint8_t* logger_data = LOGGER_get_data(log);
		uint8_t* terminal_log = TERMINAL_get_data_log(log);



	    uint8_t seconds = logger_data[4];
	    uint8_t minutes = logger_data[5];
	    uint8_t hours = logger_data[6];

	    terminal_log[4]  = (hours >> 4) & UNITS_MASK;
	    terminal_log[5]  = hours & UNITS_MASK;
	    terminal_log[6] = (minutes >> 4) & UNITS_MASK;
	    terminal_log[7] = minutes & UNITS_MASK;
	    terminal_log[8] = (seconds >> 4) & UNITS_MASK;
	    terminal_log[9] = seconds & UNITS_MASK;

	    uint16_t year =  logger_data[11];
	    uint8_t month = logger_data[10];
	    uint8_t day = logger_data[9];

	    terminal_log[10] = (day >> 4) & UNITS_MASK;
	    terminal_log[11] = day & UNITS_MASK;
	    terminal_log[12] = (month >> 4) & UNITS_MASK;
	    terminal_log[13] = month & UNITS_MASK;
	    terminal_log[14] = (year >> 4) & UNITS_MASK;
	    terminal_log[15] = year & UNITS_MASK;

	    TERMINAL_show_data_log(log, uart);
	}

}
