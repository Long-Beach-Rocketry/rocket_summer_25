#include "pwm_app_bsp.h"

static StGpioParams led_stgpio = {{0}, GPIOA_BASE, 5, {ALT_FUNC, 0, 0, 0, 0x1}};

static StPrivPwm st_pwm;
// static size_t base_address = TIM2_BASE;
static size_t mc_clock = 80000000;

void BSP_Init(Pwm* pwm_timer, Gpio* led_gpio)
{

    // LED GPIO
    RCC->AHB2ENR |= RCC_AHB2ENR_GPIOAEN;
    RCC->APB1ENR1 |= RCC_APB1ENR1_TIM2EN;

    StGpioInit(led_gpio, &led_stgpio);
    StGpioConfig(led_gpio);

    StPwmInit(pwm_timer, &st_pwm, TIM2_BASE, mc_clock, 1);
}