
#pragma once

#include <stdint.h>
#include "gpio.h"
#include "pwm.h"
#include "st_dcm.h"
#include "st_enc.h"

typedef enum
{
    IDLE = 0,
    RUNNING,
    DONE

} DCState;

typedef struct DCM_Control DCM_Control;

struct DCM_Control
{
    DCState state;

    bool cmd;
    //0 for CCW 1 for CW
    bool dir;
    float cmd_degrees;
    size_t curr_pos;
    size_t curr_enc;
    size_t prev_enc;
    size_t target_count;
    size_t pusle_per_rev;
    uint16_t diff;

    DCMotor* motor;
    QEnc* enc;
};

void DCM_Control_Init(DCM_Control* control, DCMotor* motor,
                      size_t pulse_per_rev, QEnc* enc);

void DcmControlCommand(DCM_Control* control, bool command, float degrees);

bool DcmControlUpdate(DCM_Control* control);
