







#include "I2C.h"
#include "CONFIG.h"
#include "fsl_i2c.h"
#include "fsl_port.h"
#include "fsl_gpio.h"

i2c_master_config_t masterConfig;
uint32_t sourceClock;
i2c_master_transfer_t masterXfer;

uint8_t g_master_txBuff[I2C_DATA_LENGTH];
uint8_t g_master_txBuff_wave[I2C_DATA_LENGTH];
uint8_t g_master_rxBuff[I2C_DATA_LENGTH];





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
