#pragma once

#include "alloc.h"
#include "exit.h"
#include "st_gpio_fctry.h"
#include "st_spi.h"

bool GiveStSpi(Spi* spi, Mem* mem, Timeout* timer, ChipSelect* cs,
               const uint32_t base_addr, const StGpioParams io1,
               const StGpioParams io2, const StGpioParams io3);

Spi* MakeStSpi(Mem* mem, Timeout* timer, ChipSelect* cs,
               const uint32_t base_addr, const StGpioParams io1,
               const StGpioParams io2, const StGpioParams io3);