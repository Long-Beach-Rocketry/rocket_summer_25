
#pragma once

#include "alloc.h"
#include "exit.h"
#include "st_gpio_fctry.h"
#include "st_pwm.h"

bool GiveStPwm(Pwm* pwm, Mem* mem, const uint32_t base_addr,
               const size_t channel, const size_t periph_clk_freq,
               const size_t timer_size, const StGpioParams io1);

Pwm* MakeStPwm(Mem* mem, const uint32_t base_addr, const size_t channel,
               const size_t periph_clk_freq, const size_t timer_size,
               const StGpioParams io1);