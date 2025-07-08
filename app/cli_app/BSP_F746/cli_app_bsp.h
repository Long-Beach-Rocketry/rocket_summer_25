/**
 * 
 */

#pragma once

#include "exit.h"
#include "freertos_timer.h"
#include "prealloc.h"
#include "st_gpio_fctry.h"
#include "st_i2c_fctry.h"
#include "st_usart_fctry.h"
#include "stm32f746xx.h"
#include "stm32f7xx_hal.h"
#include "usart_cli.h"

#define DRIVER_MEM_SIZE 0x256

bool BSP_Init(Usart* usart, I2c* i2c, Gpio* led_gpio);

void USART3_IRQHandler(void);
