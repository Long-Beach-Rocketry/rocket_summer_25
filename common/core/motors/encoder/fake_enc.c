#include "fake_enc.h"
#include <math.h>

bool command_rotate(MotorRotoationCtrler* controller, double degrees);
bool update(MotorRotoationCtrler* controller);

void QEnc_Init(QEnc* qenc, MotorRotoationCtrler* params, DCMotor* motor)
{
    qenc->getTicks = get_fake_ticks;
    qenc->priv = (void*)params;

    params->counter = 0;
    params->ticks_per_angle = 0;
    params->cmd = false;
    params->ppr = 100;
    params->encoder = qenc;
    params->state = idle;
    params->motor = motor;
    params->diff = 0;
}

size_t get_fake_ticks(QEnc* qenc)
{
    MotorRotoationCtrler* params = (MotorRotoationCtrler*)(qenc->priv);
    return params->counter;
}

size_t fake_increment(QEnc* qenc, size_t increment)
{
    MotorRotoationCtrler* controller = (MotorRotoationCtrler*)(qenc->priv);

    controller->counter += increment;

    if (controller->counter < controller->last_enc)
    {
        controller->counter = 0;
        controller->last_enc = 0;
        return controller->counter += increment;
    }
    else
    {
        return controller->counter;
    }
}

size_t fake_decrement(QEnc* qenc, size_t decrement)
{
    MotorRotoationCtrler* controller = (MotorRotoationCtrler*)(qenc->priv);
    if (controller->counter == 0)
    {
        controller->counter = 65535;
        controller->last_enc = 65535;
        return controller->counter -= decrement;
    }
    else
    {
        return controller->counter -= decrement;
    }
}
