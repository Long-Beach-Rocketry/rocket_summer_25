#include "drv8242_enc.h"
#include <math.h>

bool command_rotate(MotorRotoationCtrler* controller, double degrees);
bool update(MotorRotoationCtrler* controller);

void QEnc_Init(QEnc* qenc, MotorRotoationCtrler* params)
{
    qenc->getTicks = get_ticks;
    qenc->priv = (void*)params;

    params->counter = 0;
    params->ticks_per_angle = 0;
    params->command_rotate = command_rotate;
    params->update = update;
    params->cmd = false;
    params->ppr = 100;
}

size_t get_ticks(QEnc* qenc)
{
    MotorRotoationCtrler* params = (MotorRotoationCtrler*)(qenc->priv);
    return params->counter;
}

size_t increment(QEnc* qenc, size_t increment)
{
    MotorRotoationCtrler* controller = (MotorRotoationCtrler*)(qenc->priv);
    if (fabs(controller->diff) > 65535)
    {
        controller->counter = 0;
        return controller->counter;
    }
    else
    {
        return controller->counter += increment;
    }
}

size_t decrement(QEnc* qenc, size_t decrement)
{
    MotorRotoationCtrler* controller = (MotorRotoationCtrler*)(qenc->priv);
    if (fabs(controller->diff) > 65535)
    {
        controller->counter = 0;
        return controller->counter;
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
        controller->start_pos =
            controller->encoder->getTicks(controller->encoder);
        controller->cmd = true;
        return true;
    }
}

bool update(MotorRotoationCtrler* controller)
{
    size_t curr_enc = controller->encoder->getTicks(controller->encoder);
    switch (controller->state)
    {
        case idle:
            if (controller->cmd)
            {
                controller->motor->set_direction(controller->motor,
                                                 controller->dir);
                controller->motor->set_en(controller->motor, true);
                controller->state = rotating;
                return true;
            }
            break;
        case rotating:
            if (controller->cmd)
            {
                controller->diff += curr_enc - controller->last_enc;
                if (fabs(controller->diff) < fabs(controller->ticks_needed))
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
    return false;
}