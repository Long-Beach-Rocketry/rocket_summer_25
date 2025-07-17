#pragma once

#include <stdbool.h>
#include "gpio.h"
#include "pwm.h"

typedef struct SERVOMotor SERVOMotor;

struct SERVOMotor
{

    bool (*command_angle)(SERVOMotor* servo, bool command, size_t angle);

    void* priv;
};
