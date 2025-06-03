
//for rayne flight module
#include "subscale_bsp.h"

#define EXIT_IF_FAIL(cond) EXIT_IF(!(cond), false)

static Mem memory;
static uint8_t driver_mem[DRIVER_MEM_SIZE] = {0};

bool BSP_Init(Usart* usart, Spi* spi, I2c* temp_i2c, Gpio* led_gpio)
{
    EXIT_IF_FAIL(InitPrealloc(&memory, driver_mem, DRIVER_MEM_SIZE));

    // Single FRT timer.
    Timeout* time = make_frt_timer(&memory, 100);

    // LED GPIO
    // LED GPIO setting just to blue
    //blue PB0
    //green PC7
    //red PC6
    RCC->AHB2ENR |= RCC_AHB2ENR_GPIOBEN;  //only gpio B
    RCC->AHB2ENR |= RCC_AHB2ENR_GPIOCEN;  //only gpio B

    EXIT_IF_FAIL(GiveStGpio(
        led_gpio, &memory,
        (StGpioParams){{0}, GPIOC_BASE, 6, {GPOUT, 0, 0, 0, 0}}));  //output

    // USART1
    RCC->AHB2ENR |= RCC_AHB2ENR_GPIOAEN;
    RCC->APB2ENR |= RCC_APB2ENR_USART1EN;

    NVIC_SetPriorityGrouping(0);  //highest priority
    NVIC_SetPriority(USART1_IRQn, NVIC_EncodePriority(0, 6, 0));
    NVIC_EnableIRQ(USART1_IRQn);

    //TX PA9
    //RX PA10
    EXIT_IF_FAIL(GiveStUsart(
        usart, &memory, time, USART1_BASE, SystemCoreClock, 115200,
        (StGpioParams){{0}, GPIOA_BASE, 9, {ALT_FUNC, 0, 0, 0, 0x7}},
        (StGpioParams){{0}, GPIOA_BASE, 10, {ALT_FUNC, 0, 0, 0, 0x7}}));

    // SPI1
    RCC->AHB2ENR |= RCC_AHB2ENR_GPIOAEN;
    RCC->APB2ENR |= RCC_APB2ENR_SPI1EN;

    // PA4 gpio cs
    Gpio* cs_pin = MakeStGpio(
        &memory, (StGpioParams){
                     {0}, GPIOA_BASE, 4, {GPOUT, PUSH_PULL, LOW, NO_PULL, 0}});

    // PA5 SCK
    // PA6 MISO
    // PA7 MOSI
    const StGpioSettings spi_io_conf = {ALT_FUNC, 0, 0, 0, 0x5};
    EXIT_IF_FAIL(GiveStSpi(spi, &memory, time,
                           MakeGpioCs(&memory, cs_pin, true), SPI1_BASE,
                           (StGpioParams){{0}, GPIOA_BASE, 5, spi_io_conf},
                           (StGpioParams){{0}, GPIOA_BASE, 6, spi_io_conf},
                           (StGpioParams){{0}, GPIOA_BASE, 7, spi_io_conf}));

    // I2C1
    RCC->AHB2ENR |= RCC_AHB2ENR_GPIOBEN;
    RCC->APB1ENR1 |= RCC_APB1ENR1_I2C1EN;

    // PB6 PB7 AF 4
    const StGpioSettings i2c_conf = {ALT_FUNC, OPEN_DRAIN, 0, PULL_UP, 0x4};
    EXIT_IF_FAIL(GiveStI2c(temp_i2c, &memory, time, I2C1_BASE, 0x20B,
                           (StGpioParams){{0}, GPIOB_BASE, 6, i2c_conf},
                           (StGpioParams){{0}, GPIOB_BASE, 7, i2c_conf}));

    return true;
}

void USART1_IRQHandler(void)
{
    cli_usart_rx_callback();
}

void SystemReset(void)
{
    NVIC_SystemReset();
}