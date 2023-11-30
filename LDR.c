/*
 * LDR.c
 *
 *  Created on: 26 nov. 2023
 *      Author: HP}
 */

#include "LDR.h"
#include "CONFIG.h"
#include "ADC.h"
#include "GPIO.h"
#include "fsl_port.h"
#include "fsl_gpio.h"
#include "fsl_adc16.h"

void LDR_config()
{

	CLOCK_EnableClock(kCLOCK_PortB);
	CLOCK_EnableClock(kCLOCK_PortE);

	 port_pin_config_t config = {
		kPORT_PullUp,
		kPORT_SlowSlewRate,
		kPORT_PassiveFilterDisable,
		kPORT_OpenDrainDisable,
		kPORT_HighDriveStrength,
		0,
		kPORT_UnlockRegister
	};

	PORT_SetPinConfig(PORTB, PIN2, &config);
	PORT_SetPinConfig(PORTB, PIN3, &config);
	PORT_SetPinConfig(PORTE, PIN24, &config);
	PORT_SetPinConfig(PORTE, PIN25, &config);
	GPIO_pin_init_io(GPIOE, PIN24, 0);
	GPIO_pin_init_io(GPIOE, PIN25, 0);
	GPIO_pin_init_io(GPIOB, PIN2, 0);
	GPIO_pin_init_io(GPIOB, PIN3, 0);
}

uint16_t LDR_read(uint8_t ldr)
{
	return ADC_set_conversion(ldr);
}


