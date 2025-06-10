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
    params->command_rotate = command_rotate;
    params->update = update;
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

bool command_rotate(MotorRotoationCtrler* controller, double degrees)
{
    if (!controller->cmd || degrees == 0)
    {
        return false;
    }
    else
    {
        controller->ticks_per_angle = (int)(controller->ppr / 90);
        controller->ticks_needed = (controller->ticks_per_angle) * (degrees);
        controller->dir = degrees > 0;
        controller->start_pos = get_fake_ticks(controller->encoder);  //segfault
        controller->diff = 0;
        controller->cmd = true;
        return true;
    }
}

bool update(MotorRotoationCtrler* controller)
{
    size_t curr_enc = get_fake_ticks(controller->encoder);
    switch (controller->state)
    {
        case idle:
            if (controller->cmd)
            {
                controller->motor->set_direction(controller->motor,
                                                 controller->dir);
                controller->motor->set_en(controller->motor, true);
                controller->state = rotating;
                //return true;
            }
            break;
        case rotating:
            if (controller->cmd)
            {
                controller->diff += (curr_enc - controller->last_enc);
                if (fabs(controller->diff) < fabs(controller->ticks_needed))
                {

                    //return true;
                }
                else
                {
                    controller->motor->set_en(controller->motor, false);
                    controller->state = idle;
                    controller->cmd = false;
                    return false;
                }
            }
            break;
    }
    controller->last_enc = curr_enc;
    return true;
}