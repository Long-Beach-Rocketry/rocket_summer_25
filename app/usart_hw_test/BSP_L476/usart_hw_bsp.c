
#include "usart_hw_bsp.h"

#define EXIT_IF_FAIL(cond) EXIT_IF(!(cond), false)

static Mem memory;
static uint8_t driver_mem[DRIVER_MEM_SIZE] = {0};

static RingBuffer* rb1;
static RingBuffer* rb2;

Snx5176b rs485;
Usart* u2;
Usart* u3;

bool BSP_Init(Usart* cli_usart, Usart* comm_usart, Gpio* led_gpio)
{
    HAL_InitTick(0);
    SystemClock_Config();

    EXIT_IF_FAIL(InitPrealloc(&memory, driver_mem, DRIVER_MEM_SIZE));

    // Single FRT timer.
    Timeout* time = make_frt_timer(&memory, 100);

    // LED GPIO
    RCC->AHB2ENR |= RCC_AHB2ENR_GPIOAEN;
    EXIT_IF_FAIL(
        GiveStGpio(led_gpio, &memory,
                   (StGpioParams){{0}, GPIOA_BASE, 5, {GPOUT, 0, 0, 0, 0}}));

    // CLI USART2
    RCC->AHB2ENR |= RCC_AHB2ENR_GPIOAEN;
    RCC->APB1ENR1 |= RCC_APB1ENR1_USART2EN;

    NVIC_SetPriorityGrouping(0);
    NVIC_SetPriority(USART2_IRQn, NVIC_EncodePriority(0, 6, 0));
    NVIC_EnableIRQ(USART2_IRQn);

    // PA2/3 AF 7
    EXIT_IF_FAIL(GiveStUsart(
        cli_usart, &memory, time, USART2_BASE, SystemCoreClock, 115200,
        (StGpioParams){{0}, GPIOA_BASE, 2, {ALT_FUNC, 0, 0, 0, 0x7}},
        (StGpioParams){{0}, GPIOA_BASE, 3, {ALT_FUNC, 0, 0, 0, 0x7}}));

    // Comm USART3
    RCC->AHB2ENR |= RCC_AHB2ENR_GPIOCEN;
    RCC->AHB2ENR |= RCC_AHB2ENR_GPIODEN;
    RCC->APB1ENR1 |= RCC_APB1ENR1_USART3EN;

    NVIC_SetPriorityGrouping(0);
    NVIC_SetPriority(USART3_IRQn, NVIC_EncodePriority(0, 6, 0));
    NVIC_EnableIRQ(USART3_IRQn);

    EXIT_IF_FAIL(GiveStUsart(
        rs485.usart, &memory, time, USART3_BASE, SystemCoreClock, 921600,
        (StGpioParams){{0}, GPIOC_BASE, 10, {ALT_FUNC, 0, 0, 0, 0x7}},
        (StGpioParams){{0}, GPIOC_BASE, 11, {ALT_FUNC, 0, 0, 0, 0x7}}));

    rb1 = make_ring_buffer(&memory, UART_PIPE_BUF_SIZE);
    rb2 = make_ring_buffer(&memory, UART_PIPE_BUF_SIZE);

    EXIT_IF_FAIL(
        GiveStGpio(rs485.txe, &memory,
                   (StGpioParams){{0}, GPIOC_BASE, 12, {1, 0, 0, 0, 0}}));

    EXIT_IF_FAIL(
        GiveStGpio(rs485.rxe, &memory,
                   (StGpioParams){{0}, GPIOD_BASE, 2, {1, 0, 0, 0, 0}}));

    Snx5176bInit(comm_usart, &rs485);
    Snx5176bConfig(&rs485);

    u2 = cli_usart;
    u3 = comm_usart;

    return true;
}

void USART2_IRQHandler(void)
{
    UartPipeCallback(u2, u3, rb1, UART_END_CHAR);
}

void USART3_IRQHandler(void)
{
    UartPipeCallback(u3, u2, rb2, UART_END_CHAR);
}

void TIM1_UP_TIM16_IRQHandler(void)
{
    HAL_TIM_IRQHandler(&htim1);
}

void SystemClock_Config(void)
{
    RCC_OscInitTypeDef RCC_OscInitStruct = {0};
    RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

    /** Configure the main internal regulator output voltage
     */
    if (HAL_PWREx_ControlVoltageScaling(PWR_REGULATOR_VOLTAGE_SCALE1) != HAL_OK)
    {
        Error_Handler();
    }

    /** Initializes the RCC Oscillators according to the specified parameters
    in the RCC_OscInitTypeDef structure.*/
    RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
    RCC_OscInitStruct.HSIState = RCC_HSI_ON;
    RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
    RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
    RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
    RCC_OscInitStruct.PLL.PLLM = 1;
    RCC_OscInitStruct.PLL.PLLN = 10;
    RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV7;
    RCC_OscInitStruct.PLL.PLLQ = RCC_PLLQ_DIV2;
    RCC_OscInitStruct.PLL.PLLR = RCC_PLLR_DIV2;

    if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
    {
        Error_Handler();
    }

    /** Initializes the CPU, AHB and APB buses clocks
     */
    RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK |
                                  RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
    RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
    RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
    RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
    RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

    if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_4) != HAL_OK)
    {
        Error_Handler();
    }
}

void Error_Handler(void)
{
    /* USER CODE BEGIN Error_Handler_Debug */
    /* User can add his own implementation to report the HAL error return state */
    __disable_irq();
    while (1)
    {
        GPIOA->ODR |= GPIO_ODR_OD0;
    }
    /* USER CODE END Error_Handler_Debug */
}