/*
 * CONFIG.h
 *
 *  Created on: 21 oct. 2023
 *      Author: HP}
 */

#ifndef CONFIG_H_
#define CONFIG_H_

#include "stdint.h"

#define COLON_POSITION1				1
#define COLON_POSITION2 			3
#define SLASH_POSITION1 			1
#define SLASH_POSITION2 			3
#define WDT_WINDOW					3000u
#define I2C_MASTER_CLK_FREQ         CLOCK_GetFreq(I2C0_CLK_SRC)


#define EXAMPLE_DSPI_MASTER_BASEADDR         SPI0
#define DSPI_MASTER_CLK_FREQ                 CLOCK_GetFreq(DSPI0_CLK_SRC)



typedef enum{FALSE, TRUE} boolean_t;


typedef enum {
    GPIOA_name,
	GPIOB_name,
	GPIOC_name,
	GPIOD_name,
	GPIOE_name
}GPIO_Name_t;

typedef struct {
    uint8_t flag;
    uint32_t time;
    uint8_t caps;
    uint32_t time_count;
    uint8_t caps_count;
} LOG_Data_t;



typedef enum {
	 TRANSFER_SIZE = 8U,
	 SOUNDS_BUFFER_SIZE	= 20u,
	 TRANSFER_BAUDRATE 	=	5000000U, /*! Transfer baudrate - 500k */
	 TRANSFER_BAUDRATE_MEM =	2000000U, /*! Transfer baudrate - 500k */


} SPI_config_t;





typedef enum {
	CE_LCD = 0,
	LCD_CMD = 0,
	LCD_DATA = 1,
	DATA_OR_CMD_PIN = 5,
	LCD_MARG = 5u,
	LCD_CHAR_WIDTH = 7u,
	RESET_PIN = 7,
	LCD_Y = 48,
	SCREENH = 48,
	LCD_X = 84,
	SCREENW = 84,
	IMAGE_SIZE = 504,

} LCD_config_t;

typedef enum {
    PIT_CHANNEL_0,
	PIT_CHANNEL_1,
	PIT_CHANNEL_2
}PIT_Channels_t;

typedef enum {
	rx_BUFFER_SIZE = 2u,
	CE_MEM = 3,
	READ_DATA = 0x03u,
	tx_BUFFER_SIZE = 4u,



} MEM_config_t;


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
	SLASH = 47,
	CHAR_TO_INT
}UART_Keys_t;

typedef enum {
	SET_TIME = 49,
	SET_DATE,
	READ_TIME,
	READ_DATE,
	READ_TEMP,
	READ_HUM,
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



typedef enum {
    UART_TENS_HOURS_INDEX,
    UART_UNITS_HOURS_INDEX,
    UART_TENS_MINUTES_INDEX,
    UART_UNITS_MINUTES_INDEX,
    UART_TENS_SECONDS_INDEX,
    UART_UNITS_SECONDS_INDEX
} UART_Time_Index_t;

typedef enum {
    UART_TENS_DAY_INDEX,
    UART_UNITS_DAY_INDEX,
    UART_TENS_MONTH_INDEX,
    UART_UNITS_MONTH_INDEX,
    UART_TENS_YEAR_INDEX,
    UART_UNITS_YEAR_INDEX
} UART_Date_Index_t;

typedef enum {
	I2C_SECONDS_INDEX,
    I2C_MINUTES_INDEX,
	I2C_HOURS_INDEX
} I2C_Time_Index_t;

typedef enum {
	I2C_YEAR_INDEX,
	I2C_MONTH_INDEX,
    I2C_DAY_INDEX
} I2C_Date_Index_t;


typedef enum {
	LOG_FORMAT_LENGHT = 2,
	TIME_FORMAT_LENGTH = 6,
	DATE_FORMAT_LENGHT = 6,
	HUM_STRING_LENGHT,
	TEMP_STRING_LENGHT,
	TIME_STRING_LENGHT,
	DATE_STRING_LENGHT = 11,
	EEPROM_FORMAT_LENGHT,
	I2C_DATA_LENGTH = 33,
	LOG_DATA_STRING_LENGHT = 52
}UART_lenght_t;

typedef enum {
	PIN0,
	PIN1,
	PIN2,
	PIN3,
	PIN4,//////////////////
	PIN14=14,
	PIN15,
	PIN16,
	PIN17
}Pin_t;

typedef enum {
	DHT_RESET = 0xAB,
	DHT_COMMAND_1 = 0xE1,
	DHT_COMMAND_2 = 0x08,
	DHT_COMMAND_3 = 0x00,
	DHT_STATUS = 0x18u,
	DHT_COMMAND_4 = 0xAC,
	DHT_COMMAND_5 = 0x33
}I2C_Commands_t;

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
    uint8_t read_hum : 1;
    uint8_t read_temp : 1;
    uint8_t time : 1;
    uint8_t date : 1;
} UART_Flags_t;

typedef enum {
    UART0_num,
	UART1_num,
	UART2_num,
	UART3_num,
	UART4_num
}UART_Num_t;

typedef struct{
	uint8_t flag; /** Flag to indicate that there is new data*/
	uint16_t mail_box; /** it contains the received data*/
} uart_mail_box_t;



typedef enum {
	I2C_RTC_ADDRESS = 0x68U,
	I2C_DHT_ADDRESS = 0x38U,
	I2C_EEPROM_ADDRESS = 0x50U,
	HOUR_SUBADDRESS = 0x02U,
	SQUAREWAVE_SUBADDRESS = 0x07U,
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
