
#include "st_usart_fctry.h"

bool GiveStUsart(Usart* usart, Mem* mem, Timeout* timer,
                 const uint32_t base_addr, const uint32_t sys_core_clk,
                 const uint32_t baudrate, const StGpioParams io1,
                 const StGpioParams io2)
{
    EXIT_IF((timer == NULL), false);
    StPrivUsart* st = ALLOC(mem, StPrivUsart);
    EXIT_IF(st == NULL, false);

    st->instance = (USART_TypeDef*)base_addr;
    st->timer = timer;
    EXIT_IF(MakeStGpio(mem, io1) == NULL, false);
    EXIT_IF(MakeStGpio(mem, io2) == NULL, false);

    StUsartInit(usart, st, base_addr, timer);
    StUsartConfig(usart, sys_core_clk, baudrate);

    return true;
}

Usart* MakeStUsart(Mem* mem, Timeout* timer, const uint32_t base_addr,
                   const uint32_t sys_core_clk, const uint32_t baudrate,
                   const StGpioParams io1, const StGpioParams io2)
{
    Usart* u = ALLOC(mem, Usart);
    EXIT_IF(u == NULL, NULL);
    EXIT_IF(!GiveStUsart(u, mem, timer, base_addr, sys_core_clk, baudrate, io1,
                         io2),
            NULL);
    return u;
}