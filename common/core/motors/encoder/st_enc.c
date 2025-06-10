#include "st_enc.h"

<<<<<<< HEAD
void StEncInit(QEnc* enc, StQenc* st_enc, Gpio* channel_a, Gpio* channel_b,
               size_t base_addr)
{
    st_enc->instance = (TIM_TypeDef*)base_addr;

    st_enc->instance->ARR = UINT16_MAX;
=======
void StEncInit(QEnc* enc, StQenc* st_enc, size_t base_addr)
{
    st_enc->instance = (TIM_TypeDef*)base_addr;

    st_enc->instance->ARR = UINT32_MAX;
>>>>>>> a476dda1f3337ca90f54259427c7c27c974583df

    st_enc->instance->CCER &= ~TIM_CCER_CC1E;
    st_enc->instance->CCER &= ~TIM_CCER_CC2E;

<<<<<<< HEAD
    //configuring channel 1
    st_enc->instance->CCMR1 |= TIM_CCMR1_CC1S_0;
    st_enc->instance->CCER &= ~TIM_CCER_CC1P;
    st_enc->instance->CCER &= ~TIM_CCER_CC1NP;
    //configuring channel 2
    st_enc->instance->CCMR1 |= TIM_CCMR1_CC2S_0;
    st_enc->instance->CCER &= ~TIM_CCER_CC2P;
    st_enc->instance->CCER &= ~TIM_CCER_CC2NP;

    //enabling channel 1 and 2
=======
    st_enc->instance->CCMR1 = TIM_CCMR1_CC1S_0;
    st_enc->instance->CCMR1 = TIM_CCMR1_CC2S_0;
    st_enc->instance->CCER &= ~TIM_CCER_CC1P;
    st_enc->instance->CCER &= ~TIM_CCER_CC1NP;
    st_enc->instance->CCER &= ~TIM_CCER_CC2P;
    st_enc->instance->CCER &= ~TIM_CCER_CC2NP;

>>>>>>> a476dda1f3337ca90f54259427c7c27c974583df
    st_enc->instance->CCER |= TIM_CCER_CC1E;
    st_enc->instance->CCER |= TIM_CCER_CC2E;

    st_enc->instance->SMCR |= TIM_SMCR_SMS_0 | TIM_SMCR_SMS_1;
    st_enc->instance->CR1 |= TIM_CR1_CEN;

    st_enc->count = 0;

<<<<<<< HEAD
    st_enc->channel_a = channel_a;
    st_enc->channel_b = channel_b;

=======
>>>>>>> a476dda1f3337ca90f54259427c7c27c974583df
    enc->priv = (void*)st_enc;
    enc->getTicks = StGetEncCount;
}

size_t StGetEncCount(QEnc* enc)
{
    StQenc* dev = (StQenc*)enc->priv;
    return dev->instance->CNT;
}