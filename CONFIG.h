/*
 * CONFIG.h
 *
 *  Created on: 21 oct. 2023
 *      Author: HP}
 */

#ifndef CONFIG_H_
#define CONFIG_H_

#include "MK64F12.h"

#define COLON_POSITION1				1
#define COLON_POSITION2 			3
#define SLASH_POSITION1 			1
#define SLASH_POSITION2 			3
#define WDT_WINDOW					3000u
#define I2C_MASTER_CLK_FREQ         CLOCK_GetFreq(I2C0_CLK_SRC)
#define UART_CLK_FREQ  				CLOCK_GetFreq(UART0_CLK_SRC)

typedef enum {
	I2C_BAUDRATE = 400000U,
	UART_BAUDRATE = 115200U
}BAUDRATE_Value_t;

typedef enum {
	TENS_MASK = 0xF0,
	UNITS_MASK = 0x0F,
	LSBYTE_MASK = 0x08,
	MSBYTE_MASK = 0x80,
	HOUR_FORMAT_MASK = 0x40,
	SQUAREWAVE_FREQ_MASK = 0x10
}MASK_t;


typedef enum {
	ESC_KEY = 0x1B,
	ENTER_KEY = 0x0D,
	Y_KEY = 0x79,
	N_KEY = 0x6E,
	COLON = 58,
	DOT = 46,
	SLASH,
	CHAR_TO_INT
}UART_Keys_t;

typedef enum {
	SET_TIME = 49,
	SET_DATE,
	READ_TIME,
	READ_DATE,
	READ_CURR,
}Config_t;

typedef enum {
	SET_LOG = 49,
	READ_LOG,
}Log_t;

typedef enum {
	CONFIG = 49,
	LOG,
}Options_t;

typedef enum {
    OPTIONS_FLAG = 0x01,
    CONFIG_FLAG = 0x02,
    SET_TIME_FLAG = 0x04,
    SET_DATE_FLAG = 0x08,
    LOG_FLAG = 0x10,
	SET_LOG_FLAG = 0x20,
	READ_LOG_FLAG = 0x40
} UART_Flags_name_t;

typedef struct {
    uint8_t options : 1;  // Bit 0
    uint8_t config : 1;   // Bit 1
    uint8_t set_time : 1; // Bit 2
    uint8_t set_date : 1; // Bit 3
    uint8_t log : 1;      // Bit 4
    uint8_t set_log : 1;
    uint8_t read_log : 1;
    uint8_t read_time : 1;
    uint8_t read_date : 1;
    uint8_t time : 1;
    uint8_t date : 1;
    uint8_t read_curr : 1;
} UART_Flags_t;

typedef struct {
    uint8_t flag;
    uint32_t time;
    uint8_t caps;
    uint32_t time_count;
    uint8_t caps_count;
} LOG_Data_t;

typedef struct{
	uint8_t address;
	uint16_t subaddress;
	uint8_t subaddressSize;
	uint8_t *data;
	uint8_t dataSize;
}I2C_Device_t;

typedef enum {
    TERMINAL_TENS_HOURS_INDEX,
    TERMINAL_UNITS_HOURS_INDEX,
    TERMINAL_TENS_MINUTES_INDEX,
    TERMINAL_UNITS_MINUTES_INDEX,
    TERMINAL_TENS_SECONDS_INDEX,
    TERMINAL_UNITS_SECONDS_INDEX
} TERMINAL_Time_Index_t;

typedef enum {
    TERMINAL_TENS_DAY_INDEX,
    TERMINAL_UNITS_DAY_INDEX,
    TERMINAL_TENS_MONTH_INDEX,
    TERMINAL_UNITS_MONTH_INDEX,
    TERMINAL_TENS_YEAR_INDEX,
    TERMINAL_UNITS_YEAR_INDEX
} TERMINAL_Date_Index_t;

typedef enum {
	RTC_SECONDS_INDEX,
    RTC_MINUTES_INDEX,
	RTC_HOURS_INDEX
} I2C_Time_Index_t;

typedef enum {
	RTC_YEAR_INDEX,
	RTC_MONTH_INDEX,
    RTC_DAY_INDEX
} I2C_Date_Index_t;

typedef struct{
	uint8_t flag; /** Flag to indicate that there is new data*/
	uint16_t mail_box; /** it contains the received data*/
} uart_mail_box_t;

typedef enum {
	LOG_FORMAT_LENGHT = 2,
	CURR_FORMAT_LENGHT = 4,
	TIME_FORMAT_LENGTH = 6,
	DATE_FORMAT_LENGHT = 6,
	EEPROM_FORMAT_LENGHT = 8,
	LOG_DATA_FORMAT_LENGHT = 16,
	TIME_STRING_LENGHT = 11,
	DATE_STRING_LENGHT = 11,
	CURR_STRING_LENGHT = 5,
	I2C_DATA_LENGTH = 33,
	LOG_DATA_STRING_LENGHT = 52
}TERMINAL_lenght_t;

typedef enum {
	PIN0,
	PIN1,
	PIN2,
	PIN3,
	PIN14=14,
	PIN15,
	PIN16,
	PIN17
}Pin_t;

typedef enum {
    UART0_num,
	UART1_num,
	UART2_num,
	UART3_num,
	UART4_num,
}UART_Num_t;

typedef enum {
    TERMINAL_1_num = UART0_num,
	TERMINAL_2_num = UART4_num,
}TERMINAL_Num_t;


#define TERMINAL_1 UART0
#define	TERMINAL_2 UART4

typedef enum {
	RTC_ADDRESS = 0x68U,
	RTC_TIME_SUBADDRESS = 0x00U,
	RTC_HOUR_SUBADDRESS = 0x02U,
	RTC_DATE_SUBADDRESS = 0x04U,
	RTC_SQUAREWAVE_SUBADDRESS = 0x07U,
	EEPROM_ADDRESS = 0x50U,
	INA219_ADDRESS = 0x40U,
	INA219_CURR_SUBADDRESS = 0x03U
}I2C_Address_t;


typedef enum {
    LOG_1=1,
	LOG_2,
	LOG_3,
	LOG_4,
	LOG_5
}LOG_t;

typedef enum {
	LOG_1_ADDRESS = 0x000,
	LOG_2_ADDRESS = 0x258U,
	LOG_3_ADDRESS = 0x4B0U,
	LOG_4_ADDRESS = 0x708U,
	LOG_5_ADDRESS = 0x960U,
	LOG_1_ADD_CAP = 0xBB9U,
	LOG_2_ADD_CAP = 0xBC9U,
	LOG_3_ADD_CAP = 0xBD9U,
	LOG_4_ADD_CAP = 0xBE9U,
	LOG_5_ADD_CAP = 0xBF9U
}LOG_Address_t;
#endif /* CONFIG_H_ */
