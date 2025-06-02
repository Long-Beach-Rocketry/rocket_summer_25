/**
 * 
 */

#pragma once

#include "exit.h"
#include "freertos_timer.h"
#include "gpio_cs.h"
#include "prealloc.h"
#include "st_gpio_fctry.h"
#include "st_i2c_fctry.h"
#include "st_spi_fctry.h"
#include "st_usart_fctry.h"
#include "stm32l4xx_hal.h"
#include "usart_cli.h"

#define DRIVER_MEM_SIZE 0x256

bool BSP_Init(Usart* usart, Spi* spi, I2c* i2c, Gpio* led_gpio);

void USART3_IRQHandler(void);

void SystemReset(void);
