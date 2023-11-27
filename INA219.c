/*
 * INA219.c
 *
 *  Created on: Nov 8, 2023
 *      Author: alber
 */
#include "CONFIG.h"
#include "INA219.h"
#include "fsl_i2c.h"


i2c_master_transfer_t masterXferINA;

uint8_t g_master_rxBuff_INA[20];

void INA219_config(void) {


/*
    float Rsh =0.1;//resitencia shunt
    float maxCurrent = 0.1; //corriente maxima de 100mA
    float ILSB = maxCurrent / (1 << 15);
    uint32_t calibrationValue = (uint16_t)(0.04096 / (Rsh * ILSB)); //ecuaciones datasheet
*/
    uint32_t calibrationValue=8192;

    uint8_t calibrationBytes[2];
    calibrationBytes[0] = (calibrationValue >> 8) & 0xFF;
    calibrationBytes[1] = calibrationValue & 0xFF;


    masterXferINA.slaveAddress = INA219_I2C_ADDRESS;
    masterXferINA.direction = kI2C_Write;

    masterXferINA.subaddress = 0x05; // registro calibracion
    masterXferINA.subaddressSize = 1;
    masterXferINA.data = calibrationBytes;
    masterXferINA.dataSize = 2;
    masterXferINA.flags = kI2C_TransferDefaultFlag;

    I2C_MasterTransferBlocking(I2C0, &masterXferINA);
}


void INA219_calibration_reg(void) {

  //  uint16_t config = 0x281F;
	 uint32_t config=0|0x0000 |0x0700|0x0078|0x07;
//8047
    uint8_t configBytes[2];
    configBytes[0] = (config >> 8) & 0xFF;
    configBytes[1] = config & 0xFF;

    masterXferINA.slaveAddress = INA219_I2C_ADDRESS;
    masterXferINA.direction = kI2C_Write;
    masterXferINA.subaddress = 0x00; // Configuration register address
    masterXferINA.subaddressSize = 1;
    masterXferINA.data = configBytes;
    masterXferINA.dataSize = 2;
    masterXferINA.flags = kI2C_TransferDefaultFlag;

    I2C_MasterTransferBlocking(I2C0, &masterXferINA);
}


float READ_Current(void) {

    uint8_t data[2];


    masterXferINA.slaveAddress = INA219_I2C_ADDRESS;
    masterXferINA.direction = kI2C_Read;
    masterXferINA.subaddress = 0x04; // Registro de corriente
    masterXferINA.subaddressSize = 1;
    masterXferINA.data = data;
    masterXferINA.dataSize = 2;
    masterXferINA.flags = kI2C_TransferDefaultFlag;

    I2C_MasterTransferBlocking(I2C0, &masterXferINA);

    int16_t currentRaw = (data[0] >> 8) | data[1];
    float current = currentRaw * 0.001; // para devolver corriente en mA

  //  return current;
//
}

float READ_shunt(void) {

    uint8_t data[2];


    masterXferINA.slaveAddress = INA219_I2C_ADDRESS;
    masterXferINA.direction = kI2C_Read;
    masterXferINA.subaddress = 0x01; // registro shunt
    masterXferINA.subaddressSize = 1;
    masterXferINA.data = data;
    masterXferINA.dataSize = 2;
    masterXferINA.flags = kI2C_TransferDefaultFlag;

    I2C_MasterTransferBlocking(I2C0, &masterXferINA);

    int16_t vRaw = (data[0] << 8) | data[1];
    float voltage = vRaw;

    return voltage;

}
