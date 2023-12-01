/*
 * TRACKER Module
 * This module implements functions to control a tracker system based on LDR sensor readings.
 */

#include "TRACKER.h"
#include "CONFIG.h"
#include "LDR.h"
#include "FTM.h"

// Constants for tolerance and motor position limits
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

// LDR sensor readings
uint16_t ldr_lu;
uint16_t ldr_ld;
uint16_t ldr_ru;
uint16_t ldr_rd;

// Mapped LDR values
uint16_t ldr_up;
uint16_t ldr_down;
uint16_t ldr_left;
uint16_t ldr_right;

// Delta values for vertical and horizontal movements
int16_t ldr_delta_ver;
int16_t ldr_delta_hor;

// Motor positions
uint8_t motor_ver = MOTOR_VER_ORIGIN;
uint8_t motor_hor = MOTOR_HOR_ORIGIN;

// Tracker flag
uint8_t tracker_flag = 0;

/*

 * Checks if the given delta value is within the tolerance range.
 *
 * delta is the difference value to check.
 *
 *
 */
uint8_t TRACKER_check_tol(int16_t delta)
{
    if ((-1 * TOL) > delta || delta > TOL)
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}

/*
 *
 * Turns on the tracker flag.
 * The tracker flag signals that a tracking operation needs to be performed.
 */
void TRACKER_flag_on()
{
    tracker_flag = 1;
}

/*

 * Handles the tracker operation based on LDR sensor readings.
 */
void TRACKER_handler()
{
    if (TRUE == tracker_flag)
    {
        tracker_flag = 0;

        // Read LDR sensor values
        ldr_lu = LDR_read(LDR_LU);
        ldr_ld = LDR_read(LDR_LD);
        ldr_ru = LDR_read(LDR_RU);
        ldr_rd = LDR_read(LDR_RD);

        // Map LDR values to a common scale
        ldr_lu = ((ldr_lu - LDR_LEFT_MAP_MIN) * LDR_MAX_MAP) / LDR_LEFT_MAP_DELTA;
        ldr_ld = ((ldr_ld - LDR_LEFT_MAP_MIN) * LDR_MAX_MAP) / LDR_LEFT_MAP_DELTA;
        ldr_ru = (((ldr_ru - LDR_RIGHT_MAP_MIN) * LDR_MAX_MAP) / LDR_RIGHTU_MAP_DELTA);
        ldr_rd = ((ldr_rd - LDR_RIGHT_MAP_MIN) * LDR_MAX_MAP) / LDR_RIGHTD_MAP_DELTA;

        // Calculate sums for vertical and horizontal movements
        ldr_up = ldr_lu + ldr_ru;
        ldr_down = ldr_ld + ldr_rd;
        ldr_left = ldr_ld + ldr_lu;
        ldr_right = ldr_ru + ldr_rd;

        // Calculate delta values
        ldr_delta_ver = ldr_up - ldr_down;
        ldr_delta_hor = ldr_left - ldr_right;

        // Adjust motor positions based on delta values within tolerance
        if (TRACKER_check_tol(ldr_delta_ver))
        {
            if (ldr_down > ldr_up)
            {
                motor_ver -= 1;
            }
            else if (ldr_up > ldr_down)
            {
                motor_ver += 1;
            }

            // Limit motor position
            if (motor_ver > MOTOR_VER_MAX)
            {
                motor_ver = MOTOR_VER_MAX;
            }
            if (motor_ver < MOTOR_VER_MIN)
            {
                motor_ver = MOTOR_VER_MIN;
            }

            // Set PWM for vertical motor
            FTM_set_PWM(FTM0, FTM_CHANNEL_1, motor_ver);
        }

        if (TRACKER_check_tol(ldr_delta_hor))
        {
            if (ldr_left > ldr_right)
            {
                motor_hor -= 1;
            }
            else if (ldr_right > ldr_left)
            {
                motor_hor += 1;
            }

            // Limit motor position
            if (motor_hor > MOTOR_HOR_MAX)
            {
                motor_hor = MOTOR_HOR_MAX;
            }
            if (motor_hor < MOTOR_HOR_MIN)
            {
                motor_hor = MOTOR_HOR_MIN;
            }

            // Set PWM for horizontal motor
            FTM_set_PWM(FTM0, FTM_CHANNEL_0, motor_hor);
        }
    }
}
