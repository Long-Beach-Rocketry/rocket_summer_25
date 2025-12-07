#include "st_gpio_fctry.h"

bool GiveStGpio(Gpio* gpio, Mem* mem, StGpioParams params)
{
    StGpioParams* p = ALLOC(mem, StGpioParams);
    EXIT_IF(p == NULL, false);

    *p = params;

    StGpioInit(gpio, p);
    StGpioConfig(gpio);

    return true;
}

Gpio* MakeStGpio(Mem* mem, StGpioParams params)
{
    Gpio* gpio = ALLOC(mem, Gpio);
    EXIT_IF(gpio == NULL, false);
    EXIT_IF(!GiveStGpio(gpio, mem, params), NULL);

    return gpio;
}
