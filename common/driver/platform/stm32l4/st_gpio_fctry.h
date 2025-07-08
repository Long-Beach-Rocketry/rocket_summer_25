
#pragma once

#include "alloc.h"
#include "exit.h"
#include "st_gpio.h"

bool GiveStGpio(Gpio* gpio, Mem* mem, StGpioParams params);

Gpio* MakeStGpio(Mem* mem, StGpioParams params);