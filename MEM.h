/*
 * @file 	MEM.h
 *
 * @Authors Diego Delgado
 * 			Alberto Quintana
 *
 * @brief 	This file initializes spi memory and allows to extract sounds from given address with their respective size
 *
 *
 */

#ifndef MEM_H_
#define MEM_H_

#define EXAMPLE_DSPI_MASTER_BASEADDR         SPI0


void MEM_init(void);
void MEM_transfer(uint8_t *txData, uint8_t *rxData, size_t txDataSize, size_t rxDataSize);
uint8_t MEM_read_byte(uint32_t address);

#endif /* MEM_H_ */
