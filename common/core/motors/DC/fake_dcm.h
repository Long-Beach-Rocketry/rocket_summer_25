#pragma once

#include <stdbool.h>
#include <stdint.h>
#include "dcm.h"
#include "pwm.h"
#include "qenc.h"

typedef enum
{
    START = 0,
    RUNNING,
    IDLE,
    OFF

} DCState;

typedef struct
{
    DCState state;
    Pwm* pwm;
    // void (*set_enable)(DCMotor* motor, bool enable);
    // bool (*set_dir)(DCMotor* motor, bool direction);
    // void (*set_duty)(DCMotor* motor, float duty);

} DCPosControl;

void FakeInit(DCMotor* motor, DCPosControl* control);
bool FakeSetEnable(DCMotor* motor, bool enable);
bool FakeSetDir(DCMotor* motor, bool direction);
void FakeSetDuty(DCMotor* motor, float duty);
