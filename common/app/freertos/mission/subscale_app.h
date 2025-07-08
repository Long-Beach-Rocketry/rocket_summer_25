
#pragma once

#include "cmd_logger.h"
#include "usart_cli.h"

#include "bulk_logger.h"
#include "logging.h"
#include "main_loop.h"
#include "nav_logbuilder.h"
#include "nav_suite_v1.h"
#include "w25q.h"
#include "w25q_logger.h"

#include "gpio.h"

#include "blink.h"
#include "read_bmp390.h"
#include "read_bno055.h"
#include "read_w25q_id.h"
#include "reset_cmd.h"

void SubscaleAppCreate(Usart* usart, Spi* spi, I2c* i2c, Gpio* led_gpio,
                       ResetFunc reset);
