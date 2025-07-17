#include "servo_app_bsp.h"
#include "FreeRTOS.h"
#include "stm32l4xx_hal.h"

static DSServo ds_servo;

static Mem memory;
static uint8_t driver_mem[DRIVER_MEM_SIZE] = {0};

static StPrivPwm st_pwm;

static StGpioParams pwm_param = {{0},
                                 GPIOA_BASE,
                                 5,
                                 {ALT_FUNC, 0, 0, 0, 0x1}};  //Pwm PIN PA5

#define EXIT_IF_FAIL(cond) EXIT_IF(!(cond), false)

bool BSP_Init(SERVOMotor* servo, Pwm* pwm, Gpio* gpio_pwm, size_t freq)
{

    RCC->AHB2ENR |= RCC_AHB2ENR_GPIOAEN | RCC_AHB2ENR_GPIOCEN;
    RCC->APB1ENR1 |= RCC_APB1ENR1_TIM3EN | RCC_APB1ENR1_TIM2EN;

    RCC->APB1ENR1 |= RCC_APB1ENR1_USART2EN;

    EXIT_IF_FAIL(InitPrealloc(&memory, driver_mem, DRIVER_MEM_SIZE));

    StGpioInit(gpio_pwm, &pwm_param);
    StGpioConfig(gpio_pwm);

    StPwmInit(pwm, &st_pwm, TIM2_BASE, 80000000, 1);

    StPwmSetFreq(pwm, freq);

    StPwmEnable(pwm, true);

    DsServoInit(&ds_servo, servo, pwm, freq);

    return true;
}
