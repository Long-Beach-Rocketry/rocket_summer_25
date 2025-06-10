
#include "st_pwm.h"
#include "stm32l476xx.h"

void StPwmInit(Pwm* pwm, StPrivPwm* st_pwm, size_t base_address,
               size_t mc_clock)
{
    st_pwm->instance = (TIM_TypeDef*)base_address;

    pwm->priv = (void*)st_pwm;
    st_pwm->clock = mc_clock;
    pwm->enable = StPwmEnable;
    pwm->setDuty = StPwmDuty;
    pwm->setFreq = StPwmSetFreq;

    StPwmEnable(pwm, false);
    StPwmSetFreq(pwm, 1000);
    StPwmDuty(pwm, 50);  //setting duty cycle to 50%
}

bool StPwmEnable(Pwm* pwm, bool enable)
{
    StPrivPwm* dev = (StPrivPwm*)pwm->priv;

    if (enable == true)
    {

        //CCMR1 is configuring channel PWM TIMx Channel 1
        dev->instance->CCMR1 &= ~TIM_CCMR1_CC1S;
        dev->instance->CCMR1 |= TIM_CCMR1_OC1M_2;
        dev->instance->CCMR1 |= TIM_CCMR1_OC1M_1;
        dev->instance->CCMR1 |= TIM_CCMR1_OC1PE;
        //enabling channel 1, setting polarity active high
        dev->instance->CCER &= ~TIM_CCER_CC1P;
        dev->instance->CCER |= TIM_CCER_CC1E;
        //configuring TIMX
        dev->instance->CR1 |= TIM_CR1_ARPE;
        dev->instance->CR1 &= ~TIM_CR1_CMS_Msk;
        dev->instance->CR1 &= ~TIM_CR1_DIR_Msk;
        dev->instance->CR1 &= ~TIM_CR1_CKD_Msk;
        dev->instance->EGR |= TIM_EGR_UG;
        //enabling counter
        dev->instance->CR1 |= TIM_CR1_CEN;

        return (dev->instance->CR1 & TIM_CR1_CEN);
    }
    else
    {
        dev->instance->CCMR1 &= 0x0;
        dev->instance->CCER &= 0x0;
        dev->instance->CR1 &= 0x0;
        dev->instance->EGR &= 0x0;

        return (!(dev->instance->CR1 & TIM_CR1_CEN));
    }
}

void StPwmSetFreq(Pwm* pwm, size_t hz)
{
    StPrivPwm* dev = (StPrivPwm*)pwm->priv;

    dev->period = 1000 / hz;
    size_t DesiredPSC = ((dev->clock) / 65535) / hz;

    dev->instance->PSC = DesiredPSC - 1;
    dev->instance->ARR = 65535 - 1;
}

void StPwmDuty(Pwm* pwm, double duty)
{
    StPrivPwm* dev = (StPrivPwm*)pwm->priv;

    double Duty = (size_t)((duty / 100) * (dev->instance->ARR));

    dev->instance->CCR1 = Duty;
}