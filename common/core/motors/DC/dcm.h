/**
 *
 *@brief this file sets up the dc motor driver for use with different hardware
 *
 */

#pragma once

#include <stdbool.h>
#include "gpio.h"
#include "pwm.h"

/**
 * 
 *@brief this struct creates the needed function for the dc motor
 * 
 */

typedef struct DCMotor DCMotor;

struct DCMotor
{

    /**
     * 
     * @brief this function enables or disables the motor
     * 
     * @param motor is a pointer to current motor instance
     * @param enable true to enable, false to disable
     * 
     * @return true if motor was enabled, false if motor disabled
     */
    bool (*set_en)(DCMotor* motor, bool enable);
    /**
     * 
     * @brief this function set the direction the motor will travel
     * 
     * @param motor a pointer to current motor instance
     * @param direction true for Clockwise, flase for Counter-Clockwise
     * 
     * @return true if a direction was commanded, false otherise
     */
    bool (*set_direction)(DCMotor* motor, bool direction);
    /**
     * 
     * @brief this function sets the duty cycle for the motor
     * 
     * @param motor pointer to current motor instance
     * @param duty desired duty cycle as a percentage
     * 
     */
    void (*set_duty)(DCMotor* motor, float duty);

    /**
     * 
     * @brief A private context parameter for concrete implementations to use.
     */
    void* priv;
};
