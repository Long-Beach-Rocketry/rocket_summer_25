
#pragma once

#include "alloc.h"
#include "exit.h"
#include "st_gpio_fctry.h"
#include "st_usart.h"

bool GiveStUsart(Usart* usart, Mem* mem, Timeout* timer,
                 const uint32_t base_addr, const uint32_t sys_core_clk,
                 const uint32_t baudrate, const StGpioParams io1,
                 const StGpioParams io2);

Usart* MakeStUsart(Mem* mem, Timeout* timer, const uint32_t base_addr,
                   const uint32_t sys_core_clk, const uint32_t baudrate,
                   const StGpioParams io1, const StGpioParams io2);