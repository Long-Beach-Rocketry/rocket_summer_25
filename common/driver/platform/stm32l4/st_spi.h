/**
 * STM32L4 SPI Driver
 */

#pragma once

#include "stm32l4xx.h"

#include "gpio.h"
#include "spi.h"
#include "timeout.h"

typedef struct
{
    SPI_TypeDef* instance;
    Timeout* timer;
} StPrivSpi;

void StSpiInit(Spi* spi, StPrivSpi* st_spi, uint32_t base_addr, Timeout* timer);
void StSpiConfig(Spi* spi);
bool StSpiSend(Spi* spi, const uint8_t* data, size_t size);
bool StSpiRead(Spi* spi, uint8_t* data, size_t size);
bool StSpiTransact(Spi* spi, const uint8_t* txdata, uint8_t* rxdata,
                   size_t size);
