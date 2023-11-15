/*
 * @file 	MEM.c
 *
 * @Authors Diego Delgado
 * 			Alberto Quintana
 *
 * @brief 	This file initializes spi memory and allows to extract sounds from given address with their respective size
 *
 *
 */

#include "fsl_gpio.h"
#include "fsl_port.h"
#include "MEM.h"
#include "SPI.h"
#include "GPIO.h"
#include "CONFIG.h"
#include "fsl_dspi.h"

uint8_t g_image[504];

void MEM_init(void){

	port_pin_config_t mem_config = {
	            kPORT_PullDisable,
	            kPORT_FastSlewRate,
	            kPORT_PassiveFilterDisable,
	            kPORT_OpenDrainDisable,
	            kPORT_HighDriveStrength,
	            kPORT_MuxAlt2,
	            kPORT_UnlockRegister
	};

	PORT_SetPinConfig(PORTC, CE_MEM, &mem_config);
}

void MEM_transfer(uint8_t *txData, uint8_t *rxData, size_t txDataSize, size_t rxDataSize)
{
    dspi_half_duplex_transfer_t masterXfer = {0};
    masterXfer.txData      = txData;
    masterXfer.rxData      = rxData;
    masterXfer.txDataSize  = txDataSize;
    masterXfer.rxDataSize  = rxDataSize;
    masterXfer.isTransmitFirst = TRUE;
    masterXfer.isPcsAssertInTransfer = TRUE;
    masterXfer.configFlags = kDSPI_MasterCtar1 | kDSPI_MasterPcs1 | kDSPI_MasterPcsContinuous;
    DSPI_MasterHalfDuplexTransferBlocking(SPI0, &masterXfer);
}

uint8_t MEM_read_byte(uint32_t address)
{
	uint8_t rxByte[]={0};
    // Send READ command followed by address
    uint8_t buffer[tx_BUFFER_SIZE];
    buffer[0] = READ_DATA;
    buffer[1] = (address & 0xFF0000) >> 16;
    buffer[2] = (address & 0x00FF00) >> 8;
    buffer[3] = (address & 0x0000FF);

    MEM_transfer(buffer, rxByte, tx_BUFFER_SIZE, rx_BUFFER_SIZE);
    return rxByte[0];
}
