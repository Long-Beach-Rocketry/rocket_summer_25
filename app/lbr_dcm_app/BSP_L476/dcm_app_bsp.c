#include "dcm_app_bsp.h"
#include "FreeRTOS.h"
#include "stm32l4xx_hal.h"

static DCPosControl st_motor_one;  // st_motor_two;

static Mem memory;
static uint8_t driver_mem[DRIVER_MEM_SIZE] = {0};

static StQenc st_enc_one;     // st_enc_two;
static StPrivPwm st_pwm_one;  // st_pwm_two;

#define EXIT_IF_FAIL(cond) EXIT_IF(!(cond), false)

bool BSP_Init_motor_one(QEnc* enc, Gpio* channel_a, Gpio* channel_b, Gpio* led,
                        DCMotor* motor, DCM_Control* control, Gpio* brake,
                        Gpio* direction, Gpio* gpio_pwm, Pwm* pwm, Usart* usart)
{

    RCC->AHB2ENR |=
        RCC_AHB2ENR_GPIOAEN | RCC_AHB2ENR_GPIOBEN | RCC_AHB2ENR_GPIOCEN;
    RCC->APB1ENR1 |= RCC_APB1ENR1_TIM3EN | RCC_APB1ENR1_TIM2EN;

    StEncInit(enc, &st_enc_one, channel_a, channel_b, TIM3_BASE);

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

    // PA4 Moto1 Brake Pin
    EXIT_IF_FAIL(
        GiveStGpio(brake, &memory,
                   (StGpioParams){{0}, GPIOA_BASE, 4, {GPOUT, 0, 0, 0, 0}}));

    // PA6 Motor 1 Direction Pin
    EXIT_IF_FAIL(
        GiveStGpio(direction, &memory,
                   (StGpioParams){{0}, GPIOA_BASE, 6, {GPOUT, 0, 0, 0, 0}}));

    // PA5 DC Motor 1 Pwm Pin
    EXIT_IF_FAIL(GiveStGpio(
        gpio_pwm, &memory,
        (StGpioParams){{0}, GPIOA_BASE, 5, {ALT_FUNC, 0, 0, 0, 0x1}}));

    // PB0 Motor 1 Encoder CH A
    EXIT_IF_FAIL(GiveStGpio(
        channel_a, &memory,
        (StGpioParams){{0}, GPIOB_BASE, 0, {ALT_FUNC, 0, 0, 0, 0x2}}));

    //PA7 Motor 1 Encoder CH B
    EXIT_IF_FAIL(GiveStGpio(
        channel_b, &memory,
        (StGpioParams){{0}, GPIOA_BASE, 7, {ALT_FUNC, 0, 0, 0, 0x2}}));

    //Led Pin TIM3 CH1 AF 2
    EXIT_IF_FAIL(GiveStGpio(
        led, &memory, (StGpioParams){{0}, GPIOC_BASE, 6, {GPOUT, 0, 0, 0, 0}}));

    // StPwmInit(pwm, &st_pwm_one, TIM2_BASE, 80000000);
    StPwmInit(pwm, &st_pwm_one, TIM3_BASE, 80000000);

    StPwmSetFreq(pwm, 1000);

    StDcmInit(motor, &st_motor_one, brake, direction, pwm);
    DCM_Control_Init(control, motor, 400, enc);

    return true;
}

// bool BSP_Init_motor_two(QEnc* enc, Gpio* channel_a, Gpio* channel_b,
//                         DCMotor* motor, DCM_Control* control, Gpio* brake,
//                         Gpio* direction, Gpio* gpio_pwm, Pwm* pwm, Usart* usart)
// {

//     RCC->AHB2ENR |= RCC_AHB2ENR_GPIOAEN | RCC_AHB2ENR_GPIOBEN;
//     RCC->APB1ENR1 |= RCC_APB1ENR1_TIM2EN;
//     RCC->APB2ENR |= RCC_APB2ENR_TIM1EN;

//     StEncInit(enc, &st_enc_two, channel_a, channel_b,
//               TIM2_BASE);  //motor 2 has not common timers
//     StEncInit(enc, &st_enc_two, channel_a, channel_b, TIM1_BASE);

//     RCC->APB1ENR1 |= RCC_APB1ENR1_USART2EN;

//     EXIT_IF_FAIL(InitPrealloc(&memory, driver_mem, DRIVER_MEM_SIZE));

//     Timeout* time = make_frt_timer(&memory, 100);

//     NVIC_SetPriorityGrouping(0);
//     NVIC_SetPriority(USART2_IRQn, NVIC_EncodePriority(0, 6, 0));
//     NVIC_EnableIRQ(USART2_IRQn);

//     // PA2/3 AF 7
//     EXIT_IF_FAIL(GiveStUsart(
//         usart, &memory, time, USART2_BASE, SystemCoreClock, 115200,
//         (StGpioParams){{0}, GPIOA_BASE, 2, {ALT_FUNC, 0, 0, 0, 0x7}},
//         (StGpioParams){{0}, GPIOA_BASE, 3, {ALT_FUNC, 0, 0, 0, 0x7}}));

//     // PA4 Motor 2 Brake Pin
//     EXIT_IF_FAIL(
//         GiveStGpio(brake, &memory,
//                    (StGpioParams){{0}, GPIOA_BASE, 0, {GPOUT, 0, 0, 0, 0}}));

//     // PA6 Motor 2 Direction Pin
//     EXIT_IF_FAIL(
//         GiveStGpio(direction, &memory,
//                    (StGpioParams){{0}, GPIOA_BASE, 2, {GPOUT, 0, 0, 0, 0}}));

//     // PA5 DC Motor 2 Pwm Pin
//     EXIT_IF_FAIL(GiveStGpio(
//         gpio_pwm, &memory,
//         (StGpioParams){{0}, GPIOA_BASE, 5, {ALT_FUNC, 0, 0, 0, 0x1}}));

//     // PB0 Motor 2 Encoder CH A
//     EXIT_IF_FAIL(GiveStGpio(
//         channel_a, &memory,
//         (StGpioParams){{0}, GPIOA_BASE, 3, {ALT_FUNC, 0, 0, 0, 0x1}}));

//     //PA7 Motor 2 Encoder CH B
//     EXIT_IF_FAIL(GiveStGpio(
//         channel_b, &memory,
//         (StGpioParams){{0}, GPIOB_BASE, 1, {ALT_FUNC, 0, 0, 0, 0x2}}));

//     StPwmInit(pwm, &st_pwm_two, TIM2_BASE, 80000000);

//     StPwmSetFreq(pwm, 1500);

//     StDcmInit(motor, &st_motor_two, brake, direction, pwm);
//     DCM_Control_Init(control, motor, 400, enc);

//     return true;
// }
