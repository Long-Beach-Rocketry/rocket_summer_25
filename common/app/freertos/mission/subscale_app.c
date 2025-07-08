
#include "subscale_app.h"

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

static Gpio* led;

static void loop_func(void)
{
    nav.update(&nav);
    nav.tick = xTaskGetTickCount();
    logger_update(&logger);
    led->toggle(led);
}

void SubscaleAppCreate(Usart* usart, Spi* spi, I2c* i2c, Gpio* led_gpio,
                       ResetFunc reset)
{
    led = led_gpio;
    Command commands[4] = {{"Blink", blink, "Blinks LED."},
                           {"Imu", read_bno055, "Reads IMU accel/gyro."},
                           {"Baro", read_bmp390, "Reads Barometer Pressure."},
                           {"FlashId", read_w25q_id, "Reads W25q device ID."}};
    create_cli_task(&cli, usart, commands, 4);

    W25qInit(&flash, spi, 0xFFFFFF);
    Bno055_Init(&bno, i2c, BNO055_DEV_ADDR_GND);
    Bno055_Set_Mode(&bno, BNO055_IMU_MODE);
    Bmp390_Init(&bmp, i2c, BMP390_DEV_ADDR_PWR);
    Bmp390_Config(&bmp);
    Bno055_Set_Mode(&bno, BNO055_IMU_MODE);

    W25qLoggerInit(&sub, &flash_log, &flash, flash.mem_size / flash.page_size);
    BulkLoggerInit(&bulk_sub, &bulk_priv, &sub, 4, '|', data,
                   W25Q_MAX_PAGE_SIZE);
    NavSuiteV1Init(&nav, &nav_suite, &bmp, &bno);
    NavDataLogBuilderInit(&builder, &nav_builder, &nav);
    logger_init(&logger, &builder, &bulk_sub, 1, cli.comm);

    cmd_logger_init(&cli, &logger);

    init_blink(cli.comm, led_gpio);
    init_read_bno055(cli.comm, &bno);
    init_read_bmp390(cli.comm, &bmp);
    init_read_w25q_id(cli.comm, spi);
    init_reset_cmd(&cli, reset);

    create_main_loop(loop_func, 20);
}
