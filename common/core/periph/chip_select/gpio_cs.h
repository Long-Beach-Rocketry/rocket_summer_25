
#pragma once

#include "alloc.h"
#include "exit.h"
#include "gpio.h"
#include "spi.h"

typedef struct
{
    bool activeLow;
    Gpio* pin;
} GpioChipSelect;

ChipSelect* MakeGpioCs(Mem* mem, Gpio* pin, bool activeLow);
void GpioCsInit(ChipSelect* cs, GpioChipSelect* gpio_cs, Gpio* pin,
                bool activeLow);
void GpioCsConfig(ChipSelect* cs);
bool GpioCsSelect(ChipSelect* cs);
bool GpioCsDeselect(ChipSelect* cs);
