
#include "st_i2c_fctry.h"

bool GiveStI2c(I2c* i2c, Mem* mem, Timeout* timer, const uint32_t base_addr,
               const uint32_t timingr, const StGpioParams io1,
               const StGpioParams io2)
{
    EXIT_IF((timer == NULL), false);
    StPrivI2c* st = ALLOC(mem, StPrivI2c);
    EXIT_IF(st == NULL, false);

    st->instance = (I2C_TypeDef*)base_addr;
    st->timer = timer;
    EXIT_IF(MakeStGpio(mem, io1) == NULL, false);
    EXIT_IF(MakeStGpio(mem, io2) == NULL, false);

    StI2cInit(i2c, st, base_addr, timer);
    StI2cConfig(i2c, timingr);

    return true;
}

I2c* MakeStI2c(Mem* mem, Timeout* timer, const uint32_t base_addr,
               const uint32_t timingr, const StGpioParams io1,
               const StGpioParams io2)
{
    I2c* i2c = ALLOC(mem, I2c);
    EXIT_IF(i2c == NULL, NULL);
    EXIT_IF(!GiveStI2c(i2c, mem, timer, base_addr, timingr, io1, io2), NULL);
    return i2c;
}