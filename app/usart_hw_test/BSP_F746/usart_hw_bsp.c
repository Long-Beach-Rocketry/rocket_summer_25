
#include "usart_hw_bsp.h"

#define EXIT_IF_FAIL(cond) EXIT_IF(!(cond), false)

static Mem memory;
static uint8_t driver_mem[DRIVER_MEM_SIZE] = {0};

static Usart* u3;
static Usart* u4;
static RingBuffer* rb1;
static RingBuffer* rb2;

bool BSP_Init(Usart* cli_usart, Usart* comm_usart, Gpio* led_gpio)
{
    __disable_irq();

    EXIT_IF_FAIL(InitPrealloc(&memory, driver_mem, DRIVER_MEM_SIZE));

    // Single FRT timer.
    Timeout* time = make_frt_timer(&memory, 100);

    // LED GPIO
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOBEN;

    EXIT_IF_FAIL(
        GiveStGpio(led_gpio, &memory,
                   (StGpioParams){{0}, GPIOB_BASE, 0, {1, 0, 0, 0, 0}}));

    // Cli USART3
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIODEN;
    RCC->APB1ENR |= RCC_APB1ENR_USART3EN;

    NVIC_SetPriorityGrouping(0);
    NVIC_SetPriority(USART3_IRQn, NVIC_EncodePriority(0, 6, 0));
    NVIC_EnableIRQ(USART3_IRQn);

    // PD 8/9 AF7
    EXIT_IF_FAIL(GiveStUsart(
        cli_usart, &memory, time, USART3_BASE, SystemCoreClock, 115200,
        (StGpioParams){{0}, GPIOD_BASE, 8, {ALT_FUNC, 0, 0, 0, 0x7}},
        (StGpioParams){{0}, GPIOD_BASE, 9, {ALT_FUNC, 0, 0, 0, 0x7}}));

    // Comm UART4
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOCEN;
    RCC->APB1ENR |= RCC_APB1ENR_UART4EN;

    NVIC_SetPriorityGrouping(0);
    NVIC_SetPriority(UART4_IRQn, NVIC_EncodePriority(0, 6, 0));
    NVIC_EnableIRQ(UART4_IRQn);

    // UART4: PC 10/11 AF 8
    // TXE/RXE: PC12, PD2
    EXIT_IF_FAIL(GiveSnx5176b(
        comm_usart, &memory,
        MakeStUsart(
            &memory, time, UART4_BASE, SystemCoreClock, 921600,
            (StGpioParams){{0}, GPIOC_BASE, 10, {ALT_FUNC, 0, 0, 0, 0x8}},
            (StGpioParams){{0}, GPIOC_BASE, 11, {ALT_FUNC, 0, 0, 0, 0x8}}),
        MakeStGpio(&memory,
                   (StGpioParams){{0}, GPIOC_BASE, 12, {1, 0, 0, 0, 0}}),
        MakeStGpio(&memory,
                   (StGpioParams){{0}, GPIOD_BASE, 2, {1, 0, 0, 0, 0}})));

    u3 = cli_usart;
    u4 = comm_usart;
    rb1 = make_ring_buffer(&memory, UART_PIPE_BUF_SIZE);
    rb2 = make_ring_buffer(&memory, UART_PIPE_BUF_SIZE);

    __enable_irq();

    return true;
}

void USART3_IRQHandler(void)
{
    UartPipeCallback(u3, u4, rb1, UART_END_CHAR);
}

void UART4_IRQHandler(void)
{
    UartPipeCallback(u4, u3, rb2, UART_END_CHAR);
}