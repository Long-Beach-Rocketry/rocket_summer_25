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

<<<<<<< HEAD
void StEncInit(QEnc* enc, StQenc* st_enc, Gpio* channel_a, Gpio* channel_b,
               size_t base_addr);
=======
void StEncInit(QEnc* enc, StQenc* st_enc, size_t base_addr);
>>>>>>> a476dda1f3337ca90f54259427c7c27c974583df
size_t StGetEncCount(QEnc* qenc);