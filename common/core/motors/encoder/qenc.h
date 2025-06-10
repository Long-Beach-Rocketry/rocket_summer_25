#pragma once

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
//#include "stm32l4xx.h"

typedef struct QEnc QEnc;

struct QEnc
{
    size_t (*getTicks)(QEnc* qenc);

    void* priv;
};