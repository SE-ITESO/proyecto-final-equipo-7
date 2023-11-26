/*
 * MOTOR.c
 *
 *  Created on: 26 nov. 2023
 *      Author: HP}
 */

#include "MOTOR.h"
#include "CONFIG.h"
#include "GPIO.h"
#include "FTM.h"
#include "fsl_gpio.h"
#include "fsl_port.h"

#define T 0.01

typedef struct{
	uint16_t kp;
	uint16_t ki;
	uint16_t kd;
}PID_Gain_t;

uint8_t g_motor_pid_flag = 0;
uint8_t encoderA;
uint8_t encoderB;
uint8_t direction;
int32_t encoderPosCount = 0;
float vueltas = 0;
float angularPos = 0;
PID_Gain_t PID1;

void MOTOR_set_PID()
{
	PID1.kp =19;
	PID1.ki = 7;
	PID1.kd = 1;
}

void MOTOR_config()
{
    CLOCK_EnableClock(kCLOCK_PortC);
    CLOCK_EnableClock(kCLOCK_PortB);
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
    PORT_SetPinConfig(PORTC, 11, &config);
    GPIO_PinInit(GPIOC, 11, &interrupt_config);
    PORT_SetPinInterruptConfig(PORTC, 11, kPORT_InterruptRisingEdge);
	GPIO_pin_init_io(GPIOB, 11, 0);
	GPIO_pin_alt("B", 11, 1);
	GPIO_pin_clr(GPIOC, 8);
	MOTOR_set_PID();
}

void MOTOR_encoder_channelA()
{
	encoderA = 1;
}

void MOTOR_position()
{
	if(encoderA)
	{
		encoderA = 0;
		if(GPIO_pin_read(GPIOB, 11)==1)
		{
			direction = 1;
			encoderPosCount++;
		}
		else if(GPIO_pin_read(GPIOB, 11)==0)
		{
			direction = 0;
			encoderPosCount--;
		}
		angularPos = (encoderPosCount*RAD_CONVERSION)/ENCODER_TO_MOTOR;
		vueltas = encoderPosCount/ENCODER_TO_MOTOR;
	}
}

static float error;
static float error_;
static float upk;
static float uik;
static float uik_;
static float udk;
static int8_t uk;

void MOTOR_PID_flag()
{
	g_motor_pid_flag = TRUE;
}

void MOTOR_PID(float ref)
{
	if(g_motor_pid_flag)
	{
		g_motor_pid_flag = FALSE;
		error = ref - angularPos;
		upk = PID1.kp*error;
		uik = uik_ + (PID1.ki*T*error_);
		uik_ = uik;
		udk = (PID1.kd/T)*(error-error_);
		error_ = error;
		uk = abs(upk + uik + udk);
		if(uk>80)
		{
			uk=80;
		}else if(uk<55)
		{
			uk=55;
		}

		if((error<0.00001)&&(error>(-0.00001)))
		{
			FTM_set_PWM(FTM0, 0, 0);
			FTM_set_PWM(FTM0, 1, 0);
		}

		else if (error>0)
		{
			FTM_set_PWM(FTM0, 1, uk);
			FTM_set_PWM(FTM0, 0, 0);
		}else if(error<0)
		{
			FTM_set_PWM(FTM0, 0, uk);
			FTM_set_PWM(FTM0, 1, 0);
		}

	}
}

