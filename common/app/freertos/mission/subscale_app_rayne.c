
#include "subscale_app_rayne.h"
//needs to be moved and its own
static Cli cli;

static W25q flash;
static Bno055 bno;
static Bmp390 bmp;

static Logger logger;
static LogBuilder builder;
static LogSubscriber sub;
LogSubscriber bulk_sub;
BulkLogger bulk_priv;
uint8_t data[W25Q_MAX_PAGE_SIZE];
static W25qLogger flash_log;
static NavDataLogBuilder nav_builder;
static NavData nav;
static NavSuiteV1 nav_suite;

static Gpio* red_led;
static Gpio* green_led;
static Gpio* blue_led;

static void loop_func(void)
{
    if (logger.enabled)
    {
        nav.update(&nav);
        red_led->toggle(red_led);
    }
    nav.tick = xTaskGetTickCount();
    logger_update(&logger);
}

static void init(void)
{
    vTaskDelay(10);
    Bno055_Set_Mode(&bno, BNO055_IMU_MODE);
    Bmp390_Config(&bmp);
    blue_led->set(blue_led, 1);  //turn off both leds
    green_led->set(green_led, 1);
}

void SubscaleAppCreateRayne(Usart* usart, Spi* spi, I2c* i2c, Gpio* red_gpio,
                            Gpio* green_gpio, Gpio* blue_gpio, ResetFunc reset)
{
    red_led = red_gpio;
    green_led = green_gpio;
    blue_led = blue_gpio;
    Command commands[4] = {{"Blink", blink, "Blinks LED."},
                           {"Imu", read_bno055, "Reads IMU accel/gyro."},
                           {"Baro", read_bmp390, "Reads Barometer Pressure."},
                           {"FlashId", read_w25q_id, "Reads W25q device ID."}};
    create_cli_task(&cli, usart, commands, 4);

    W25qInit(&flash, spi, 0xFFFFFF);
    Bno055_Init(&bno, i2c, BNO055_DEV_ADDR_GND);
    Bmp390_Init(&bmp, i2c, BMP390_DEV_ADDR_GND);

    // Bno055_Set_Mode(&bno, BNO055_IMU_MODE);  //why is this here twice

    W25qLoggerInit(&sub, &flash_log, &flash, flash.mem_size / flash.page_size);
    BulkLoggerInit(&bulk_sub, &bulk_priv, &sub, 4, '|', data,
                   W25Q_MAX_PAGE_SIZE);
    NavSuiteV1Init(&nav, &nav_suite, &bmp, &bno);
    NavDataLogBuilderInit(&builder, &nav_builder, &nav);
    logger_init(&logger, &builder, &bulk_sub, 1, cli.comm);

    cmd_logger_init(&cli, &logger);

    init_blink(cli.comm, red_gpio);
    init_read_bno055(cli.comm, &bno);
    init_read_bmp390(cli.comm, &bmp);
    init_read_w25q_id(cli.comm, spi);
    init_reset_cmd(&cli, reset);

    create_main_loop(init, loop_func, 20);
}
