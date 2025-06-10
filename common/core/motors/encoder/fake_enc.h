#pragma once

#include "dcm.h"
#include "pwm.h"
#include "qenc.h"

typedef enum
{
    idle = 0,
    rotating,

} QEncState;

typedef struct MotorRotoationCtrler MotorRotoationCtrler;

struct MotorRotoationCtrler
{
    QEnc* encoder;
    DCMotor* motor;
    Pwm* pwm;
    QEncState state;
    int ticks_needed;
    size_t ppr;
    size_t last_enc;
    uint16_t counter;
    int diff;
    size_t ticks_per_angle;
    size_t start_pos;
    bool cmd;
    bool dir;

    bool (*command_rotate)(MotorRotoationCtrler* controller, double degrees);
    bool (*update)(MotorRotoationCtrler* controller);
};

void QEnc_Init(QEnc* qenc, MotorRotoationCtrler* params, DCMotor* motor);
size_t get_fake_ticks(QEnc* qenc);
size_t fake_increment(QEnc* qenc, size_t increment);
size_t fake_decrement(QEnc* qenc, size_t decrement);
