
#include "st_spi_fctry.h"

bool GiveStSpi(Spi* spi, Mem* mem, Timeout* timer, ChipSelect* cs,
               const uint32_t base_addr, const StGpioParams io1,
               const StGpioParams io2, const StGpioParams io3)
{
    EXIT_IF((timer == NULL) || (cs == NULL), false);
    StPrivSpi* st = ALLOC(mem, StPrivSpi);
    EXIT_IF(st == NULL, false);

    st->instance = (SPI_TypeDef*)base_addr;
    st->timer = timer;
    EXIT_IF(MakeStGpio(mem, io1) == NULL, false);
    EXIT_IF(MakeStGpio(mem, io2) == NULL, false);
    EXIT_IF(MakeStGpio(mem, io3) == NULL, false);

    spi->cs = cs;
    StSpiInit(spi, st, base_addr, timer);
    StSpiConfig(spi);

    return true;
}

Spi* MakeStSpi(Mem* mem, Timeout* timer, ChipSelect* cs,
               const uint32_t base_addr, const StGpioParams io1,
               const StGpioParams io2, const StGpioParams io3)
{
    Spi* spi = ALLOC(mem, Spi);
    EXIT_IF(spi == NULL, NULL);
    EXIT_IF(!GiveStSpi(spi, mem, timer, cs, base_addr, io1, io2, io3), NULL);
    return spi;
}