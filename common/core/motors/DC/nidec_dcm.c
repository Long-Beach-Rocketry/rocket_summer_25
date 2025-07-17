#include "nidec_dcm.h"

void NidecDcmInit(DCMotor* motor, DCPosControl* st_motor, Gpio* brake,
                  Gpio* direction, Pwm* pwm)
{
    motor->set_en = NidecDcmSetEnable;
    motor->set_duty = NidecDcmSetDuty;
    motor->set_direction = NidecDcmSetDir;
    motor->priv = (void*)st_motor;

    st_motor->brake = brake;
    st_motor->direction = direction;
    st_motor->pulses_per_rev = 400;

    st_motor->pwm = pwm;

    NidecDcmSetEnable(motor, false);
}

//when enable is true brake is off, when enable is false brake is on
bool NidecDcmSetEnable(DCMotor* motor, bool enable)
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

bool NidecDcmSetDir(DCMotor* motor, bool direction)
{
    DCPosControl* dev = (DCPosControl*)motor->priv;

    if (NidecDcmSetEnable(motor, dev->enable))
    {
        dev->direction->set(dev->direction, direction);
        return true;
    }
    else
    {
        return false;
    }
}

void NidecDcmSetDuty(DCMotor* motor, float duty)
{
    DCPosControl* dev = (DCPosControl*)motor->priv;
    dev->pwm->setDuty(dev->pwm, (100 - duty));  //inverted duty cycle
}
