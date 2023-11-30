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

#define TOL 20

uint16_t ldr_lu;
uint16_t ldr_ld;
uint16_t ldr_ru;
uint16_t ldr_rd;

uint16_t ldr_up;
uint16_t ldr_down;
uint16_t ldr_left;
uint16_t ldr_right;

int16_t ldr_delta_ver;
int16_t ldr_delta_hor;

uint8_t motor_ver = 130;
uint8_t motor_hor = 162;

uint8_t tracker_flag = 0;

uint8_t TRACKER_check_tol(int16_t delta)
{
	if(((-1*TOL)>delta) || (delta>TOL) )
		//if( (delta>TOL) )
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
		ldr_lu = LDR_read(LDR_LU);	//4	4023
		ldr_ld = LDR_read(LDR_LD);	//4	3995
		ldr_ru = LDR_read(LDR_RU);	//2789	3871
		ldr_rd = LDR_read(LDR_RD);	//2789	4005

		ldr_lu = ((ldr_lu-4)*100)/4019;
		ldr_ld = ((ldr_ld-4)*100)/4019;
		ldr_ru = (((ldr_ru-2789)*100)/1082);
		ldr_rd = ((ldr_rd-2789)*100)/1216;

		ldr_up = ldr_lu + ldr_ru;
		ldr_down = ldr_ld + ldr_rd;
		ldr_left = ldr_ld + ldr_lu;
		ldr_right = ldr_ru + ldr_rd;

		ldr_delta_ver = ldr_up - ldr_down;
		ldr_delta_hor = ldr_left - ldr_right;

		if(TRACKER_check_tol(ldr_delta_ver))
		{
			if(ldr_down > ldr_up)
			{
			motor_ver -= 1;
			}
			else if(ldr_up > ldr_down)
			{
				motor_ver += 1;
			}



			if(motor_ver > 214)
			{
				motor_ver = 214;
			}
			if(motor_ver < 110)
			{
				motor_ver = 110;
			}
			FTM_set_PWM(FTM0, 1, motor_ver);
		}

		if(TRACKER_check_tol(ldr_delta_hor))
		{
			if(ldr_left > ldr_right)
			{
				motor_hor -= 1;
			}
			else if(ldr_right > ldr_left)
			{
				motor_hor += 1;
			}
			if(motor_hor > 210)
			{
				motor_hor = 210;
			}
			if(motor_hor < 50)
			{
				motor_hor = 50;
			}
			FTM_set_PWM(FTM0, 0, motor_hor);
		}
	}

}
