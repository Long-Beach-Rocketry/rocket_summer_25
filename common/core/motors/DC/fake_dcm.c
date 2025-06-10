#include "fake_dcm.h"

void FakeInit(DCMotor* motor, DCPosControl* control)
{
    motor->set_en = FakeSetEnable;
    motor->set_duty = FakeSetDuty;
    motor->set_direction = FakeSetDir;
    motor->priv = (void*)control;

    control->state = OFF;
    // control->enabled = true;
    // control->disabled = false;
    // control->direction_one = false;
    // control->direction_two = true;
    // control->duty = 50;

    FakeSetEnable(motor, false);
}

bool FakeSetEnable(DCMotor* motor, bool enable)
{
    DCPosControl* dev = (DCPosControl*)motor->priv;

    if (enable)
    {
        dev->state = START;
        //pwm->enable(&pwm, enable);
        return true;
    }
    else
    {
        dev->state = OFF;
        // dev->pwm->enable(dev->pwm, enable);  // causing segfaul
        return false;
    }
    // causing segfault
}

bool FakeSetDir(DCMotor* motor, bool direction)
{
    DCPosControl* dev = (DCPosControl*)motor->priv;

    if (dev->state != OFF)
    {
        if (direction)
        {
            dev->state = RUNNING;
            return true;
        }
        else
        {
            dev->state = RUNNING;
            return true;
        }
    }
    else
    {
        return false;
    }
}

void FakeSetDuty(DCMotor* motor, float duty)
{
    DCPosControl* dev = (DCPosControl*)motor->priv;
    dev->pwm->setDuty(dev->pwm, duty);
}
