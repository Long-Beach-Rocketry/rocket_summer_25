#pragma once

#include "dcm_control.h"
#include "freertos_timer.h"
#include "prealloc.h"
#include "st_dcm.h"
#include "st_enc.h"
#include "st_gpio.h"
#include "st_gpio_fctry.h"
#include "st_i2c_fctry.h"
#include "st_pwm.h"
#include "st_usart.h"
#include "st_usart_fctry.h"
#include "stm32l476xx.h"
#include "stm32l4xx_hal.h"
#include "usart_cli.h"

#define DRIVER_MEM_SIZE 0x256

bool BSP_Init_motor_one(QEnc* enc, Gpio* channel_a, Gpio* channel_b, Gpio* led,
                        DCMotor* motor, DCM_Control* control, Gpio* brake,
                        Gpio* direction, Gpio* gpio_pwm, Pwm* pwm,
                        Usart* usart);

// bool BSP_Init_motor_two(QEnc* enc, Gpio* channel_a, Gpio* channel_b,
//                         DCMotor* motor, DCM_Control* control, Gpio* brake,
//                         Gpio* direction, Gpio* gpio_pwm, Pwm* pwm,
//                         Usart* usart);
