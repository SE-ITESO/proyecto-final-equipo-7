







#include "I2C.h"
#include "CONFIG.h"
#include "fsl_i2c.h"
#include "fsl_port.h"
#include "fsl_gpio.h"
#include "board.h"
i2c_master_config_t masterConfig;
uint32_t sourceClock;
i2c_master_transfer_t masterXfer;

uint8_t g_master_txBuff[I2C_DATA_LENGTH];
uint8_t g_master_txBuff_wave[I2C_DATA_LENGTH];
uint8_t g_master_rxBuff[I2C_DATA_LENGTH];

uint8_t g_i2c_rx_time[3] = {0,0,0};
uint8_t g_i2c_rx_date[3] = {0,0,0};
uint8_t g_i2c_rx_hum[3] = {0,0,0};
uint8_t g_i2c_rx_temp[3] = {0,0,0};


uint8_t g_i2c_data_log_1[EEPROM_FORMAT_LENGHT] = {0,0,0,0,0,0,0,0,0,0,0,0};
uint8_t g_i2c_data_log_2[EEPROM_FORMAT_LENGHT] = {0,0,0,0,0,0,0,0,0,0,0,0};
uint8_t g_i2c_data_log_3[EEPROM_FORMAT_LENGHT] = {0,0,0,0,0,0,0,0,0,0,0,0};
uint8_t g_i2c_data_log_4[EEPROM_FORMAT_LENGHT] = {0,0,0,0,0,0,0,0,0,0,0,0};
uint8_t g_i2c_data_log_5[EEPROM_FORMAT_LENGHT] = {0,0,0,0,0,0,0,0,0,0,0,0};

uint8_t g_transfer_success_flag;

/*

void I2C_init(void) {


		    uint32_t sourceClock;
		    i2c_master_transfer_t masterXfer;

		    CLOCK_EnableClock(kCLOCK_PortB);
		    PORT_SetPinMux(PORTB, 2u, kPORT_MuxAlt2);
		    PORT_SetPinMux(PORTB, 3u, kPORT_MuxAlt2);

    i2c_master_config_t masterConfig;
    I2C_MasterGetDefaultConfig(&masterConfig);
    I2C_MasterInit(I2C0, &masterConfig, CLOCK_GetFreq(I2C0_CLK_SRC));
}
*/



void I2C_init(){

	    i2c_master_config_t masterConfig;
	    uint32_t sourceClock;
	    i2c_master_transfer_t masterXfer;

	    CLOCK_EnableClock(kCLOCK_PortB);
	    PORT_SetPinMux(PORTB, PIN2, kPORT_MuxAlt2);
	    PORT_SetPinMux(PORTB, PIN3, kPORT_MuxAlt2);

	    //Para conectar square wave
	    CLOCK_EnableClock(kCLOCK_PortE);
	    port_pin_config_t config = {
	        kPORT_PullUp,
	        kPORT_SlowSlewRate,
	        kPORT_PassiveFilterDisable,
	        kPORT_OpenDrainDisable,
	        kPORT_HighDriveStrength,
	        kPORT_MuxAsGpio,
	        kPORT_UnlockRegister
	    };
	    gpio_pin_config_t interrupt_config = {
	        kGPIO_DigitalInput,
	        0,
	    };

	    PORT_SetPinConfig(PORTE, 24, &config);
	    GPIO_PinInit(GPIOE, 24, &interrupt_config);
	    PORT_SetPinInterruptConfig(PORTE, 24, kPORT_InterruptRisingEdge);

	    BOARD_InitBootClocks();
	    BOARD_InitDebugConsole();

	    I2C_MasterGetDefaultConfig(&masterConfig);
	    masterConfig.baudRate_Bps = I2C_BAUDRATE;

	    sourceClock = I2C_MASTER_CLK_FREQ;

	    I2C_MasterInit(I2C0, &masterConfig, sourceClock);

	    uint8_t currentHours;
	    currentHours |= HOUR_FORMAT_MASK;
	    masterXfer.slaveAddress = I2C_RTC_ADDRESS;
	    masterXfer.direction = kI2C_Write;
	    masterXfer.subaddress = HOUR_SUBADDRESS;
	    masterXfer.subaddressSize = 1;
	    masterXfer.data = &currentHours;
	    masterXfer.dataSize = 1;
	    masterXfer.flags = kI2C_TransferDefaultFlag;

	    // Realiza la transferencia
	    I2C_MasterTransferBlocking(I2C0, &masterXfer);


}


void I2C_rtc_squarewave(){
	g_master_txBuff_wave[0] = SQUAREWAVE_FREQ_MASK;
	masterXfer.slaveAddress   = I2C_RTC_ADDRESS;
	masterXfer.direction      = kI2C_Write;
	masterXfer.subaddress     = SQUAREWAVE_SUBADDRESS;
	masterXfer.subaddressSize = 1;
	masterXfer.data           = g_master_txBuff_wave;
	masterXfer.dataSize       = 1;
	masterXfer.flags          = kI2C_TransferDefaultFlag;
	I2C_MasterTransferBlocking(I2C0, &masterXfer);

	if (I2C_MasterTransferBlocking(I2C0, &masterXfer) == kStatus_Success) {
		g_transfer_success_flag=true;
	} else {
		g_transfer_success_flag=false;
	}
}







//////////////////////////extra revisar

void I2C_write_rtc_time(){

	for(uint8_t i=3;i!=0;i--){

		g_master_txBuff[0] = g_i2c_rx_time[i-1];
		masterXfer.slaveAddress   = I2C_RTC_ADDRESS;
		masterXfer.direction      = kI2C_Write;
		masterXfer.subaddress     = i-1;
		masterXfer.subaddressSize = 1;
		masterXfer.data           = g_master_txBuff;
		masterXfer.dataSize       = 1;
		masterXfer.flags          = kI2C_TransferDefaultFlag;

		I2C_MasterTransferBlocking(I2C0, &masterXfer);
	}
	if (I2C_MasterTransferBlocking(I2C0, &masterXfer) == kStatus_Success) {
		g_transfer_success_flag=true;
	} else {
		g_transfer_success_flag=false;
	}
}

void I2C_read_rtc_time(){

	for(uint8_t i=3;i!=0;i--){

		masterXfer.slaveAddress   = I2C_RTC_ADDRESS;
		masterXfer.direction      = kI2C_Read;
		masterXfer.subaddress     = i-1;
		masterXfer.subaddressSize = 1;
		masterXfer.data           = g_master_rxBuff;
		masterXfer.dataSize       = 1;
		masterXfer.flags          = kI2C_TransferDefaultFlag;

		I2C_MasterTransferBlocking(I2C0, &masterXfer);
		g_i2c_rx_time[i-1] = g_master_rxBuff[0];
	}
	if (I2C_MasterTransferBlocking(I2C0, &masterXfer) == kStatus_Success) {
		g_transfer_success_flag=true;
	} else {
		g_transfer_success_flag=false;
	}
}
void I2C_write_rtc_date(){

	for(uint8_t i=7;i!=4;i--){ //dia 06, mes 05, año 04

		g_master_txBuff[0] = g_i2c_rx_date[i-5];
		masterXfer.slaveAddress   = I2C_RTC_ADDRESS;
		masterXfer.direction      = kI2C_Write;
		masterXfer.subaddress     = i-1;
		masterXfer.subaddressSize = 1;
		masterXfer.data           = g_master_txBuff;
		masterXfer.dataSize       = 1;
		masterXfer.flags          = kI2C_TransferDefaultFlag;

		I2C_MasterTransferBlocking(I2C0, &masterXfer);
	}
	if (I2C_MasterTransferBlocking(I2C0, &masterXfer) == kStatus_Success) {
		g_transfer_success_flag=true;
	} else {
		g_transfer_success_flag=false;
	}
}

void I2C_read_rtc_date(){

	for(uint8_t i=7;i!=4;i--){

		masterXfer.slaveAddress   = I2C_RTC_ADDRESS;
		masterXfer.direction      = kI2C_Read;
		masterXfer.subaddress     = i-1;
		masterXfer.subaddressSize = 1;
		masterXfer.data           = g_master_rxBuff;
		masterXfer.dataSize       = 1;
		masterXfer.flags          = kI2C_TransferDefaultFlag;

		I2C_MasterTransferBlocking(I2C0, &masterXfer);
		g_i2c_rx_time[i-1] = g_master_rxBuff[0];
	}
	if (I2C_MasterTransferBlocking(I2C0, &masterXfer) == kStatus_Success) {
		g_transfer_success_flag=true;
	} else {
		g_transfer_success_flag=false;
	}
}

uint8_t* I2C_get_rx_time() {
    return g_i2c_rx_time;
}

uint8_t* I2C_get_rx_date() {
    return g_i2c_rx_date;
}

void I2C_init_dht()
{

	uint8_t calibration_flag = 0;

	g_master_txBuff[0]		  = DHT_RESET;
	masterXfer.slaveAddress   = I2C_DHT_ADDRESS;
	masterXfer.direction      = kI2C_Write;
	masterXfer.subaddress     = 0;
	masterXfer.subaddressSize = 0;
	masterXfer.data           = g_master_txBuff;
	masterXfer.dataSize       = 1;
	masterXfer.flags          = kI2C_TransferDefaultFlag;

	I2C_MasterTransferBlocking(I2C0, &masterXfer);

	//delay

	g_master_txBuff[0]	= DHT_COMMAND_1;
	g_master_txBuff[1]	= DHT_COMMAND_2;
	g_master_txBuff[2]	= DHT_COMMAND_3;
	masterXfer.slaveAddress   = I2C_DHT_ADDRESS;
	masterXfer.direction      = kI2C_Write;
	masterXfer.subaddress     = 0;
	masterXfer.subaddressSize = 0;
	masterXfer.data           = g_master_txBuff;
	masterXfer.dataSize       = 3;
	masterXfer.flags          = kI2C_TransferDefaultFlag;

	I2C_MasterTransferBlocking(I2C0, &masterXfer);

	while(calibration_flag==0)
	{
		masterXfer.slaveAddress   = I2C_DHT_ADDRESS;
		masterXfer.direction      = kI2C_Read;
		masterXfer.subaddress     = DHT_STATUS;
		masterXfer.subaddressSize = 1;
		masterXfer.data           = g_master_rxBuff;
		masterXfer.dataSize       = 1;
		masterXfer.flags          = kI2C_TransferDefaultFlag;

		I2C_MasterTransferBlocking(I2C0, &masterXfer);
		calibration_flag = g_master_rxBuff[0] & LSBYTE_MASK;
	}
}




uint8_t I2C_get_transfer_success_flag() {
    return g_transfer_success_flag;
}

void I2C_write_data_log(uint8_t log, uint8_t cap){

	uint16_t address;
	uint8_t logData[EEPROM_FORMAT_LENGHT];

    switch (log) {
        case LOG_1:
        	address = LOG_1_ADDRESS + EEPROM_FORMAT_LENGHT*cap;
        	break;
        case LOG_2:
        	address = LOG_2_ADDRESS + EEPROM_FORMAT_LENGHT*cap;
        	break;
        case LOG_3:
        	address = LOG_3_ADDRESS + EEPROM_FORMAT_LENGHT*cap;
        	break;
        case LOG_4:
        	address = LOG_4_ADDRESS + EEPROM_FORMAT_LENGHT*cap;
        	break;
        case LOG_5:
        	address = LOG_5_ADDRESS + EEPROM_FORMAT_LENGHT*cap;
        	break;
        default:
        	break;
    }

	logData[0] = g_i2c_rx_hum[0];
	logData[1] = g_i2c_rx_hum[1];
	logData[2] = g_i2c_rx_hum[2];
	logData[3] = g_i2c_rx_temp[0];
	logData[4] = g_i2c_rx_temp[1];
	logData[5] = g_i2c_rx_temp[2];
	logData[6] = g_i2c_rx_time[I2C_SECONDS_INDEX];
	logData[7] = g_i2c_rx_time[I2C_MINUTES_INDEX];
	logData[8] = g_i2c_rx_time[I2C_HOURS_INDEX];
	logData[9] = g_i2c_rx_date[I2C_DAY_INDEX];
	logData[10] = g_i2c_rx_date[I2C_MONTH_INDEX];
	logData[11] = g_i2c_rx_date[I2C_YEAR_INDEX];

	masterXfer.slaveAddress = I2C_EEPROM_ADDRESS;
	masterXfer.direction = kI2C_Write;
	masterXfer.subaddress = address;
	masterXfer.subaddressSize = 2;
	masterXfer.data = logData;
	masterXfer.dataSize = EEPROM_FORMAT_LENGHT;
	masterXfer.flags = kI2C_TransferDefaultFlag;
	I2C_MasterTransferBlocking(I2C0, &masterXfer);

	if (I2C_MasterTransferBlocking(I2C0, &masterXfer) == kStatus_Success) {
		g_transfer_success_flag=true;
	} else {
		g_transfer_success_flag=false;
	}
}

void I2C_read_log(uint8_t log, uint8_t cap)
{
	uint16_t address;
	uint8_t logData[EEPROM_FORMAT_LENGHT];

    switch (log) {
        case LOG_1:
        	address = LOG_1_ADDRESS + EEPROM_FORMAT_LENGHT*cap;
        	break;
        case LOG_2:
        	address = LOG_2_ADDRESS + EEPROM_FORMAT_LENGHT*cap;
        	break;
        case LOG_3:
        	address = LOG_3_ADDRESS + EEPROM_FORMAT_LENGHT*cap;
        	break;
        case LOG_4:
        	address = LOG_4_ADDRESS + EEPROM_FORMAT_LENGHT*cap;
        	break;
        case LOG_5:
        	address = LOG_5_ADDRESS + EEPROM_FORMAT_LENGHT*cap;
        	break;
        default:
        	break;
    }

    masterXfer.slaveAddress = I2C_EEPROM_ADDRESS;
    masterXfer.direction = kI2C_Read;
    masterXfer.subaddress = address;
    masterXfer.subaddressSize = 2;
    masterXfer.data = logData;
    masterXfer.dataSize = EEPROM_FORMAT_LENGHT;
    masterXfer.flags = kI2C_TransferDefaultFlag;

    I2C_MasterTransferBlocking(I2C0, &masterXfer);

	if (I2C_MasterTransferBlocking(I2C0, &masterXfer) == kStatus_Success) {
		g_transfer_success_flag=true;
	} else {
		g_transfer_success_flag=false;;
	}

    switch (log) {
        case LOG_1:
        	for (uint8_t i = 0; i < EEPROM_FORMAT_LENGHT; i++) {
        	    g_i2c_data_log_1[i] = logData[i];
        	}
        	break;
        case LOG_2:
        	for (uint8_t i = 0; i < EEPROM_FORMAT_LENGHT; i++) {
        	    g_i2c_data_log_2[i] = logData[i];
        	}
        	break;
        case LOG_3:
        	for (uint8_t i = 0; i < EEPROM_FORMAT_LENGHT; i++) {
        	    g_i2c_data_log_3[i] = logData[i];
        	}
        	break;
        case LOG_4:
        	for (uint8_t i = 0; i < EEPROM_FORMAT_LENGHT; i++) {
        	    g_i2c_data_log_4[i] = logData[i];
        	}
        	break;
        case LOG_5:
        	for (uint8_t i = 0; i < EEPROM_FORMAT_LENGHT; i++) {
        	    g_i2c_data_log_5[i] = logData[i];
        	}
        	break;
        default:
        	break;
    }
}

uint8_t* I2C_get_data_log(uint8_t log)
{
    switch (log) {
        case LOG_1:
            return g_i2c_data_log_1;
        case LOG_2:
            return g_i2c_data_log_2;
        case LOG_3:
            return g_i2c_data_log_3;
        case LOG_4:
            return g_i2c_data_log_4;
        case LOG_5:
            return g_i2c_data_log_5;
        default:
            return NULL;
    }
}

void I2C_write_caps_log(uint8_t log, uint8_t caps)
{
	uint16_t address;
    switch (log) {
        case LOG_1:
        	address = LOG_1_ADD_CAP;
        	break;
        case LOG_2:
        	address = LOG_2_ADD_CAP;
        	break;
        case LOG_3:
        	address = LOG_3_ADD_CAP;
        	break;
        case LOG_4:
        	address = LOG_4_ADD_CAP;
        	break;
        case LOG_5:
        	address = LOG_5_ADD_CAP;
        	break;
        default:
            break;
    }
    g_master_txBuff[0] = caps;
	masterXfer.slaveAddress   = I2C_EEPROM_ADDRESS;
	masterXfer.direction      = kI2C_Write;
	masterXfer.subaddress     = address;
	masterXfer.subaddressSize = 2;
	masterXfer.data           = g_master_txBuff;
	masterXfer.dataSize       = 1;
	masterXfer.flags          = kI2C_TransferDefaultFlag;

    I2C_MasterTransferBlocking(I2C0, &masterXfer);

	if (I2C_MasterTransferBlocking(I2C0, &masterXfer) == kStatus_Success) {
		g_transfer_success_flag=true;
	} else {
		g_transfer_success_flag=false;
	}

}

uint8_t I2C_get_caps_log(uint8_t log)
{
	uint8_t savedCaps;
	uint16_t address;
    switch (log) {
        case LOG_1:
        	address = LOG_1_ADD_CAP;
        	break;
        case LOG_2:
        	address = LOG_2_ADD_CAP;
        	break;
        case LOG_3:
        	address = LOG_3_ADD_CAP;
        	break;
        case LOG_4:
        	address = LOG_4_ADD_CAP;
        	break;
        case LOG_5:
        	address = LOG_5_ADD_CAP;
        	break;
        default:
            break;
    }
    masterXfer.slaveAddress = I2C_EEPROM_ADDRESS;
    masterXfer.direction = kI2C_Read;
    masterXfer.subaddress = address;
    masterXfer.subaddressSize = 2;
    masterXfer.data = g_master_rxBuff;
    masterXfer.dataSize = 1;
    masterXfer.flags = kI2C_TransferDefaultFlag;

    I2C_MasterTransferBlocking(I2C0, &masterXfer);
    savedCaps = g_master_rxBuff[0];
    return savedCaps;

	if (I2C_MasterTransferBlocking(I2C0, &masterXfer) == kStatus_Success) {
		g_transfer_success_flag=true;
	} else {
		g_transfer_success_flag=false;
	}

}




/*




void I2C_init(){

	    i2c_master_config_t masterConfig;
	    uint32_t sourceClock;
	    i2c_master_transfer_t masterXfer;

	    CLOCK_EnableClock(kCLOCK_PortB);
	    PORT_SetPinMux(PORTB, PIN2, kPORT_MuxAlt2);
	    PORT_SetPinMux(PORTB, PIN3, kPORT_MuxAlt2);

	    //Para conectar square wave
	    CLOCK_EnableClock(kCLOCK_PortD);
	    port_pin_config_t config = {
	        kPORT_PullUp,
	        kPORT_SlowSlewRate,
	        kPORT_PassiveFilterDisable,
	        kPORT_OpenDrainDisable,
	        kPORT_HighDriveStrength,
	        kPORT_MuxAsGpio,
	        kPORT_UnlockRegister
	    };
	    gpio_pin_config_t interrupt_config = {
	        kGPIO_DigitalInput,
	        0,
	    };

	    PORT_SetPinConfig(PORTD, PIN0, &config);
	    GPIO_PinInit(GPIOD, PIN0, &interrupt_config);
	    PORT_SetPinInterruptConfig(PORTD, PIN0, kPORT_InterruptRisingEdge);

	    BOARD_InitBootClocks();
	    BOARD_InitDebugConsole();

	    I2C_MasterGetDefaultConfig(&masterConfig);
	    masterConfig.baudRate_Bps = I2C_BAUDRATE;

	    sourceClock = I2C_MASTER_CLK_FREQ;

	    I2C_MasterInit(I2C0, &masterConfig, sourceClock);

	    uint8_t currentHours;
	    currentHours |= HOUR_FORMAT_MASK;
	    masterXfer.slaveAddress = I2C_RTC_ADDRESS;
	    masterXfer.direction = kI2C_Write;
	    masterXfer.subaddress = HOUR_SUBADDRESS;
	    masterXfer.subaddressSize = 1;
	    masterXfer.data = &currentHours;
	    masterXfer.dataSize = 1;
	    masterXfer.flags = kI2C_TransferDefaultFlag;

	    // Realiza la transferencia
	    I2C_MasterTransferBlocking(I2C0, &masterXfer);


}

*/
