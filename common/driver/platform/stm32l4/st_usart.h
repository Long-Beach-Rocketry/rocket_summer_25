

#pragma once

#include "stm32l4xx.h"

#include "gpio.h"
#include "timeout.h"
#include "usart.h"

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

typedef struct
{
    USART_TypeDef* instance;
    Timeout* timer;
} StPrivUsart;

void StUsartInit(Usart* usart, StPrivUsart* st_usart, uint32_t base_addr,
                 Timeout* timer);
void StUsartConfig(Usart* usart, uint32_t system_core_clk, uint32_t baudrate);
bool StUsartSend(Usart* usart, const uint8_t* data, size_t size);
bool StUsartRecv(Usart* usart, uint8_t* data, size_t size);
