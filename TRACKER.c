/*
 * TRACKER.c
 *
 *  Created on: 29 nov. 2023
 *      Author: HP}
 */

#include "TRACKER.h"
#include "CONFIG.h"
#include "LDR.h"
#include "FTM.h"

#define TOL 50

uint16_t ldr_lu;
uint16_t ldr_ld;
uint16_t ldr_ru;
uint16_t ldr_rd;

uint16_t ldr_up;
uint16_t ldr_down;
uint16_t ldr_left;
uint16_t ldr_right;

uint16_t ldr_delta_ver;
uint16_t ldr_delta_hor;

uint8_t motor_ver;
uint8_t motor_hor;

uint8_t tracker_flag = 0;

uint8_t TRACKER_check_tol(uint16_t delta)
{
	if(((-TOL)<delta) || (delta>TOL))
	{
		return 1;
	}
	else
	{
		return 0;
	}
}

void TRACKER_flag_on()
{
	tracker_flag = 1;
}

void TRACKER_handler()
{

	if(1==tracker_flag)
	{
		tracker_flag = 0;
		ldr_lu = LDR_read(LDR_LU);
		ldr_ld = LDR_read(LDR_LD);
		ldr_ru = LDR_read(LDR_RU);
		ldr_rd = LDR_read(LDR_RD);

		ldr_up = ldr_lu + ldr_ru;
		ldr_down = ldr_ld + ldr_rd;
		ldr_left = ldr_ld + ldr_lu;
		ldr_right = ldr_ru + ldr_rd;

		ldr_delta_ver = ldr_up - ldr_down;
		ldr_delta_hor = ldr_left - ldr_right;

		if(TRACKER_check_tol(ldr_delta_ver))
		{
			if(ldr_up > ldr_down)
			{
				motor_ver += 2;
				if(motor_ver > 255)
				{
					motor_ver = 255;
				}
			}
			else if(ldr_down > ldr_up)
			{
				motor_ver -= 2;
				if(motor_ver < 100)
				{
					motor_ver = 100;
				}
			}
			FTM_set_PWM(FTM0, 0, motor_ver);
		}

		if(TRACKER_check_tol(ldr_delta_hor))
		{
			if(ldr_left > ldr_right)
			{
				motor_hor += 2;
				if(motor_hor > 255)
				{
					motor_hor = 255;
				}
			}
			else if(ldr_right > ldr_left)
			{
				motor_hor -= 2;
				if(motor_hor < 100)
				{
					motor_hor = 100;
				}
			}
			FTM_set_PWM(FTM0, 0, motor_hor);
		}
	}

}
