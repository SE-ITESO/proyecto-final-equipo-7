
#ifndef I2C_H_
#define I2C_H_


#include "stdint.h"
#include "CONFIG.h"

void I2C_init();
void I2C_write(I2C_Device_t device);
void I2C_read(I2C_Device_t device);
uint8_t I2C_get_transfer_success_flag();


#endif /* I2C_H_ */
