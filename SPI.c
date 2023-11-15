
#include "SPI.h"
#include "CONFIG.h"
#include "fsl_dspi.h"
#include "fsl_port.h"
#include "fsl_gpio.h"
#include "fsl_clock.h"
#include "clock_config.h"

void SPI_config(void)
{
	CLOCK_SetSimSafeDivs();
	BOARD_InitBootClocks();


	CLOCK_EnableClock(kCLOCK_Spi0);
	CLOCK_EnableClock(kCLOCK_PortD);
	CLOCK_EnableClock(kCLOCK_PortC);

	PORT_SetPinMux(PORTD, PIN1, kPORT_MuxAlt2);//CLK//SPI
	PORT_SetPinMux(PORTD, PIN2, kPORT_MuxAlt2);//DIN//SPI

	uint32_t srcClock_Hz;
	dspi_master_config_t masterConfig;

	masterConfig.whichCtar                                = kDSPI_Ctar0;
	masterConfig.ctarConfig.baudRate                      = TRANSFER_BAUDRATE;
	masterConfig.ctarConfig.bitsPerFrame                  = 8U;
	masterConfig.ctarConfig.cpol                          = kDSPI_ClockPolarityActiveHigh;
	masterConfig.ctarConfig.cpha                          = kDSPI_ClockPhaseFirstEdge;
	masterConfig.ctarConfig.direction                     = kDSPI_MsbFirst;
	masterConfig.ctarConfig.pcsToSckDelayInNanoSec        = 1000000000U / TRANSFER_BAUDRATE;
	masterConfig.ctarConfig.lastSckToPcsDelayInNanoSec    = 1000000000U / TRANSFER_BAUDRATE;
	masterConfig.ctarConfig.betweenTransferDelayInNanoSec = 1000000000U / TRANSFER_BAUDRATE;

	masterConfig.whichPcs           = kDSPI_Pcs0;
	masterConfig.pcsActiveHighOrLow = kDSPI_PcsActiveLow;

	masterConfig.enableContinuousSCK        = false;
	masterConfig.enableRxFifoOverWrite      = false;
	masterConfig.enableModifiedTimingFormat = false;
	masterConfig.samplePoint                = kDSPI_SckToSin0Clock;



	srcClock_Hz = DSPI_MASTER_CLK_FREQ;
	DSPI_MasterInit(SPI0, &masterConfig, srcClock_Hz);

	port_pin_config_t mem_config = {
				kPORT_PullDisable,
				kPORT_FastSlewRate,
				kPORT_PassiveFilterDisable,
				kPORT_OpenDrainDisable,
				kPORT_HighDriveStrength,
				kPORT_MuxAlt2,
				kPORT_UnlockRegister
	};

	PORT_SetPinConfig(PORTD, PIN1, &mem_config);
	PORT_SetPinConfig(PORTD, PIN2, &mem_config);
	PORT_SetPinConfig(PORTD, PIN3, &mem_config);

	masterConfig.whichCtar                                  = kDSPI_Ctar1;
	masterConfig.ctarConfig.baudRate                      = TRANSFER_BAUDRATE_MEM;
	masterConfig.ctarConfig.bitsPerFrame                  = 8U;
	masterConfig.ctarConfig.cpol                          = kDSPI_ClockPolarityActiveLow;
	masterConfig.ctarConfig.cpha                          = kDSPI_ClockPhaseSecondEdge;
	masterConfig.ctarConfig.direction                     = kDSPI_MsbFirst;
	masterConfig.ctarConfig.pcsToSckDelayInNanoSec        = 1000000000U / TRANSFER_BAUDRATE_MEM;
	masterConfig.ctarConfig.lastSckToPcsDelayInNanoSec    = 1000000000U / TRANSFER_BAUDRATE_MEM;
	masterConfig.ctarConfig.betweenTransferDelayInNanoSec = 1000000000U / TRANSFER_BAUDRATE_MEM;

	masterConfig.whichPcs           = kDSPI_Pcs1;
	masterConfig.pcsActiveHighOrLow = kDSPI_PcsActiveLow;

	masterConfig.enableContinuousSCK        = false;
	masterConfig.enableRxFifoOverWrite      = false;
	masterConfig.enableModifiedTimingFormat = false;
	masterConfig.samplePoint                = kDSPI_SckToSin0Clock;

	srcClock_Hz = DSPI_MASTER_CLK_FREQ;
	DSPI_MasterInit(SPI0, &masterConfig, srcClock_Hz);

}




