/*
 * LOGGER.c
 *
 *  Created on: 20 nov. 2023
 *      Author: HP}
 */

#include "LOGGER.h"
#include "CONFIG.h"
#include "I2C.h"
#include "RTC.h"
#include "INA219.h"


uint8_t g_logger_data_1[EEPROM_FORMAT_LENGHT] = {0,0,0,0,0,0,0,0};
uint8_t g_logger_data_2[EEPROM_FORMAT_LENGHT] = {0,0,0,0,0,0,0,0};
uint8_t g_logger_data_3[EEPROM_FORMAT_LENGHT] = {0,0,0,0,0,0,0,0};
uint8_t g_logger_data_4[EEPROM_FORMAT_LENGHT] = {0,0,0,0,0,0,0,0};
uint8_t g_logger_data_5[EEPROM_FORMAT_LENGHT] = {0,0,0,0,0,0,0,0};

I2C_Device_t eeprom;

void LOGGER_set_device()
{
	eeprom.address = EEPROM_ADDRESS;
	eeprom.subaddressSize = 2;
	eeprom.dataSize = EEPROM_FORMAT_LENGHT;
}

void LOGGER_write_data(uint8_t log, uint8_t cap){

    switch (log) {
        case LOG_1:
        	eeprom.subaddress = LOG_1_ADDRESS + EEPROM_FORMAT_LENGHT*cap;
        	eeprom.data = g_logger_data_1;
        	break;
        case LOG_2:
        	eeprom.subaddress = LOG_2_ADDRESS + EEPROM_FORMAT_LENGHT*cap;
        	eeprom.data = g_logger_data_2;
        	break;
        case LOG_3:
        	eeprom.subaddress = LOG_3_ADDRESS + EEPROM_FORMAT_LENGHT*cap;
        	eeprom.data = g_logger_data_3;
        	break;
        case LOG_4:
        	eeprom.subaddress = LOG_4_ADDRESS + EEPROM_FORMAT_LENGHT*cap;
        	eeprom.data = g_logger_data_4;
        	break;
        case LOG_5:
        	eeprom.subaddress = LOG_5_ADDRESS + EEPROM_FORMAT_LENGHT*cap;
        	eeprom.data = g_logger_data_5;
        	break;
        default:
        	break;
    }

    eeprom.data[0] = INA219_get_curr()[0];
    eeprom.data[1] = INA219_get_curr()[1];
    eeprom.data[2] = RTC_get_time()[RTC_SECONDS_INDEX];
    eeprom.data[3] = RTC_get_time()[RTC_MINUTES_INDEX];
    eeprom.data[4] = RTC_get_time()[RTC_HOURS_INDEX];
    eeprom.data[5] = RTC_get_date()[RTC_DAY_INDEX];
    eeprom.data[6] = RTC_get_date()[RTC_MONTH_INDEX];
    eeprom.data[7] = RTC_get_date()[RTC_YEAR_INDEX];
    eeprom.dataSize = EEPROM_FORMAT_LENGHT;
	I2C_write(eeprom);
}

void LOGGER_read_data(uint8_t log, uint8_t cap)
{
	switch (log) {
		case LOG_1:
			eeprom.subaddress = LOG_1_ADDRESS + EEPROM_FORMAT_LENGHT*cap;
			eeprom.data = g_logger_data_1;
			break;
		case LOG_2:
			eeprom.subaddress = LOG_2_ADDRESS + EEPROM_FORMAT_LENGHT*cap;
			eeprom.data = g_logger_data_2;
			break;
		case LOG_3:
			eeprom.subaddress = LOG_3_ADDRESS + EEPROM_FORMAT_LENGHT*cap;
			eeprom.data = g_logger_data_3;
			break;
		case LOG_4:
			eeprom.subaddress = LOG_4_ADDRESS + EEPROM_FORMAT_LENGHT*cap;
			eeprom.data = g_logger_data_4;
			break;
		case LOG_5:
			eeprom.subaddress = LOG_5_ADDRESS + EEPROM_FORMAT_LENGHT*cap;
			eeprom.data = g_logger_data_5;
			break;
		default:
			break;
	}
	eeprom.dataSize = EEPROM_FORMAT_LENGHT;
    I2C_read(eeprom);
}

uint8_t* LOGGER_get_data(uint8_t log)
{
    switch (log) {
        case LOG_1:
            return g_logger_data_1;
        case LOG_2:
            return g_logger_data_2;
        case LOG_3:
            return g_logger_data_3;
        case LOG_4:
            return g_logger_data_4;
        case LOG_5:
            return g_logger_data_5;
        default:
            return 0;
    }
}

void LOGGER_write_caps(uint8_t log, uint8_t caps)
{
    switch (log) {
        case LOG_1:
        	eeprom.subaddress = LOG_1_ADD_CAP;
        	break;
        case LOG_2:
        	eeprom.subaddress = LOG_2_ADD_CAP;
        	break;
        case LOG_3:
        	eeprom.subaddress = LOG_3_ADD_CAP;
        	break;
        case LOG_4:
        	eeprom.subaddress = LOG_4_ADD_CAP;
        	break;
        case LOG_5:
        	eeprom.subaddress = LOG_5_ADD_CAP;
        	break;
        default:
            break;
    }
    eeprom.data[0] = caps;
    eeprom.dataSize = 1;
    I2C_write(eeprom);

}

uint8_t LOGGER_read_caps(uint8_t log)
{
	uint8_t savedCaps[1];

    switch (log) {
        case LOG_1:
        	eeprom.subaddress = LOG_1_ADD_CAP;
        	break;
        case LOG_2:
        	eeprom.subaddress = LOG_2_ADD_CAP;
        	break;
        case LOG_3:
        	eeprom.subaddress = LOG_3_ADD_CAP;
        	break;
        case LOG_4:
        	eeprom.subaddress = LOG_4_ADD_CAP;
        	break;
        case LOG_5:
        	eeprom.subaddress = LOG_5_ADD_CAP;
        	break;
        default:
            break;
    }
    eeprom.data = savedCaps;
    eeprom.dataSize = 1;
    I2C_read(eeprom);

    return savedCaps[0];

}
