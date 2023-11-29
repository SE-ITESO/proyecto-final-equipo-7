/*
 * CORE_config.c
 *
 *  Created on: 3 nov. 2023
 *      Author: HP}
 */

#include "CORE_config.h"
#include "MK64F12.h"
#include "fsl_clock.h"

void CORE_config_set()
{
    mcg_pll_config_t pll0Config =
    {
        .enableMode = MCG_PLL_DISABLE, /* MCGPLLCLK disabled */
        .prdiv      = 0x0U,             /* PLL Reference divider: divided by 1 */
        .vdiv       = 0x01,             /* VCO divider: multiplied by 2 */
    };

    const osc_config_t osc_config = {
        .freq        = 50000000U,    /* Oscillator frequency: 50000000Hz */
        .capLoad     = (OSC_CAP0P),   /* Oscillator capacity load: 0pF */
        .workMode    = kOSC_ModeExt,  /* Use external clock */
        .oscerConfig = {
            .enableMode =
                kOSC_ErClkEnable, /* Enable external reference clock, disable external reference clock in STOP mode */
        }};

    CLOCK_SetSimSafeDivs();

    CLOCK_InitOsc0(&osc_config);

    CLOCK_SetXtal0Freq(osc_config.freq);

    // Cambiar a modo FEI (FLL Engaged Internal) para ajustar la frecuencia a 48 MHz
    CLOCK_SetFeiMode(kMCG_Dmx32Default, kMCG_DrsLow, NULL);

    // Configurar el divisor de la frecuencia interna (OUTDIV1) para ajustar la frecuencia a 21 MHz
    SIM->CLKDIV1 = SIM_CLKDIV1_OUTDIV1(2); // Divisor de 3 (48 MHz / 3 = 16 MHz)

    SystemCoreClock = CLOCK_GetFreq(kCLOCK_CoreSysClk);
}
