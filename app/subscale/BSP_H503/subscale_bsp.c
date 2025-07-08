
#include "subscale_bsp.h"

static StPrivUsart st_usart;
static StPrivSpi st_spi;
static GpioChipSelect st_spi_cs;
static StPrivI2c st_i2c;
static StGpioParams led_stgpio = {{0}, GPIOA_BASE, 5, {1, 0, 0, 0, 0}};
static StGpioParams spi_cs_io = {{0}, GPIOC_BASE, 1, {1, 0, 0, 0, 0}};

// Sequential use of these, so using one is fine. Not thread safe.
static Timeout time;
static FrtTimerData frt;

static StGpioParams uart_io1 = {{0},
                                GPIOB_BASE,
                                6,
                                {ALT_FUNC, 0, 0, 0, 0x7}};  // USART1 AF 7
static StGpioParams uart_io2 = {{0},
                                GPIOB_BASE,
                                7,
                                {ALT_FUNC, 0, 0, 0, 0x7}};  // USART1 AF 7

static StGpioParams spi_io1 = {{0},
                               GPIOC_BASE,
                               0,
                               {ALT_FUNC, 0, VERY_HIGH, 0, 0x5}};
static StGpioParams spi_io2 = {{0},
                               GPIOC_BASE,
                               2,
                               {ALT_FUNC, 0, VERY_HIGH, 0, 0x4}};
static StGpioParams spi_io3 = {{0},
                               GPIOC_BASE,
                               3,
                               {ALT_FUNC, 0, VERY_HIGH, 0, 0x4}};

const StGpioSettings i2c_io_conf = {ALT_FUNC, OPEN_DRAIN, 0, PULL_UP, 0x4};

static StGpioParams i2c1_io1 = {{0}, GPIOC_BASE, 8, i2c_io_conf};
static StGpioParams i2c1_io2 = {{0}, GPIOC_BASE, 9, i2c_io_conf};

void BSP_Init(Usart* usart, Spi* spi, I2c* temp_i2c, Gpio* led_gpio)
{
    HAL_InitTick(0);
    SystemClock_Config();

    // LED GPIO
    RCC->AHB2ENR |= RCC_AHB2ENR_GPIOAEN;

    StGpioInit(led_gpio, &led_stgpio);
    StGpioConfig(led_gpio);

    // Single FreeRTOS timer
    frt_timer_init(&time, &frt, 100);

    // UART1
    RCC->AHB2ENR |= RCC_AHB2ENR_GPIOBEN;

    StGpioInit(&st_usart.rx, &uart_io1);
    StGpioInit(&st_usart.tx, &uart_io2);

    RCC->APB2ENR |= RCC_APB2ENR_USART1EN;

    NVIC_SetPriorityGrouping(0);
    NVIC_SetPriority(USART1_IRQn, NVIC_EncodePriority(0, 6, 0));
    NVIC_EnableIRQ(USART1_IRQn);

    StUsartInit(usart, &st_usart, USART1_BASE, &time);
    StUsartConfig(usart, SystemCoreClock, 115200);
    // SPI1
    RCC_PeriphCLKInitTypeDef PeriphClkInitStruct = {0};
    PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_SPI1;
    PeriphClkInitStruct.Spi1ClockSelection = RCC_SPI1CLKSOURCE_PLL1Q;
    HAL_StatusTypeDef stat = HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct);
    if (stat != HAL_OK)
    {
        Error_Handler();
    }

    RCC->AHB2ENR |= RCC_AHB2ENR_GPIOCEN;
    RCC->APB2ENR |= RCC_APB2ENR_SPI1EN;

    StGpioInit(&st_spi.scl, &spi_io1);
    StGpioInit(&st_spi.miso, &spi_io2);
    StGpioInit(&st_spi.mosi, &spi_io3);
    StGpioInit(st_spi_cs.pin, &spi_cs_io);

    GpioCsInit(spi->cs, &st_spi_cs, st_spi_cs.pin, true);
    GpioCsConfig(spi->cs);
    StSpiInit(spi, &st_spi, SPI1_BASE, 100);
    StSpiConfig(spi);

    // I2c1 PC8, PC9
    RCC->AHB2ENR |= RCC_AHB2ENR_GPIOCEN;

    StGpioInit(&st_i2c.scl, &i2c1_io1);
    StGpioInit(&st_i2c.sda, &i2c1_io2);

    RCC->APB1LENR |= RCC_APB1LENR_I2C1EN;

    StI2cInit(temp_i2c, &st_i2c, I2C1_BASE, &time);
    StI2cConfig(temp_i2c, 0x300F38);
}

void USART1_IRQHandler(void)
{
    cli_usart_rx_callback();
}

void SystemReset(void)
{
    HAL_NVIC_SystemReset();
}

void SystemClock_Config(void)
{
    RCC_OscInitTypeDef RCC_OscInitStruct = {0};
    RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

    /** Configure the main internal regulator output voltage
     */
    __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE3);

    while (!__HAL_PWR_GET_FLAG(PWR_FLAG_VOSRDY))
    {
    }

    /** Initializes the RCC Oscillators according to the specified parameters
     * in the RCC_OscInitTypeDef structure.
     */
    RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
    RCC_OscInitStruct.HSIState = RCC_HSI_ON;
    RCC_OscInitStruct.HSIDiv = RCC_HSI_DIV1;
    RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
    RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
    RCC_OscInitStruct.PLL.PLLSource = RCC_PLL1_SOURCE_HSI;
    RCC_OscInitStruct.PLL.PLLM = 4;
    RCC_OscInitStruct.PLL.PLLN = 8;
    RCC_OscInitStruct.PLL.PLLP = 2;
    RCC_OscInitStruct.PLL.PLLQ = 2;
    RCC_OscInitStruct.PLL.PLLR = 2;
    RCC_OscInitStruct.PLL.PLLRGE = RCC_PLL1_VCIRANGE_3;
    RCC_OscInitStruct.PLL.PLLVCOSEL = RCC_PLL1_VCORANGE_WIDE;
    RCC_OscInitStruct.PLL.PLLFRACN = 0;
    if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
    {
        Error_Handler();
    }

    /** Initializes the CPU, AHB and APB buses clocks
     */
    RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK |
                                  RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2 |
                                  RCC_CLOCKTYPE_PCLK3;
    RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
    RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
    RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
    RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;
    RCC_ClkInitStruct.APB3CLKDivider = RCC_HCLK_DIV1;

    if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_3) != HAL_OK)
    {
        Error_Handler();
    }
}

void Error_Handler(void)
{
    __disable_irq();
    while (1)
    {
    }
}