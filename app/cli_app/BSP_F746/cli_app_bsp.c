
#include "cli_app_bsp.h"

#define EXIT_IF_FAIL(cond) EXIT_IF(!(cond), false)

static Mem memory;
static uint8_t driver_mem[DRIVER_MEM_SIZE] = {0};

bool BSP_Init(Usart* usart, I2c* temp_i2c, Gpio* led_gpio)
{
    EXIT_IF_FAIL(InitPrealloc(&memory, driver_mem, DRIVER_MEM_SIZE));

    // Single FRT timer.
    Timeout* time = make_frt_timer(&memory, 100);

    // LED GPIO
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOBEN;

    EXIT_IF_FAIL(
        GiveStGpio(led_gpio, &memory,
                   (StGpioParams){{0}, GPIOB_BASE, 0, {1, 0, 0, 0, 0}}));

    // USART3
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIODEN;
    RCC->APB1ENR |= RCC_APB1ENR_USART3EN;

    NVIC_SetPriorityGrouping(0);
    NVIC_SetPriority(USART3_IRQn, NVIC_EncodePriority(0, 6, 0));
    NVIC_EnableIRQ(USART3_IRQn);

    // PD8/9 AF 7
    EXIT_IF_FAIL(GiveStUsart(
        usart, &memory, time, USART3_BASE, SystemCoreClock, 115200,
        (StGpioParams){{0}, GPIOD_BASE, 8, {ALT_FUNC, 0, 0, 0, 0x7}},
        (StGpioParams){{0}, GPIOD_BASE, 9, {ALT_FUNC, 0, 0, 0, 0x7}}));

    // I2C1
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOBEN;
    RCC->APB1ENR |= RCC_APB1ENR_I2C1EN;

    // PB8/9 AF 4
    const StGpioSettings i2c_conf = {ALT_FUNC, OPEN_DRAIN, 0, PULL_UP, 0x4};
    EXIT_IF_FAIL(GiveStI2c(temp_i2c, &memory, time, I2C1_BASE, 0x20B,
                           (StGpioParams){{0}, GPIOB_BASE, 8, i2c_conf},
                           (StGpioParams){{0}, GPIOB_BASE, 9, i2c_conf}));

    return true;
}

void USART3_IRQHandler(void)
{
    cli_usart_rx_callback();
}