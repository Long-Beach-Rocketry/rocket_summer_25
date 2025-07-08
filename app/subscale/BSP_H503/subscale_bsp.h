/**
 * 
 */

#pragma once

#include "freertos_timer.h"
#include "gpio_cs.h"
#include "st_gpio.h"
#include "st_i2c.h"
#include "st_spi.h"
#include "st_usart.h"
#include "stm32h5xx_hal.h"
#include "usart_cli.h"

void BSP_Init(Usart* usart, Spi* spi, I2c* i2c, Gpio* led_gpio);

void USART1_IRQHandler(void);

void SystemReset(void);

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
