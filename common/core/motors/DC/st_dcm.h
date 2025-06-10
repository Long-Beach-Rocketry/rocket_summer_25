

#pragma once

#include "dcm.h"
#include "gpio.h"
#include "pwm.h"
#include "qenc.h"
#include "timeout.h"

#include <stdbool.h>

typedef struct
{

    Pwm* pwm;
    Gpio* brake;
    Gpio* direction;
    bool enable;

} DCPosControl;

void StDcmInit(DCMotor* motor, DCPosControl* control, Gpio* Brake,
               Gpio* direction, Pwm* pwm);
bool StDcmSetEnable(DCMotor* motor, bool enable);
bool StDcmSetDir(DCMotor* motor, bool direction);
void StDcmSetDuty(DCMotor* motor, float duty);
