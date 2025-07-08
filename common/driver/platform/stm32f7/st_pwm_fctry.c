
#include "st_pwm_fctry.h"

bool GiveStPwm(Pwm* pwm, Mem* mem, const uint32_t base_addr,
               const size_t channel, const size_t periph_clk_freq,
               const size_t timer_size, const StGpioParams io1)
{
    StPrivPwm* st = ALLOC(mem, StPrivPwm);
    EXIT_IF(st == NULL, false);

    EXIT_IF(MakeStGpio(mem, io1) == NULL, false);

    StPwmInit(pwm, st, base_addr, channel, periph_clk_freq, timer_size);
    StPwmConfig(pwm);

    return true;
}

Pwm* MakeStPwm(Mem* mem, const uint32_t base_addr, const size_t channel,
               const size_t periph_clk_freq, const size_t timer_size,
               const StGpioParams io1)
{
    Pwm* pwm = ALLOC(mem, Pwm);
    EXIT_IF(pwm == NULL, NULL);
    EXIT_IF(!GiveStPwm(pwm, mem, base_addr, channel, periph_clk_freq,
                       timer_size, io1),
            NULL);
    return pwm;
}
