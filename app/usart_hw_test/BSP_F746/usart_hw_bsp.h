/**
 * 
 */

#pragma once

#include "exit.h"
#include "freertos_timer.h"
#include "prealloc.h"
#include "snx5176b.h"
#include "st_gpio_fctry.h"
#include "st_usart_fctry.h"
#include "stm32f746xx.h"
#include "stm32f7xx_hal.h"
#include "uart_pipe.h"

#define DRIVER_MEM_SIZE 0x256
#define UART_PIPE_BUF_SIZE 256
#define UART_END_CHAR '\n'

bool BSP_Init(Usart* cli_usart, Usart* comm_usart, Gpio* led_gpio);

void USART3_IRQHandler(void);

void UART4_IRQHandler(void);
