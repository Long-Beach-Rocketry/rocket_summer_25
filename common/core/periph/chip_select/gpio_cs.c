
#include "gpio_cs.h"

ChipSelect* MakeGpioCs(Mem* mem, Gpio* pin, bool activeLow)
{
    ChipSelect* cs = ALLOC(mem, ChipSelect);
    GpioChipSelect* gpio_cs = ALLOC(mem, GpioChipSelect);
    EXIT_IF((cs == NULL) || (gpio_cs == NULL) || (pin == NULL), NULL);
    GpioCsInit(cs, gpio_cs, pin, activeLow);
    GpioCsConfig(cs);
    return cs;
}

void GpioCsInit(ChipSelect* cs, GpioChipSelect* gpio_cs, Gpio* pin,
                bool activeLow)
{
    gpio_cs->pin = pin;
    gpio_cs->activeLow = activeLow;
    cs->priv = (void*)gpio_cs;
    cs->select = GpioCsSelect;
    cs->deselect = GpioCsDeselect;
}

void GpioCsConfig(ChipSelect* cs)
{
    GpioCsDeselect(cs);
}

bool GpioCsSelect(ChipSelect* cs)
{
    GpioChipSelect* dev = (GpioChipSelect*)cs->priv;
    dev->pin->set(dev->pin, !dev->activeLow);
    return true;
}

bool GpioCsDeselect(ChipSelect* cs)
{
    GpioChipSelect* dev = (GpioChipSelect*)cs->priv;
    dev->pin->set(dev->pin, dev->activeLow);
    return true;
}