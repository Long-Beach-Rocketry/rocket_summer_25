/**
 * @brief this file sets up the PWM drivers for use with various timers
 */

#pragma once

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

/**
  * @brief structure that contains the functions needed to intialize and setup specific timers usint PWM drivers.
  */
typedef struct Pwm Pwm;

struct Pwm
{
    /**
      * @brief Enables the timers and their registers to setup PWM.
      * 
      * @param pwm A pointer to the peripheral instance.
      * @param enable True to enable the PWM peripheral, false to disable.
      * 
      * @return True on success, false otherwise.
      */
    bool (*enable)(Pwm* pwm, bool enable);

    /**
      * @brief Sets the desired pwm frequency to operate at.
      * 
      * @param pwm A pointer to the peripheral instance.
      * @param hz Desired frequency in Hz set by user.
      * 
      */
    void (*setFreq)(Pwm* pwm, size_t hz);

    /**
      * @brief Sets the desired duty cycle for pwm device.
      * 
      * @param pwm A pointer to the peripheral instance.
      * @param duty Desired duty cycle given as a percentage of the period.
      * 
      */
    void (*setDuty)(Pwm* pwm, double duty);

    /**
      * @brief A private context parameter for concrete implementations to use.
      * 
      */
    void* priv;
};
