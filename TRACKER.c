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
#define MOTOR_VER_ORIGIN	130
#define MOTOR_HOR_ORIGIN	162
#define MOTOR_VER_MAX		214
#define MOTOR_VER_MIN		110
#define MOTOR_HOR_MAX		210
#define MOTOR_HOR_MIN		50
#define LDR_MAX_MAP			100
#define LDR_LEFT_MAP_MIN	4
#define LDR_RIGHT_MAP_MIN	2789
#define LDR_LEFT_MAP_DELTA	4019
#define LDR_RIGHTU_MAP_DELTA	1082
#define LDR_RIGHTD_MAP_DELTA	1216

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

uint8_t motor_ver = MOTOR_VER_ORIGIN;
uint8_t motor_hor = MOTOR_HOR_ORIGIN;

uint8_t tracker_flag = 0;

uint8_t TRACKER_check_tol(int16_t delta)
{
	if(((-1*TOL)>delta) || (delta>TOL) )
		//if( (delta>TOL) )
	{
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}

void TRACKER_flag_on()
{
	tracker_flag = 1;
}

void TRACKER_handler()
{

	if(TRUE==tracker_flag)
	{
		tracker_flag = 0;
		ldr_lu = LDR_read(LDR_LU);
		ldr_ld = LDR_read(LDR_LD);
		ldr_ru = LDR_read(LDR_RU);
		ldr_rd = LDR_read(LDR_RD);

		ldr_lu = ((ldr_lu-LDR_LEFT_MAP_MIN)*LDR_MAX_MAP)/LDR_LEFT_MAP_DELTA;
		ldr_ld = ((ldr_ld-LDR_LEFT_MAP_MIN)*LDR_MAX_MAP)/LDR_LEFT_MAP_DELTA;
		ldr_ru = (((ldr_ru-LDR_RIGHT_MAP_MIN)*LDR_MAX_MAP)/LDR_RIGHTU_MAP_DELTA);
		ldr_rd = ((ldr_rd-LDR_RIGHT_MAP_MIN)*LDR_MAX_MAP)/LDR_RIGHTD_MAP_DELTA;

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



			if(motor_ver > MOTOR_VER_MAX)
			{
				motor_ver = MOTOR_VER_MAX;
			}
			if(motor_ver < MOTOR_VER_MIN)
			{
				motor_ver = MOTOR_VER_MIN;
			}
			FTM_set_PWM(FTM0, FTM_CHANNEL_1, motor_ver);
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
			if(motor_hor > MOTOR_HOR_MAX)
			{
				motor_hor = MOTOR_HOR_MAX;
			}
			if(motor_hor < MOTOR_HOR_MIN)
			{
				motor_hor = MOTOR_HOR_MIN;
			}
			FTM_set_PWM(FTM0, FTM_CHANNEL_0, motor_hor);
		}
	}

}
