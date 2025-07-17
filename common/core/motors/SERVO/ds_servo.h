#pragma once

#include <stdint.h>
#include "servo.h"

typedef struct
{
    size_t angle;
    bool cmd;
    Pwm* pwm;
    size_t counter_value;
    size_t period_ms;
    size_t max_angle;
} DSServo;

void DsServoInit(DSServo* ds_servo, SERVOMotor* servo, Pwm* pwm,
                 size_t pwm_freq);

bool DsCommandAngle(SERVOMotor* servo, bool command, size_t angle);
