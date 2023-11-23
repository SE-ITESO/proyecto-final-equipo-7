/*
 * INA219.c
 *
 *  Created on: Nov 8, 2023
 *      Author: alber
 */
#include "CONFIG.h"
#include "INA219.h"
#include "fsl_i2c.h"


void INA219_config(void) {
    // Configuración específica del INA219
    // Puedes ajustar estos valores según las necesidades de tu aplicación
    uint16_t config = 0x079u; // Ver hoja de datos del INA219 para los bits de configuración

    // Envía la configuración al INA219
    uint8_t configBytes[2];
    configBytes[0] = (config >> 8) & 0xFF;
    configBytes[1] = config & 0xFF;

    i2c_master_transfer_t i2cTransfer;
    i2cTransfer.flags = kI2C_TransferDefaultFlag;
    i2cTransfer.slaveAddress = INA219_I2C_ADDRESS;
    i2cTransfer.direction = kI2C_Write;
    i2cTransfer.subaddress = 0x00; // Registro de configuración
    i2cTransfer.subaddressSize = 1;
    i2cTransfer.data = configBytes;
    i2cTransfer.dataSize = 2;

    I2C_MasterTransferBlocking(I2C0, &i2cTransfer);
}

// Función para leer la corriente del INA219
float READ_Current(void) {
    i2c_master_transfer_t i2cTransfer;
    uint8_t data[2];

    i2cTransfer.flags = kI2C_TransferDefaultFlag;
    i2cTransfer.slaveAddress = INA219_I2C_ADDRESS;
    i2cTransfer.direction = kI2C_Read;
    i2cTransfer.subaddress = 0x04; // Registro de corriente
    i2cTransfer.subaddressSize = 1;
    i2cTransfer.data = data;
    i2cTransfer.dataSize = 2;

    I2C_MasterTransferBlocking(I2C0, &i2cTransfer);

    int16_t currentRaw = (data[0] << 8) | data[1];
    float current = currentRaw * 0.001; // La corriente se devuelve en mA

    return current;
}

