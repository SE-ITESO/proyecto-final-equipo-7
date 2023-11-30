/*
 * ADC.c
 *
 *  Created on: 26 nov. 2023
 *      Author: HP}
 */

#include "ADC.h"
#include "CONFIG.h"
#include "fsl_adc16.h"

adc16_channel_config_t adcChannelConfig;

void ADC_init()
{
	adc16_config_t adcConfig;
	ADC16_GetDefaultConfig(&adcConfig);
	ADC16_Init(ADC0, &adcConfig);
	ADC16_EnableHardwareTrigger(ADC0, false);
	adcChannelConfig.enableInterruptOnConversionCompleted = false;
}

uint16_t ADC_set_conversion(uint8_t channel)
{
	adcChannelConfig.channelNumber = channel;
	ADC16_SetChannelConfig(ADC0, 0, &adcChannelConfig);
	while (0U == (kADC16_ChannelConversionDoneFlag & ADC16_GetChannelStatusFlags(ADC0, 0)))
				{
				}
	return ADC16_GetChannelConversionValue(ADC0, 0);
}
