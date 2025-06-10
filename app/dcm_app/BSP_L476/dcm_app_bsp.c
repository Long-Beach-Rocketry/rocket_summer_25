#include "dcm_app_bsp.h"
#include "stm32l4xx_hal.h"

static DCPosControl st_motor;

static Mem memory;
static uint8_t driver_mem[DRIVER_MEM_SIZE] = {0};

static StQenc st_enc;
static StPrivPwm st_pwm;

static StGpioParams direction_param = {{0},
                                       GPIOA_BASE,
                                       0,
                                       {GPOUT, 0, 0, 0, 0x0}};  //PIN PA0
static StGpioParams brake_param = {{0},
                                   GPIOA_BASE,
                                   1,
                                   {GPOUT, 0, 0, 0, 0x0}};  //PIN PA1
static StGpioParams pwm_param = {{0},
                                 GPIOA_BASE,
                                 5,
                                 {ALT_FUNC, 0, 0, 0, 0x1}};  //TIM2 channel 1

static StGpioParams channel_a_param = {
    {0},
    GPIOA_BASE,
    6,
    {ALT_FUNC, 0, 0, 0, 0x2}};  //TIM3 channel 1

static StGpioParams channel_b_param = {
    {0},
    GPIOA_BASE,
    7,
    {ALT_FUNC, 0, 0, 0, 0x2}};  //TIM3 channel 2

#define EXIT_IF_FAIL(cond) EXIT_IF(!(cond), false)

bool BSP_Init(QEnc* enc, Gpio* channel_a, Gpio* channel_b, DCMotor* motor,
              DCM_Control* control, Gpio* brake, Gpio* direction,
              Gpio* gpio_pwm, Pwm* pwm, Usart* usart)
{

    RCC->AHB2ENR |= RCC_AHB2ENR_GPIOAEN | RCC_AHB2ENR_GPIOCEN;
    RCC->APB1ENR1 |= RCC_APB1ENR1_TIM3EN | RCC_APB1ENR1_TIM2EN;

    StEncInit(enc, &st_enc, channel_a, channel_b, TIM3_BASE);

    RCC->APB1ENR1 |= RCC_APB1ENR1_USART2EN;

    EXIT_IF_FAIL(InitPrealloc(&memory, driver_mem, DRIVER_MEM_SIZE));

    Timeout* time = make_frt_timer(&memory, 100);

    NVIC_SetPriorityGrouping(0);
    NVIC_SetPriority(USART2_IRQn, NVIC_EncodePriority(0, 6, 0));
    NVIC_EnableIRQ(USART2_IRQn);

    // PA2/3 AF 7
    EXIT_IF_FAIL(GiveStUsart(
        usart, &memory, time, USART2_BASE, SystemCoreClock, 115200,
        (StGpioParams){{0}, GPIOA_BASE, 2, {ALT_FUNC, 0, 0, 0, 0x7}},
        (StGpioParams){{0}, GPIOA_BASE, 3, {ALT_FUNC, 0, 0, 0, 0x7}}));

    StGpioInit(brake, &brake_param);
    StGpioConfig(brake);

    StGpioInit(direction, &direction_param);
    StGpioConfig(direction);

    StGpioInit(gpio_pwm, &pwm_param);
    StGpioConfig(gpio_pwm);

    StGpioInit(channel_a, &channel_a_param);
    StGpioConfig(channel_a);

    StGpioInit(channel_b, &channel_b_param);
    StGpioConfig(channel_b);

    StPwmInit(pwm, &st_pwm, TIM2_BASE, 84000000);

    StDcmInit(motor, &st_motor, brake, direction, pwm);
    DCM_Control_Init(control, motor, 100, enc);

    return true;
}
