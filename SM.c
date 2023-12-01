/*
 * @file 	SM.c
 *
 * @Authors Diego Delgado
 * 			Alberto Quintana
 *
 * @brief 	State machine with different "levels". Checks current state to execute certain task.
 *
 *
 */

#include "SM.h"
#include "CONFIG.h"
#include "UART.h"
#include "fsl_uart.h"
#include "RTC.h"
#include "INA219.h"
#include "LOGGER.h"
#include "TERMINAL.h"
#include "MANAGER.h"
#include <stdbool.h>

static uint8_t flagSwitch = 0;

static uint8_t TERMINAL1_input = 0;
static uint8_t TERMINAL2_input = 0;

static uint8_t time1_input_position=0;
static uint8_t date1_input_position=0;
static uint8_t log1_input_position=1;

static uint8_t time2_input_position=0;
static uint8_t date2_input_position=0;
static uint8_t log2_input_position=1;

static uint8_t log1_num = 0;
static uint8_t log1_time = 0;
static uint8_t log1_caps[2];
static uint8_t log1_step=0;
static uint8_t log1_yes = 0;
static uint8_t log1_no = 0;

static uint8_t log2_num = 0;
static uint8_t log2_time = 0;
static uint8_t log2_caps[2];
static uint8_t log2_step=0;
static uint8_t log2_yes = 0;
static uint8_t log2_no = 0;

UART_Flags_t TERMINAL1_flags;
UART_Flags_t TERMINAL2_flags;

void SM_init_flags()
{
	TERMINAL1_flags.options = true;
	TERMINAL1_flags.config = false;
	TERMINAL1_flags.set_time = false;
	TERMINAL1_flags.set_date = false;
	TERMINAL1_flags.log = false;
	TERMINAL1_flags.set_log = false;
	TERMINAL1_flags.read_log = false;
	TERMINAL1_flags.read_time = false;
	TERMINAL1_flags.read_date = false;
	TERMINAL1_flags.time = false;
	TERMINAL1_flags.date = false;
	TERMINAL1_flags.read_curr = false;

	TERMINAL2_flags.options = true;
	TERMINAL2_flags.config = false;
	TERMINAL2_flags.set_time = false;
	TERMINAL2_flags.set_date = false;
	TERMINAL2_flags.log = false;
	TERMINAL2_flags.set_log = false;
	TERMINAL2_flags.read_log = false;
	TERMINAL2_flags.read_time = false;
	TERMINAL2_flags.read_date = false;
	TERMINAL2_flags.time = false;
	TERMINAL2_flags.date = false;
	TERMINAL2_flags.read_curr = false;
}

void SM_state_machine()
{
	if(UART_get_mail_flag(TERMINAL_1_num))
	{
		TERMINAL1_input = UART_get_mail_box(TERMINAL_1_num);

		if (ESC_KEY == TERMINAL1_input)
		{
			MANAGER_update_seconds_terminal1_flag_off();
			TERMINAL_show_options(TERMINAL_1);
			TERMINAL1_flags.options = true;
			TERMINAL1_flags.config = false;
			TERMINAL1_flags.set_time = false;
			TERMINAL1_flags.set_date = false;
			TERMINAL1_flags.log = false;
			TERMINAL1_flags.set_log = false;
			TERMINAL1_flags.read_log = false;
			TERMINAL1_flags.read_time = false;
			TERMINAL1_flags.read_date = false;
			TERMINAL1_flags.read_curr = false;
			TERMINAL1_flags.time = false;
			TERMINAL1_flags.date = false;
			time1_input_position = 0;
			date1_input_position = 0;
			log1_input_position = 1;
			log1_step = 0;
			log1_yes = 0;
			log1_no = 0;
			log1_caps[0]=0;
			log1_caps[1]=0;
		}

		flagSwitch = TERMINAL1_flags.options | (TERMINAL1_flags.config << 1)
				| (TERMINAL1_flags.set_time << 2) | (TERMINAL1_flags.set_date << 3) | (TERMINAL1_flags.log << 4)
				| (TERMINAL1_flags.set_log << 5) | (TERMINAL1_flags.read_log << 6);


		switch (flagSwitch) {

			case OPTIONS_FLAG:
				switch(TERMINAL1_input)
				{
					case CONFIG:
						TERMINAL_show_config(TERMINAL_1);
						TERMINAL1_flags.config = true;
						TERMINAL1_flags.options = false;
						break;

					case LOG:
						TERMINAL_show_log(TERMINAL_1);
						TERMINAL1_flags.log = true;
						TERMINAL1_flags.options = false;
						break;

					default:
						break;
				}
				break;

			case CONFIG_FLAG:
				switch(TERMINAL1_input)
				{
					case SET_TIME:

						if(TERMINAL2_flags.time == true){
							TERMINAL_msg_busy(TERMINAL_1);
						}else
						{
							TERMINAL_show_set_time(TERMINAL_1);
							TERMINAL1_flags.set_time = true;
							TERMINAL1_flags.time = true;
							TERMINAL1_flags.config = false;
						}
						break;

					case SET_DATE:
						if(TERMINAL2_flags.date == true){
							TERMINAL_msg_busy(TERMINAL_1);
						}else
						{
							TERMINAL_show_set_date(TERMINAL_1);
							TERMINAL1_flags.set_date = true;
							TERMINAL1_flags.date = true;
							TERMINAL1_flags.config = false;
						}

						break;

					case READ_TIME:
						if(TERMINAL2_flags.time == true){
							TERMINAL_msg_busy(TERMINAL_1);
						}else
						{
							RTC_read_time();
							MANAGER_update_seconds_terminal1_flag();
							MANAGER_RTC_to_TERMINAL_time();
							TERMINAL_update_time(TERMINAL_1);
							TERMINAL_show_time(TERMINAL_1);
							TERMINAL1_flags.config = false;
							TERMINAL1_flags.read_time = true;
							TERMINAL1_flags.time = true;
						}
						break;

					case READ_DATE:
						if(TERMINAL2_flags.read_date == true){
							TERMINAL_msg_busy(TERMINAL_1);
						}else
						{
							RTC_read_date();
							MANAGER_RTC_to_TERMINAL_date();
							TERMINAL_update_date(TERMINAL_1);
							TERMINAL_show_date(TERMINAL_1);
							TERMINAL1_flags.config = false;
							TERMINAL1_flags.read_date = true;
							TERMINAL1_flags.date = true;
						}
						break;

					case READ_CURR:
						if(TERMINAL2_flags.read_curr == true){
							TERMINAL_msg_busy(TERMINAL_1);
						}else
						{
							INA219_read_curr();
							MANAGER_INA_to_TERMINAL_curr();
							TERMINAL_update_curr(TERMINAL_1);
							TERMINAL_show_curr(TERMINAL_1);
							TERMINAL1_flags.config = false;
							TERMINAL1_flags.read_curr = true;
						}
						break;

					default:
						break;
				}
				break;

			case LOG_FLAG:
				switch(TERMINAL1_input)
				{
					case SET_LOG:
						if(TERMINAL2_flags.set_log == true){
							TERMINAL_msg_busy(TERMINAL_1);
						}else
						{
							TERMINAL_show_set_log(TERMINAL_1);
							TERMINAL1_flags.set_log = true;
							TERMINAL1_flags.log = false;
						}
						break;

					case READ_LOG:
						if(TERMINAL2_flags.read_log == true){
							TERMINAL_msg_busy(TERMINAL_1);
						}else{
							TERMINAL_show_read_log(TERMINAL_1);
							TERMINAL1_flags.read_log = true;
							TERMINAL1_flags.log = false;
						}
						break;

					default:
						break;
				}
				break;

			case SET_TIME_FLAG:


					if((ENTER_KEY == TERMINAL1_input) && (time1_input_position == TIME_FORMAT_LENGTH))
					{
						MANAGER_TERMINAL_to_RTC_time();
						RTC_write_time();
						TERMINAL_msg_set_time(TERMINAL_1);
					}
					else if (time1_input_position < TIME_FORMAT_LENGTH)
					{
						UART_WriteByte(TERMINAL_1, TERMINAL1_input);

						if((time1_input_position==COLON_POSITION1)
								||(time1_input_position==COLON_POSITION2)){
							UART_WriteByte(TERMINAL_1, COLON);
						}

						TERMINAL_get_time()[time1_input_position] = TERMINAL1_input;
						time1_input_position++;
					}

				break;

			case SET_DATE_FLAG:

					if((ENTER_KEY == TERMINAL1_input) && (date1_input_position == DATE_FORMAT_LENGHT))
					{
						MANAGER_TERMINAL_to_RTC_date();
						RTC_write_date();
						TERMINAL_msg_set_date(TERMINAL_1);
					}
					else if (date1_input_position < DATE_FORMAT_LENGHT)
					{
						UART_WriteByte(TERMINAL_1, TERMINAL1_input);

						if((date1_input_position==SLASH_POSITION1)
								||(date1_input_position==SLASH_POSITION2)){
							UART_WriteByte(TERMINAL_1, SLASH);
						}
						TERMINAL_get_date()[date1_input_position] = TERMINAL1_input;
						date1_input_position++;
					}

				break;

			case SET_LOG_FLAG:
				if((ENTER_KEY == TERMINAL1_input) && (log1_input_position == LOG_FORMAT_LENGHT))
				{
					switch(log1_step)
					{
						case 0:
							TERMINAL_msg_set_log_confirm(TERMINAL_1);
							log1_input_position--;
							log1_step++;
							break;

						case 1:
							if(log1_yes){
								TERMINAL_show_set_log_time(TERMINAL_1);
								log1_input_position--;
								log1_step++;
							}else if(log1_no){
								TERMINAL_msg_set_log_cancel(TERMINAL_1);
								log1_step = 4;
							}
							break;

						case 2:

							TERMINAL_show_set_log_cap(TERMINAL_1);
							log1_input_position=0;
							log1_step++;
							break;

						case 3:
							TERMINAL_msg_set_log_done(TERMINAL_1);
							MANAGER_set_log(log1_num, log1_time, log1_caps[0], log1_caps[1]);
							log1_step++;
							log1_input_position = 1;

							break;

						default:
							break;
					}
				}
				else if (log1_input_position < LOG_FORMAT_LENGHT)
				{

					switch(log1_step)
					{
						case 0:
							log1_num = TERMINAL1_input;
							break;
						case 2:
							log1_time = TERMINAL1_input;
							break;
						case 3:
							log1_caps[log1_input_position] = TERMINAL1_input;
							break;
					}

					if(TERMINAL1_input==Y_KEY){
						log1_yes = 1;
					}else if(TERMINAL1_input==N_KEY){
						log1_no = 1;
					}
					UART_WriteByte(TERMINAL_1, TERMINAL1_input);
					log1_input_position++;
				}
				break;

			case READ_LOG_FLAG:
				if((ENTER_KEY == TERMINAL1_input) && (log1_input_position == LOG_FORMAT_LENGHT))
				{
					MANAGER_LOGGER_to_TERMINAL(log1_num, TERMINAL_1);
				}else if (log1_input_position < LOG_FORMAT_LENGHT)
				{
					log1_num = TERMINAL1_input;
					UART_WriteByte(TERMINAL_1, TERMINAL1_input);
					log1_input_position++;
				}

				break;

			default:
				break;
		}
	UART_clear_mail_flag(TERMINAL_1_num);
	}

	/////////////////

	/////////////////

	if(UART_get_mail_flag(TERMINAL_2_num))
	{
		TERMINAL2_input = UART_get_mail_box(TERMINAL_2_num);

		if (ESC_KEY == TERMINAL2_input)
		{
			MANAGER_update_seconds_terminal2_flag_off();
			TERMINAL_show_options(TERMINAL_2);
			TERMINAL2_flags.options = true;
			TERMINAL2_flags.config = false;
			TERMINAL2_flags.set_time = false;
			TERMINAL2_flags.set_date = false;
			TERMINAL2_flags.log = false;
			TERMINAL2_flags.set_log = false;
			TERMINAL2_flags.read_log = false;
			TERMINAL2_flags.read_time = false;
			TERMINAL2_flags.read_date = false;
			TERMINAL2_flags.read_curr = false;
			TERMINAL2_flags.time = false;
			TERMINAL2_flags.date = false;
			time2_input_position = 0;
			date2_input_position = 0;
			log2_input_position = 1;
			log2_step = 0;
			log2_yes = 0;
			log2_no = 0;
			log2_caps[0]=0;
			log2_caps[1]=0;
		}

		flagSwitch = TERMINAL2_flags.options | (TERMINAL2_flags.config << 1)
				| (TERMINAL2_flags.set_time << 2) | (TERMINAL2_flags.set_date << 3) | (TERMINAL2_flags.log << 4)
				| (TERMINAL2_flags.set_log << 5) | (TERMINAL2_flags.read_log << 6);


		switch (flagSwitch) {

			case OPTIONS_FLAG:
				switch(TERMINAL2_input)
				{
					case CONFIG:
						TERMINAL_show_config(TERMINAL_2);
						TERMINAL2_flags.config = true;
						TERMINAL2_flags.options = false;
						break;

					case LOG:
						TERMINAL_show_log(TERMINAL_2);
						TERMINAL2_flags.log = true;
						TERMINAL2_flags.options = false;
						break;

					default:
						break;
				}
				break;

			case CONFIG_FLAG:
				switch(TERMINAL2_input)
				{
					case SET_TIME:

						if(TERMINAL1_flags.time == true){
							TERMINAL_msg_busy(TERMINAL_2);
						}else
						{
							TERMINAL_show_set_time(TERMINAL_2);
							TERMINAL2_flags.set_time = true;
							TERMINAL2_flags.time = true;
							TERMINAL2_flags.config = false;
						}
						break;

					case SET_DATE:
						if(TERMINAL1_flags.date == true){
							TERMINAL_msg_busy(TERMINAL_2);
						}else
						{
							TERMINAL_show_set_date(TERMINAL_2);
							TERMINAL2_flags.set_date = true;
							TERMINAL2_flags.date = true;
							TERMINAL2_flags.config = false;
						}

						break;

					case READ_TIME:
						if(TERMINAL1_flags.time == true){
							TERMINAL_msg_busy(TERMINAL_2);
						}else
						{
							RTC_read_time();
							MANAGER_update_seconds_terminal2_flag();
							MANAGER_RTC_to_TERMINAL_time();
							TERMINAL_update_time(TERMINAL_2);
							TERMINAL_show_time(TERMINAL_2);
							TERMINAL2_flags.config = false;
							TERMINAL2_flags.read_time = true;
							TERMINAL2_flags.time = true;
						}
						break;

					case READ_DATE:
						if(TERMINAL1_flags.read_date == true){
							TERMINAL_msg_busy(TERMINAL_2);
						}else
						{
							RTC_read_date();
							MANAGER_RTC_to_TERMINAL_date();
							TERMINAL_update_date(TERMINAL_2);
							TERMINAL_show_date(TERMINAL_2);
							TERMINAL2_flags.config = false;
							TERMINAL2_flags.read_date = true;
							TERMINAL2_flags.date = true;
						}
						break;

					case READ_CURR:
						if(TERMINAL1_flags.read_curr == true){
							TERMINAL_msg_busy(TERMINAL_2);
						}else
						{
							INA219_read_curr();
							MANAGER_INA_to_TERMINAL_curr();
							TERMINAL_update_curr(TERMINAL_2);
							TERMINAL_show_curr(TERMINAL_2);
							TERMINAL2_flags.config = false;
							TERMINAL2_flags.read_curr = true;
						}
						break;

					default:
						break;
				}
				break;

			case LOG_FLAG:
				switch(TERMINAL2_input)
				{
					case SET_LOG:
						if(TERMINAL1_flags.set_log == true){
							TERMINAL_msg_busy(TERMINAL_2);
						}else
						{
							TERMINAL_show_set_log(TERMINAL_2);
							TERMINAL2_flags.set_log = true;
							TERMINAL2_flags.log = false;
						}
						break;

					case READ_LOG:
						if(TERMINAL1_flags.read_log == true){
							TERMINAL_msg_busy(TERMINAL_2);
						}else{
							TERMINAL_show_read_log(TERMINAL_2);
							TERMINAL2_flags.read_log = true;
							TERMINAL2_flags.log = false;
						}
						break;

					default:
						break;
				}
				break;

			case SET_TIME_FLAG:


					if((ENTER_KEY == TERMINAL2_input) && (time2_input_position == TIME_FORMAT_LENGTH))
					{
						MANAGER_TERMINAL_to_RTC_time();
						RTC_write_time();
						TERMINAL_msg_set_time(TERMINAL_2);
					}
					else if (time2_input_position < TIME_FORMAT_LENGTH)
					{
						UART_WriteByte(TERMINAL_2, TERMINAL2_input);

						if((time2_input_position==COLON_POSITION1)
								||(time2_input_position==COLON_POSITION2)){
							UART_WriteByte(TERMINAL_2, COLON);
						}

						TERMINAL_get_time()[time2_input_position] = TERMINAL2_input;
						time2_input_position++;
					}

				break;

			case SET_DATE_FLAG:

					if((ENTER_KEY == TERMINAL2_input) && (date2_input_position == DATE_FORMAT_LENGHT))
					{
						MANAGER_TERMINAL_to_RTC_date();
						RTC_write_date();
						TERMINAL_msg_set_date(TERMINAL_2);
					}
					else if (date2_input_position < DATE_FORMAT_LENGHT)
					{
						UART_WriteByte(TERMINAL_2, TERMINAL2_input);

						if((date2_input_position==SLASH_POSITION1)
								||(date2_input_position==SLASH_POSITION2)){
							UART_WriteByte(TERMINAL_2, SLASH);
						}
						TERMINAL_get_date()[date2_input_position] = TERMINAL2_input;
						date2_input_position++;
					}

				break;

			case SET_LOG_FLAG:
				if((ENTER_KEY == TERMINAL2_input) && (log2_input_position == LOG_FORMAT_LENGHT))
				{
					switch(log2_step)
					{
						case 0:
							TERMINAL_msg_set_log_confirm(TERMINAL_2);
							log2_input_position--;
							log2_step++;
							break;

						case 1:
							if(log2_yes){
								TERMINAL_show_set_log_time(TERMINAL_2);
								log2_input_position--;
								log2_step++;
							}else if(log2_no){
								TERMINAL_msg_set_log_cancel(TERMINAL_2);
								log2_step = 4;
							}
							break;

						case 2:

							TERMINAL_show_set_log_cap(TERMINAL_2);
							log2_input_position=0;
							log2_step++;
							break;

						case 3:
							TERMINAL_msg_set_log_done(TERMINAL_2);
							MANAGER_set_log(log2_num, log2_time, log2_caps[0], log2_caps[1]);
							log2_step++;
							log2_input_position = 1;

							break;

						default:
							break;
					}
				}
				else if (log2_input_position < LOG_FORMAT_LENGHT)
				{

					switch(log2_step)
					{
						case 0:
							log2_num = TERMINAL2_input;
							break;
						case 2:
							log2_time = TERMINAL2_input;
							break;
						case 3:
							log2_caps[log2_input_position] = TERMINAL2_input;
							break;
					}

					if(TERMINAL2_input==Y_KEY){
						log2_yes = 1;
					}else if(TERMINAL2_input==N_KEY){
						log2_no = 1;
					}
					UART_WriteByte(TERMINAL_2, TERMINAL2_input);
					log2_input_position++;
				}
				break;

			case READ_LOG_FLAG:
				if((ENTER_KEY == TERMINAL2_input) && (log2_input_position == LOG_FORMAT_LENGHT))
				{
					MANAGER_LOGGER_to_TERMINAL(log2_num, TERMINAL_2);
				}else if (log2_input_position < LOG_FORMAT_LENGHT)
				{
					log2_num = TERMINAL2_input;
					UART_WriteByte(TERMINAL_2, TERMINAL2_input);
					log2_input_position++;
				}

				break;

			default:
				break;
		}
	UART_clear_mail_flag(TERMINAL_2_num);
	}
}
