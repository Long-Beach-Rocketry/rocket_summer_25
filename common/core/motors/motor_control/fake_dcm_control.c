#include "fake_dcm_control.h"

void Fake_Control_Init(DCM_Control* control, DCPosControl* motor,
                       MotorRotoationCtrler* qenc)
{
    control->motor = motor;
    control->qenc = qenc;
    control->cmd_enable = false;
}

void config(DCM_Control* control)
{
    switch (control->motor->state)
    {
        case START:
            if (!control->qenc->cmd)
            {
                control->qenc->cmd = true;
            }
            break;

        default:
            break;
    }
}

void motor_update(DCM_Control* control)
{
    control->qenc->update(control->qenc);

    if (control->qenc->cmd)
    {
        control->cmd_enable = true;
    }
    else
    {
        control->cmd_enable = false;
    }
}

int get_state(DCM_Control* control)
{
    return control->qenc->state;
}