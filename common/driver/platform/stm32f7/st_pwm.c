
#include "st_pwm.h"

#define PWM_DUTY_PERCENT_ACCURACY(percent) (100 / (percent))
#define DESIRED_PWM_PERCENT_ACCURACY (0.5f)

bool StPwmInit(Pwm* pwm, StPrivPwm* st_pwm, size_t base_address, size_t channel,
               size_t pclk_freq, size_t timer_size)
{
    st_pwm->instance = (TIM_TypeDef*)base_address;

    if (channel < 1 || channel > 4)
    {
        return false;
    }

    st_pwm->channel = channel;
    st_pwm->pclk_freq = pclk_freq;
    st_pwm->timer_max = timer_size;
    st_pwm->curr_duty = 0;

    pwm->priv = (void*)st_pwm;
    pwm->set_duty = StPwmDuty;
    pwm->set_freq = StPwmSetFreq;

    StPwmSetFreq(pwm, 0);
    StPwmDuty(pwm, 0);

    return true;
}

void StPwmConfig(Pwm* pwm)
{
    StPrivPwm* dev = (StPrivPwm*)pwm->priv;

    switch (dev->channel)
    {
        case 1:
            dev->instance->CCMR1 |=
                TIM_CCMR1_OC1M_1 | TIM_CCMR1_OC1M_2 | TIM_CCMR1_OC1PE;
            dev->instance->CCER |= TIM_CCER_CC1E;
            break;
        case 2:
            dev->instance->CCMR1 |=
                TIM_CCMR1_OC2M_1 | TIM_CCMR1_OC2M_2 | TIM_CCMR1_OC2PE;
            dev->instance->CCER |= TIM_CCER_CC2E;
            break;
        case 3:
            dev->instance->CCMR2 |=
                TIM_CCMR2_OC3M_1 | TIM_CCMR2_OC3M_2 | TIM_CCMR2_OC3PE;
            dev->instance->CCER |= TIM_CCER_CC3E;
            break;
        case 4:
            dev->instance->CCMR2 |=
                TIM_CCMR2_OC4M_1 | TIM_CCMR2_OC4M_2 | TIM_CCMR2_OC4PE;
            dev->instance->CCER |= TIM_CCER_CC4E;
            break;
        default:
            break;
    }

    dev->instance->EGR |= TIM_EGR_UG;
    dev->instance->CR1 &= 0;
    dev->instance->CR1 |= TIM_CR1_ARPE | TIM_CR1_CEN;
}

bool StPwmSetFreq(Pwm* pwm, size_t hz)
{
    StPrivPwm* dev = (StPrivPwm*)pwm->priv;

    size_t target_psc = 0;

    if (hz > 0)
    {
        if (((double)dev->pclk_freq <
             (hz * PWM_DUTY_PERCENT_ACCURACY(DESIRED_PWM_PERCENT_ACCURACY))))
        {
            return false;
        }

        target_psc =
            dev->pclk_freq /
            (hz * PWM_DUTY_PERCENT_ACCURACY(DESIRED_PWM_PERCENT_ACCURACY));
    }

    dev->instance->PSC = (target_psc > 0) ? target_psc - 1 : 0;
    dev->instance->ARR =
        (size_t)(PWM_DUTY_PERCENT_ACCURACY(DESIRED_PWM_PERCENT_ACCURACY) - 1);

    StPwmDuty(pwm, dev->curr_duty);

    return true;
}

bool StPwmDuty(Pwm* pwm, double duty)
{
    StPrivPwm* dev = (StPrivPwm*)pwm->priv;

    if ((duty > 100) || (duty < 0))
    {
        return false;
    }

    dev->curr_duty = duty;

    size_t cmp = (size_t)((duty / 100) * (dev->instance->ARR));

    switch (dev->channel)
    {
        case 1:
            dev->instance->CCR1 = cmp;
            break;
        case 2:
            dev->instance->CCR2 = cmp;
            break;
        case 3:
            dev->instance->CCR3 = cmp;
            break;
        case 4:
            dev->instance->CCR4 = cmp;
            break;
        default:
            break;
    }

    return true;
}