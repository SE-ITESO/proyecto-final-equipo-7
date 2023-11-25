/*
 * PWM.c
 *
 *  Created on: 24 nov. 2023
 *      Author: HP}
 */

#include "PWM.h"
#include "fsl_ftm.h"

#define PWM_FREQ_HZ 490

ftm_chnl_pwm_signal_param_t ftmParam;

void PWM_config()
{
    ftmParam.chnlNumber = kFTM_Chnl_0;
    ftmParam.level = kFTM_HighTrue;
    ftmParam.dutyCyclePercent = 0;
    FTM_SetupPwm(FTM0, &ftmParam, 1, kFTM_EdgeAlignedPwm, PWM_FREQ_HZ, CLOCK_GetFreq(kCLOCK_BusClk));
    FTM_SetupPwm(FTM1, &ftmParam, 1, kFTM_EdgeAlignedPwm, PWM_FREQ_HZ, CLOCK_GetFreq(kCLOCK_BusClk));
}

void PWM_set_dutycycle(FTM_Type *ftm, uint8_t value)
{
	FTM_UpdatePwmDutycycle(ftm, kFTM_Chnl_0, kFTM_EdgeAlignedPwm, value);
}
