/*
 * Copyright 2016-2023 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/**
 * @file    FINAL.c
 * @brief   Application entry point.
 */

#include <stdbool.h>




#include "INA219.h"
#include "I2C.h"
#include "LCD_nokia.h"
#include "SPI.h"
#include "UART.h"
#include "NVIC.h"
#include "fsl_uart.h"

#include "MANAGER.h"
#include "CONFIG.h"









int main(void) {




    SPI_config();
    I2C_init();
    INA219_config();
    LCD_nokia_init();
	I2C_rtc_squarewave();
	UART_init();
	UART_show_options(UART0);


    /* Enable RX interrupt. */
    UART_EnableInterrupts(UART0, kUART_RxDataRegFullInterruptEnable | kUART_RxOverrunInterruptEnable);


    NVIC_set_basepri_threshold(PRIORITY_7);
    NVIC_enable_interrupt_and_priority(UART0_IRQ, PRIORITY_2);

    //NVIC_enable_interrupt_and_priority(PORTD_IRQ, PRIORITY_5);

    NVIC_global_enable_interrupts;

    uint8_t* uart_rx_time = UART_get_rx_time();
    	uint8_t* uart_rx_date = UART_get_rx_date();

    	UART_Flags_t UART0_flags;
    	UART0_flags.options = true;
    	UART0_flags.config = false;
    	UART0_flags.set_time = false;
    	UART0_flags.set_date = false;
    	UART0_flags.log = false;
    	UART0_flags.set_log = false;
    	UART0_flags.read_log = false;
    	UART0_flags.read_time = false;
    	UART0_flags.read_date = false;
    	UART0_flags.read_hum = false;
    	UART0_flags.read_temp = false;
    	UART0_flags.time = false;
    	UART0_flags.date = false;


    	uint8_t flagSwitch = 0;

    	uint8_t UART0_input = 0;


        uint8_t time_input_position=0;
        uint8_t date_input_position=0;
        uint8_t log_input_position=1;



        uint8_t log_num = 0;
        uint8_t log_time = 0;
        uint8_t log_caps[2];
        uint8_t log_step=0;
        uint8_t log_yes = 0;
        uint8_t log_no = 0;



    while (1) {
    	MANAGER_handler_log();
    	//MANAGER_UART();
    	if(UART_get_mail_flag(UART0_num))
    	    			{
    	    	    		UART0_input = UART_get_mail_box(UART0_num);

    	    	    		if (ESC_KEY == UART0_input)
    	    	    		{
    	    	    			MANAGER_update_seconds_uart0_flag_off();
    	    	    			UART_show_options(UART0);
    	    					UART0_flags.options = true;
    	    				    UART0_flags.config = false;
    	    				    UART0_flags.set_time = false;
    	    				    UART0_flags.set_date = false;
    	    				    UART0_flags.log = false;
    	    				    UART0_flags.set_log = false;
    	    				    UART0_flags.read_log = false;
    	    					UART0_flags.read_time = false;
    	    					UART0_flags.read_date = false;
    	    					UART0_flags.read_hum = false;
    	    					UART0_flags.read_temp = false;
    	    					UART0_flags.time = false;
    	    					UART0_flags.date = false;
    	    					time_input_position = 0;
    	    					date_input_position = 0;
    	    					log_input_position = 1;
    	    					log_step = 0;
    	    					log_yes = 0;
    	    					log_no = 0;
    	    					log_caps[0]=0;
    	    					log_caps[1]=0;
    	    	    		}

    	    	    		flagSwitch = UART0_flags.options | (UART0_flags.config << 1)
    	    	    				| (UART0_flags.set_time << 2) | (UART0_flags.set_date << 3) | (UART0_flags.log << 4)
    	    						| (UART0_flags.set_log << 5) | (UART0_flags.read_log << 6);


    	    	    		switch (flagSwitch) {

    	    					case OPTIONS_FLAG:
    	    						switch(UART0_input)
    	    						{
    	    							case CONFIG:
    	    								UART_show_config(UART0);
    	    								UART0_flags.config = true;
    	    								UART0_flags.options = false;
    	    								break;

    	    							case LOG:
    	    								I2C_rtc_squarewave();
    	    								if(I2C_get_transfer_success_flag()){
    	    									UART_show_log(UART0);
    	    									UART0_flags.log = true;
    	    									UART0_flags.options = false;
    	    								}else{
    	    									UART_show_error_i2c(UART0);
    	    								}
    	    								break;

    	    							default:
    	    								break;
    	    						}
    	    						break;

    	    					case CONFIG_FLAG:
    	    						switch(UART0_input)
    	    						{
    	    							case SET_TIME:

    	    								I2C_rtc_squarewave();
    	    								if(I2C_get_transfer_success_flag()){

    	    										UART_show_set_time(UART0);
    	    										UART0_flags.set_time = true;
    	    										UART0_flags.time = true;
    	    										UART0_flags.config = false;

    	    								}else{
    	    									UART_show_error_i2c(UART0);
    	    								}
    	    								break;

    	    							case SET_DATE:
    	    								I2C_rtc_squarewave();
    	    								if(I2C_get_transfer_success_flag()){

    	    										UART_show_set_date(UART0);
    	    										UART0_flags.set_date = true;
    	    										UART0_flags.date = true;
    	    										UART0_flags.config = false;

    	    								}else{
    	    									UART_show_error_i2c(UART0);
    	    								}
    	    								break;

    	    							case READ_TIME:

    	    									I2C_read_rtc_time();
    	    									if(I2C_get_transfer_success_flag()){
    	    										MANAGER_update_seconds_uart0_flag();
    	    										MANAGER_I2C_to_UART_time();
    	    										UART_update_time(UART0);
    	    										LCD_nokia_show_time();///prueba
    	    										UART_show_time(UART0);
    	    										UART0_flags.config = false;
    	    										UART0_flags.read_time = true;
    	    										UART0_flags.time = true;
    	    									}else{
    	    										UART_show_error_i2c(UART0);
    	    									}

    	    								break;

    	    							case READ_DATE:

    	    									I2C_read_rtc_date();
    	    									if(I2C_get_transfer_success_flag()){
    	    										MANAGER_I2C_to_UART_date();
    	    										UART_update_date(UART0);
    	    										LCD_nokia_show_data();
    	    									//	LCD_nokia_show_date();/////////////prueba para mostrar fecha en pantalla
    	    										UART_show_date(UART0);
    	    										UART0_flags.config = false;
    	    										UART0_flags.read_date = true;
    	    										UART0_flags.date = true;
    	    									}else{
    	    										UART_show_error_i2c(UART0);
    	    									}

    	    								break;



    	    							default:
    	    								break;
    	    						}
    	    						break;

    	    					case LOG_FLAG:
    	    						switch(UART0_input)
    	    						{
    	    							case SET_LOG:

    	    									UART_show_set_log(UART0);
    	    									UART0_flags.set_log = true;
    	    									UART0_flags.log = false;

    	    								break;

    	    							case READ_LOG:

    	    									UART_show_read_log(UART0);
    	    									UART0_flags.read_log = true;
    	    									UART0_flags.log = false;

    	    								break;

    	    							default:
    	    								break;
    	    						}
    	    						break;

    	    					case SET_TIME_FLAG:


    	    							if((ENTER_KEY == UART0_input) && (time_input_position == TIME_FORMAT_LENGTH))
    	    							{
    	    								MANAGER_UART_to_I2C_time();
    	    								I2C_write_rtc_time();
    	    								if(I2C_get_transfer_success_flag()){
    	    									UART_msg_set_time(UART0);
    	    								}else{
    	    									UART_show_error_i2c(UART0);
    	    								}
    	    							}
    	    							else if (time_input_position < TIME_FORMAT_LENGTH)
    	    							{
    	    								UART_WriteByte(UART0, UART0_input);

    	    								if((time_input_position==COLON_POSITION1)
    	    										||(time_input_position==COLON_POSITION2)){
    	    									UART_WriteByte(UART0, COLON);
    	    								}

    	    								uart_rx_time[time_input_position] = UART0_input;
    	    								time_input_position++;
    	    							}

    	    						break;

    	    					case SET_DATE_FLAG:

    	    							if((ENTER_KEY == UART0_input) && (date_input_position == DATE_FORMAT_LENGHT))
    	    							{
    	    								MANAGER_UART_to_I2C_date();
    	    								I2C_write_rtc_date();
    	    								if(I2C_get_transfer_success_flag()){
    	    									UART_msg_set_date(UART0);
    	    								}else{
    	    									UART_show_error_i2c(UART0);
    	    								}
    	    							}
    	    							else if (date_input_position < DATE_FORMAT_LENGHT)
    	    							{
    	    								UART_WriteByte(UART0, UART0_input);

    	    								if((date_input_position==SLASH_POSITION1)
    	    										||(date_input_position==SLASH_POSITION2)){
    	    									UART_WriteByte(UART0, SLASH);
    	    								}
    	    								uart_rx_date[date_input_position] = UART0_input;
    	    								date_input_position++;
    	    							}

    	    						break;

    	    					case SET_LOG_FLAG:
    	    						if((ENTER_KEY == UART0_input) && (log_input_position == LOG_FORMAT_LENGHT))
    	    						{
    	    							switch(log_step)
    	    							{
    	    								case 0:
    	    									UART_msg_set_log_confirm(UART0);
    	    									log_input_position--;
    	    									log_step++;
    	    									break;

    	    								case 1:
    	    									if(log_yes){
    	    										UART_show_set_log_time(UART0);
    	    										log_input_position--;
    	    										log_step++;
    	    									}else if(log_no){
    	    										UART_msg_set_log_cancel(UART0);
    	    										log_step = 4;
    	    									}
    	    									break;

    	    								case 2:

    	    									UART_show_set_log_cap(UART0);
    	    									log_input_position=0;
    	    									log_step++;
    	    									break;

    	    								case 3:
    	    									UART_msg_set_log_done(UART0);
    	    									MANAGER_set_log(log_num, log_time, log_caps[0], log_caps[1]);
    	    									log_step++;
    	    									log_input_position = 1;

    	    									break;

    	    								default:
    	    									break;
    	    							}
    	    						}
    	    						else if (log_input_position < LOG_FORMAT_LENGHT)
    	    						{

    	    							switch(log_step)
    	    							{
    	    								case 0:
    	    									log_num = UART0_input;
    	    									break;
    	    								case 2:
    	    									log_time = UART0_input;
    	    									break;
    	    								case 3:
    	    									log_caps[log_input_position] = UART0_input;
    	    									break;
    	    							}

    	    							if(UART0_input==Y_KEY){
    	    								log_yes = 1;
    	    							}else if(UART0_input==N_KEY){
    	    								log_no = 1;
    	    							}
    	    							UART_WriteByte(UART0, UART0_input);
    	    							log_input_position++;
    	    						}
    	    						break;

    	    					case READ_LOG_FLAG:
    	    						if((ENTER_KEY == UART0_input) && (log_input_position == LOG_FORMAT_LENGHT))
    	    						{
    	    							MANAGER_I2C_to_UART_log(log_num,UART0);
    	    							if(!I2C_get_transfer_success_flag()){
    	    								UART_show_error_i2c(UART0);
    	    							}
    	    						}else if (log_input_position < LOG_FORMAT_LENGHT)
    	    						{
    	    							log_num = UART0_input;
    	    							UART_WriteByte(UART0, UART0_input);
    	    							log_input_position++;
    	    						}

    	    						break;

    	    					default:
    	    						break;
    	    	    		}
    	    	    	UART_clear_mail_flag(UART0_num);
    	    			}

    }
}






