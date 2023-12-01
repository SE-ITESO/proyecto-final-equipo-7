/*
 * @file 	ADC.c
 *
 * @Authors Diego Delgado
 * 			Alberto Quintana
 *
 * @brief  	This file initializes ADC, which will allow us to read values from LDR'S
 *
 *
 */

//
/*
 * ADC Module
 * This module provides functions to initialize and perform ADC conversions.
 */

#include "ADC.h"
#include "CONFIG.h"
#include "fsl_adc16.h"

// Configuration structure for ADC channels
adc16_channel_config_t adcChannelConfig;

/*
 * Function: ADC_init
 * -------------------
 * Initializes ADC settings.
 * Configures ADC with default settings, disables hardware trigger, and interrupts on conversion completed.
 */
void ADC_init()
{
    adc16_config_t adcConfig;

    // Get default ADC configuration
    ADC16_GetDefaultConfig(&adcConfig);

    // Initialize ADC with default configuration
    ADC16_Init(ADC0, &adcConfig);

    // Disable hardware trigger for ADC
    ADC16_EnableHardwareTrigger(ADC0, false);

    // Disable interrupt on conversion completed
    adcChannelConfig.enableInterruptOnConversionCompleted = false;
}

/*

 *
 * Performs ADC conversion on the specified channel.
 * Sets ADC channel for conversion, waits for completion, and returns the conversion value.
 *
 *
 */
uint16_t ADC_set_conversion(uint8_t channel)
{
    // Set ADC channel for conversion
    adcChannelConfig.channelNumber = channel;
    ADC16_SetChannelConfig(ADC0, 0, &adcChannelConfig);

    // Wait for the conversion to be completed
    while (0U == (kADC16_ChannelConversionDoneFlag & ADC16_GetChannelStatusFlags(ADC0, 0)))
    {
        // Wait for conversion completion
    }


    return ADC16_GetChannelConversionValue(ADC0, 0);
}
