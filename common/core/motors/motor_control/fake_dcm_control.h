
#pragma once

#include "fake_dcm.h"
#include "fake_enc.h"
#include "gpio.h"
#include "pwm.h"

typedef struct DCM_Control DCM_Control;

struct DCM_Control
{
    MotorRotoationCtrler* qenc;
    DCPosControl* motor;
    bool cmd_enable;
    size_t pusle_per_rev;
};

void Fake_Control_Init(DCM_Control* control, DCPosControl* motor,
                       MotorRotoationCtrler* qenc);

void config(DCM_Control* control);

void motor_update(DCM_Control* control);

int get_state();
