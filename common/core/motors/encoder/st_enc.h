#pragma once

#include "gpio.h"
#include "qenc.h"
#include "stm32l476xx.h"

typedef struct StQenc StQenc;

struct StQenc
{
    TIM_TypeDef* instance;
    size_t count;
    Gpio* channel_a;
    Gpio* channel_b;
};

void StEncInit(QEnc* enc, StQenc* st_enc, Gpio* channel_a, Gpio* channel_b,
               size_t base_addr);
size_t StGetEncCount(QEnc* qenc);