/*
 * RTC.c
 *
 *  Created on: 20 nov. 2023
 *      Author: HP}
 */

#include "RTC.h"
#include "CONFIG.h"
#include "I2C.h"
#include "fsl_i2c.h"
#include "fsl_port.h"
#include "fsl_gpio.h"

uint8_t g_rtc_time[3] = {0,0,0};
uint8_t g_rtc_date[3] = {0,0,0};
uint8_t g_rtc_config[1] = {0};
I2C_Device_t rtc;

void RTC_set_device_squarewave()
{
	g_rtc_config[0] = SQUAREWAVE_FREQ_MASK;
	rtc.address = RTC_ADDRESS;
	rtc.subaddress = RTC_SQUAREWAVE_SUBADDRESS;
	rtc.subaddressSize = 1;
	rtc.data = g_rtc_config;
	rtc.dataSize = 1;
}

void RTC_set_device_time()
{
	rtc.address = RTC_ADDRESS;
	rtc.subaddress = RTC_TIME_SUBADDRESS;
	rtc.subaddressSize = 1;
	rtc.data = g_rtc_time;
	rtc.dataSize = 3;
}

void RTC_set_device_date()
{
	rtc.address = RTC_ADDRESS;
	rtc.subaddress = RTC_DATE_SUBADDRESS;
	rtc.subaddressSize = 1;
	rtc.data = g_rtc_date;
	rtc.dataSize = 3;
}

void RTC_set_squarewave(){

	RTC_set_device_squarewave();
	I2C_write(rtc);
}

void RTC_config()
{
    port_pin_config_t config = {
        kPORT_PullUp,
        kPORT_SlowSlewRate,
        kPORT_PassiveFilterDisable,
        kPORT_OpenDrainDisable,
        kPORT_HighDriveStrength,
        kPORT_MuxAsGpio,
        kPORT_UnlockRegister
    };
    gpio_pin_config_t interrupt_config = {
        kGPIO_DigitalInput,
        0,
    };
    CLOCK_EnableClock(kCLOCK_PortB);

    PORT_SetPinConfig(PORTB, PIN10, &config);
    GPIO_PinInit(GPIOB, PIN10, &interrupt_config);
    PORT_SetPinInterruptConfig(PORTB, PIN10, kPORT_InterruptRisingEdge);

    g_rtc_config[0] = HOUR_FORMAT_MASK;
	rtc.address = RTC_ADDRESS;
	rtc.subaddress = RTC_HOUR_SUBADDRESS;
	rtc.subaddressSize = 1;
	rtc.data = g_rtc_config;
	rtc.dataSize = 1;
    I2C_write(rtc);
    RTC_set_squarewave();
}
void RTC_write_time(){
	RTC_set_device_time();
	I2C_write(rtc);
}

void RTC_read_time(){
	RTC_set_device_time();
	I2C_read(rtc);
}

void RTC_write_date(){

	RTC_set_device_date();
	I2C_write(rtc);
}

void RTC_read_date(){

	RTC_set_device_date();
	I2C_read(rtc);
}

uint8_t* RTC_get_time() {
    return g_rtc_time;
}

uint8_t* RTC_get_date() {
    return g_rtc_date;
}
