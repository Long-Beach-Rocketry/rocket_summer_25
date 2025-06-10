#include "st_enc.h"

void StEncInit(QEnc* enc, StQenc* st_enc, Gpio* channel_a, Gpio* channel_b,
               size_t base_addr)
{
    st_enc->instance = (TIM_TypeDef*)base_addr;

    st_enc->instance->ARR = UINT16_MAX;

    st_enc->instance->CCER &= ~TIM_CCER_CC1E;
    st_enc->instance->CCER &= ~TIM_CCER_CC2E;

    //configuring channel 1
    st_enc->instance->CCMR1 |= TIM_CCMR1_CC1S_0;
    st_enc->instance->CCER &= ~TIM_CCER_CC1P;
    st_enc->instance->CCER &= ~TIM_CCER_CC1NP;
    //configuring channel 2
    st_enc->instance->CCMR1 |= TIM_CCMR1_CC2S_0;
    st_enc->instance->CCER &= ~TIM_CCER_CC2P;
    st_enc->instance->CCER &= ~TIM_CCER_CC2NP;

    //enabling channel 1 and 2
    st_enc->instance->CCER |= TIM_CCER_CC1E;
    st_enc->instance->CCER |= TIM_CCER_CC2E;

    st_enc->instance->SMCR |= TIM_SMCR_SMS_0 | TIM_SMCR_SMS_1;
    st_enc->instance->CR1 |= TIM_CR1_CEN;

    st_enc->count = 0;

    st_enc->channel_a = channel_a;
    st_enc->channel_b = channel_b;

    enc->priv = (void*)st_enc;
    enc->getTicks = StGetEncCount;
}

size_t StGetEncCount(QEnc* enc)
{
    StQenc* dev = (StQenc*)enc->priv;
    return dev->instance->CNT;
}