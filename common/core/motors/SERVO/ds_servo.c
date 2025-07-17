#include "ds_servo.h"
#include "st_pwm.h"

void DsServoInit(DSServo* ds_servo, SERVOMotor* servo, Pwm* pwm,
                 size_t pwm_freq)
{

    StPrivPwm* st_pwm = (StPrivPwm*)ds_servo->pwm->priv;
    ds_servo->pwm = pwm;
    ds_servo->cmd = false;
    ds_servo->angle = 0;
    ds_servo->pwm->setFreq(ds_servo->pwm, pwm_freq);
    ds_servo->counter_value = st_pwm->instance->ARR;
    ds_servo->period_ms = 1000 / pwm_freq;
    ds_servo->max_angle = 180;

    servo->priv = (void*)ds_servo;
    servo->command_angle = DsCommandAngle;
}

bool DsCommandAngle(SERVOMotor* servo, bool command, size_t angle)
{
    DSServo* dev = (DSServo*)servo->priv;

    float max_ms = 2.5 / dev->period_ms;
    float min_ms = 0.5 / dev->period_ms;
    float max_period = max_ms * dev->counter_value;
    float min_period = min_ms * dev->counter_value;

    float diff_period = (max_period - min_period) / dev->max_angle;

    float target_period = (diff_period * angle) + min_period;

    float target_duty = target_period / dev->counter_value;

    if (!dev->cmd)
    {
        dev->cmd = command;
        dev->angle = angle;
        dev->pwm->enable(dev->pwm, command);
        dev->pwm->setDuty(dev->pwm, 50);
    }
    else
    {
        dev->cmd = false;
        dev->angle = 0;
        dev->pwm->enable(dev->pwm, command);
        dev->pwm->setDuty(dev->pwm, 0);
    }

    return true;
}