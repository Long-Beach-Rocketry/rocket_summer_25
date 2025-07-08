/**
 * 
 * 
 */

#pragma once

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

typedef struct ChipSelect ChipSelect;

struct ChipSelect
{
    bool (*select)(ChipSelect* cs);
    bool (*deselect)(ChipSelect* cs);
    void* priv;
};

typedef struct Spi Spi;

struct Spi
{
    ChipSelect* cs;
    bool (*send)(Spi* spi, const uint8_t* data, size_t size);
    bool (*read)(Spi* spi, uint8_t* data, size_t size);
    bool (*transact)(Spi* spi, const uint8_t* txdata, uint8_t* rxdata,
                     size_t size);
    void* priv;
};
