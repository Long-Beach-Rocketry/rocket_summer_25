#include "st_dcm.h"

void StDcmInit(DCMotor* motor, DCPosControl* control, Gpio* brake,
               Gpio* direction, Pwm* pwm)
{
    motor->set_en = StDcmSetEnable;
    motor->set_duty = StDcmSetDuty;
    motor->set_direction = StDcmSetDir;
    motor->priv = (void*)control;

    control->brake = brake;
    control->direction = direction;

    control->pwm = pwm;

    StDcmSetEnable(motor, false);
}

//when enable is true brake is off, when enable is false brake is on
bool StDcmSetEnable(DCMotor* motor, bool enable)
{
    DCPosControl* dev = (DCPosControl*)motor->priv;

    dev->brake->set(dev->brake, enable);
    dev->pwm->enable(dev->pwm, enable);
    dev->enable = enable;
    if (enable)
    {
        return true;
    }
    else
    {
        return false;
    }
}

bool StDcmSetDir(DCMotor* motor, bool direction)
{
    DCPosControl* dev = (DCPosControl*)motor->priv;

    if (StDcmSetEnable(motor, dev->enable))
    {
        dev->direction->set(dev->direction, direction);
        return true;
    }
    else
    {
        return false;
    }
}

void StDcmSetDuty(DCMotor* motor, float duty)
{
    DCPosControl* dev = (DCPosControl*)motor->priv;
    dev->pwm->setDuty(dev->pwm, (100 - duty));  //inverted duty cycle
}
