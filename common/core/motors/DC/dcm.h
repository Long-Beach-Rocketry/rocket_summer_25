#pragma once

#include <stdbool.h>
#include "gpio.h"
#include "pwm.h"

typedef struct DCMotor DCMotor;

struct DCMotor
{

    bool (*set_en)(DCMotor* motor, bool enable);
    bool (*set_direction)(DCMotor* motor, bool direction);
    void (*set_duty)(DCMotor* motor, float duty);

    void* priv;
};
