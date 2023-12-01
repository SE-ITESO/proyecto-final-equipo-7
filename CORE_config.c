/*
 * @file 	CORE_config.c
 *
 * @Authors Diego Delgado
 * 			Alberto Quintana
 *
 * @brief   This file sets the desired clock rate.
 *
 *
 */

#include "CORE_config.h"
#include "MK64F12.h"
#include "fsl_clock.h"

/*

 * Configures the core settings of the microcontroller.
 * Sets the PLL and oscillator configurations to achieve a specified system clock frequency.
 */
void CORE_config_set()
{
    // PLL configuration structure
    mcg_pll_config_t pll0Config =
    {
        .enableMode = MCG_PLL_DISABLE, /* MCGPLLCLK disabled */
        .prdiv      = 0x0U,             /* PLL Reference divider: divided by 1 */
        .vdiv       = 0x01,             /* VCO divider: multiplied by 2 */
    };

    // Oscillator configuration structure
    const osc_config_t osc_config = {
        .freq        = 50000000U,    /* Oscillator frequency: 50000000Hz */
        .capLoad     = (OSC_CAP0P),   /* Oscillator capacity load: 0pF */
        .workMode    = kOSC_ModeExt,  /* Use external clock */
        .oscerConfig = {
            .enableMode =
                kOSC_ErClkEnable, /* Enable external reference clock, disable external reference clock in STOP mode */
        }};

    // Set safe divisor values for SIM clock
    CLOCK_SetSimSafeDivs();

    // Initialize Oscillator 0 with specified configuration
    CLOCK_InitOsc0(&osc_config);

    // Set the frequency of external crystal Oscillator 0
    CLOCK_SetXtal0Freq(osc_config.freq);

    // Switch to FEI mode (FLL Engaged Internal) to adjust the frequency to 48 MHz
    CLOCK_SetFeiMode(kMCG_Dmx32Default, kMCG_DrsLow, NULL);

    // Configure the internal frequency divisor (OUTDIV1) to adjust the frequency to 21 MHz
    SIM->CLKDIV1 = SIM_CLKDIV1_OUTDIV1(2); // Divisor of 3 (48 MHz / 3 = 16 MHz)

    // Update the System Core Clock variable
    SystemCoreClock = CLOCK_GetFreq(kCLOCK_CoreSysClk);
}
