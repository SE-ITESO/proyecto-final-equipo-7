/*
 * FTM.c
 *
 *  Created on: 24 nov. 2023
 *      Author: HP}
 */

#include "FTM.h"
#include "fsl_ftm.h"

ftm_config_t ftmConfig;

void FTM_init()
{


    FTM_GetDefaultConfig(&ftmConfig);
    FTM_Init(FTM0, &ftmConfig);
}
