
#include "st_usart.h"

static bool StUsartClearErrors(StPrivUsart* dev)
{
    if (dev->instance->ISR & USART_ISR_ORE)
    {
        dev->instance->ICR |= USART_ICR_ORECF;
    }

    return true;
}

static bool StUsartIsrSet(StPrivUsart* dev, uint32_t mask)
{
    return dev->instance->ISR & mask;
}

void StUsartInit(Usart* usart, StPrivUsart* st_usart, uint32_t base_addr,
                 Timeout* timer)
{
    st_usart->instance = (USART_TypeDef*)base_addr;
    st_usart->timer = timer;

    usart->priv = (void*)st_usart;
    usart->send = StUsartSend;
    usart->recv = StUsartRecv;
}

void StUsartConfig(Usart* usart, uint32_t system_core_clk, uint32_t baudrate)
{
    StPrivUsart* dev = (StPrivUsart*)usart->priv;

    uint16_t uartdiv = system_core_clk / baudrate;
    dev->instance->BRR = (((uartdiv / 16) << USART_BRR_DIV_MANTISSA_Pos) |
                          ((uartdiv % 16) << USART_BRR_DIV_FRACTION_Pos));
    dev->instance->CR1 |= USART_CR1_UE | USART_CR1_RE | USART_CR1_TE;
    dev->instance->CR1 |= USART_CR1_RXNEIE;
}

bool StUsartSend(Usart* usart, const uint8_t* data, size_t size)
{
    StPrivUsart* dev = (StPrivUsart*)usart->priv;
    for (size_t i = 0; i < size; ++i)
    {

        WAIT(dev->timer, dev->instance->ISR & USART_ISR_TXE, false);
        dev->instance->TDR = data[i];
    }

    WAIT(dev->timer, dev->instance->ISR & USART_ISR_TC, false);

    return true;
}

bool StUsartRecv(Usart* usart, uint8_t* data, size_t size)
{
    StPrivUsart* dev = (StPrivUsart*)usart->priv;

    StUsartClearErrors(dev);

    if (!StUsartIsrSet(dev, USART_ISR_RXNE))
    {
        return false;
    }

    size_t count = 0;
    while (count < size)
    {
        WAIT(dev->timer, dev->instance->ISR & USART_ISR_RXNE, false);
        data[count++] = dev->instance->RDR;
    }

    return true;
}
