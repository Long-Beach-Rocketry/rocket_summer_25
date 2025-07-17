#pragma once

#include "st_gpio.h"
#include "st_pwm.h"
#include "stm32l476xx.h"
#include "stm32l4xx_hal_tim.h"
#include "usart_cli.h"

void BSP_Init(Pwm* pwm_timer, Gpio* led_gpio);