/*
 * FTM.c
 *
 *  Created on: 24 nov. 2023
 *      Author: HP}
 */

#include "FTM.h"
#include "CONFIG.h"
#include "fsl_gpio.h"
#include "fsl_port.h"
#include "stdint.h"

ftm_config_t ftmConfig;


void FTM_init()
{
	CLOCK_EnableClock(kCLOCK_PortC);
	const gpio_pin_config_t gpio_output_config =
			{
					kGPIO_DigitalOutput,
					1
			};
	GPIO_PinInit(GPIOC,PIN1,&gpio_output_config);
	PORT_SetPinMux(PORTC, PIN1, kPORT_MuxAlt4);
	GPIO_PinInit(GPIOC,PIN2,&gpio_output_config);
	PORT_SetPinMux(PORTC, PIN2, kPORT_MuxAlt4);


	/** Clock gating for the FlexTimer 0*/
	SIM->SCGC6 |= FLEX_TIMER_0_CLOCK_GATING;
	/**When write protection is enabled (WPDIS = 0), write protected bits cannot be written.
	* When write protection is disabled (WPDIS = 1), write protected bits can be written.*/
	FTM0->MODE |= FLEX_TIMER_WPDIS;
	/**Enables the writing over all registers*/
	FTM0->MODE &= ~FLEX_TIMER_FTMEN;
	/**Assigning a default value for modulo register*/
	FTM0->MOD = 0x00FF;
	/**Selects the Edge-Aligned PWM mode mode*/
	FTM0->CONTROLS[0].CnSC = FLEX_TIMER_MSB | FLEX_TIMER_ELSB;
	/**Assign a duty cycle of 50%*/
	FTM0->CONTROLS[0].CnV = FTM0->MOD/2;
	/**Configure the times*/
	FTM0->SC = FLEX_TIMER_CLKS_1|FLEX_TIMER_PS_128;

	FTM0->CONTROLS[1].CnSC = FLEX_TIMER_MSB | FLEX_TIMER_ELSB;
	/**Assign a duty cycle of 0%*/
	FTM0->CONTROLS[1].CnV = 0;

}

void FTM0_ISR()
{
	FTM0->SC &= ~FLEX_TIMER_TOF;
	GPIOD->PDOR ^= 0xFF;
}

void FTM_set_PWM(FTM_Type *ftm, uint8_t channel, uint8_t value)
{
	/**Assigns a new value for the duty cycle*/
	ftm->CONTROLS[channel].CnV = value;
}

