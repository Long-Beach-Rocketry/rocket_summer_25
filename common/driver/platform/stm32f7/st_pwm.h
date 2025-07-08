/**
 * @brief STM32F7 implementation of PWM interface.
 */

#pragma once

#include "pwm.h"

#include "stm32f7xx.h"

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

/**
 * @brief Private STM32L4 PWM implementation context.
 */
typedef struct
{
    /**
	 * Timer instance.
	 */
    TIM_TypeDef* instance;

    /**
	 * Timer channel number.
	 */
    size_t channel;

    /**
	 * Frequency of peripheral clock driving the timer.
	 */
    size_t pclk_freq;

    /**
	 * Max size of the timer.
	 */
    size_t timer_max;

    /**
	 * Current configured duty cycle.
	 */
    float curr_duty;
} StPrivPwm;

/**
  * @brief Initializes settings for a PWM object.
  * 
  * @param pwm A pointer to the peripheral instance.
  * @param st_pwm A pointer to a private data variable giving access to other private members.
  * @param base_address The base address of the PWM peripheral.
  * @param channel Timer channel number to configure.
  * @param pclk_freq Frequency in Hz of the PCLK driving the timer.
  * @param timer_size Max size of the timer counter, for example 65535 for a 16-bit.
  * 
  * @return True on success, false otherwise.
  */
bool StPwmInit(Pwm* pwm, StPrivPwm* st_pwm, size_t base_address, size_t channel,
               size_t pclk_freq, size_t timer_size);

/**
  * @brief Initializes settings for a PWM instance.
  * 
  * @param pwm A pointer to the peripheral instance.
  */
void StPwmConfig(Pwm* pwm);

/**
  * @brief Sets the desired pwm frequency to operate at.
  * 
  * @param pwm A pointer to the peripheral instance.
  * @param hz Desired frequency in Hz set by user.
  * 
  * @return True on success, false otherwise.
  */
bool StPwmSetFreq(Pwm* pwm, size_t hz);

/**
  * @brief Sets the desired duty cycle for pwm device.
  * 
  * @param pwm A pointer to the peripheral instance.
  * @param duty Desired duty cycle given as a percentage of the period (1000ms / Freq in Hz). 
  * 
  * @return True on success, false otherwise.
  */
bool StPwmDuty(Pwm* pwm, double duty);
