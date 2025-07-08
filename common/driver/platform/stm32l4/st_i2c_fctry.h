
#pragma once

#include "alloc.h"
#include "exit.h"
#include "st_gpio_fctry.h"
#include "st_i2c.h"

bool GiveStI2c(I2c* i2c, Mem* mem, Timeout* timer, const uint32_t base_addr,
               const uint32_t timingr, const StGpioParams io1,
               const StGpioParams io2);

I2c* MakeStI2c(Mem* mem, Timeout* timer, const uint32_t base_addr,
               const uint32_t timingr, const StGpioParams io1,
               const StGpioParams io2);