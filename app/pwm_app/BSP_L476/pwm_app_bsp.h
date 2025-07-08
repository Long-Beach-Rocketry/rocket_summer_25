/**
 * 
 */

#pragma once

#include "freertos_timer.h"
#include "prealloc.h"
#include "st_gpio_fctry.h"
#include "st_pwm_fctry.h"
#include "st_usart_fctry.h"
#include "stm32l4xx_hal.h"
#include "usart_cli.h"

#define DRIVER_MEM_SIZE 0x256

extern TIM_HandleTypeDef htim1;

bool BSP_Init(Usart* usart, Pwm* pwm, Gpio* led_gpio);

void USART2_IRQHandler(void);

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void);

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void);
