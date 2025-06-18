/**
 * @brief this file is redefining previously stated functions
 * 
 */

#pragma once

#include "dcm.h"
#include "gpio.h"
#include "pwm.h"
#include "qenc.h"
#include "timeout.h"

#include <stdbool.h>

typedef struct
{

    Pwm* pwm;
    Gpio* brake;
    Gpio* direction;
    bool enable;

} DCPosControl;

/**
 * 
 * @brief this function initializes settings for a DCMotor object
 * 
 * @param motor a pointer to a DCMotor instance
 * @param control a pointer to a DCPosControl instance
 * @param brake a pointer to a Gpio instance for the brake pin
 * @param direction a pointer to a Gpio instance for the direction pin
 * @param pwm a point to an insatnce of a Pwm object
 * 
 */
void StDcmInit(DCMotor* motor, DCPosControl* control, Gpio* Brake,
               Gpio* direction, Pwm* pwm);
/**
 * 
 * @brief this function enables the motor
 * 
 * @param motor a pointer to a DCMotor instance
 * @param enable true to enable, false to disable
 * 
 * @return true is motor was enabled, false if disabled
 * 
 */
bool StDcmSetEnable(DCMotor* motor, bool enable);
/**
 * 
 * @brief this function set the direction the motor will travel
 * 
 * @param motor a pointer to current motor instance
 * @param direction true for Clockwise, flase for Counter-Clockwise
 * 
 * @return true if a direction was commanded, false otherise
 */
bool StDcmSetDir(DCMotor* motor, bool direction);

/**
 * 
 * @brief this function sets the duty cycle for the motor
 * 
 * @param motor pointer to current motor instance
 * @param duty desired duty cycle as a percentage
 * 
 */
void StDcmSetDuty(DCMotor* motor, float duty);
