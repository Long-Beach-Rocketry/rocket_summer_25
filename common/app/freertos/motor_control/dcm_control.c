#include "dcm_control.h"
#include <math.h>
#include "FreeRTOS.h"
#include "stm32l4xx_hal.h"

void DCM_Control_Init(DCM_Control* control, DCMotor* motor,
                      size_t pulse_per_rev, QEnc* enc)
{

    control->motor = motor;
    control->pusle_per_rev = pulse_per_rev;

    control->state = IDLE;
    control->target_count = 0;
    control->cmd_degrees = 0;
    control->prev_enc = 0;
    control->curr_enc = 0;
    control->curr_pos = 0;
    control->dir = 1;
    control->diff = 0;
    control->velocity = 0;
    control->count = 0;
    control->prev_count = 0;
    control->cmd = false;
    control->delta_time = 0;
    control->pid_signal = 0;
    control->target_velocity = 20;
    control->velcoity_error = 0;
    control->constant = 5;
    control->integral_constant = 1;
    control->integral_error = 0;

    control->enc = enc;
}

void DcmControlCommand(DCM_Control* control, bool command, float degrees)
{
    control->cmd = command;
    control->cmd_degrees = degrees;

    StDcmSetEnable(control->motor, command);
    control->motor->set_direction(control->motor, control->dir);
    control->motor->set_duty(control->motor, 0);
    if (command)
    {
        control->state = IDLE;
    }
    else
    {
        control->curr_enc = 0;
        control->diff = 0;
        control->prev_enc = 0;
        control->state = IDLE;
    }
}

bool DcmControlUpdate(DCM_Control* control)
{
    control->count = xTaskGetTickCount();

    control->curr_enc = StGetEncCount(control->enc);
    if (control->cmd)
    {
        switch (control->state)
        {
            case IDLE:
                control->target_count =
                    ((control->cmd_degrees / 360) *
                     ((control->cmd_degrees < 0) ? -1 : 1)) *
                    (control->pusle_per_rev);

                control->state = RUNNING;
                break;
            case RUNNING:
                int16_t delta;
                if (control->dir)
                {
                    //overflow handling
                    if (control->curr_enc >= control->prev_enc)
                        delta =
                            (int16_t)(control->curr_enc - control->prev_enc);
                    else
                        delta =
                            (int16_t)(control->curr_enc - control->prev_enc);
                    control->delta_time = control->count - control->prev_count;
                    control->velocity = (float)(delta / control->delta_time);
                    control->diff += delta;

                    //pid
                    control->velcoity_error =
                        (int16_t)(control->target_velocity - control->velocity);

                    control->integral_error +=
                        (int16_t)(control->velcoity_error *
                                  control->delta_time);

                    control->pid_signal =
                        (int16_t)(control->constant * control->velcoity_error) +
                        (int16_t)(control->integral_constant *
                                  control->integral_error);

                    if (control->pid_signal > 100)
                    {
                        control->pid_signal =
                            100;  //capping pwm freq to 100% duty cycle
                    }

                    // control->motor->priv->pwm->setFreq(
                    // control->motor->priv->pwm, control->pid_signal);
                }
                else
                {
                    if (control->curr_enc <= control->prev_enc)
                    {
                        //overflow handling
                        delta =
                            (int16_t)(control->curr_enc -
                                      control->prev_enc);  //  backward rotation
                        if (delta < 0)
                            delta *= -1;
                    }
                    else
                        delta =
                            (int16_t)((control->curr_enc - control->prev_enc));

                    if (delta > 0)
                        delta *= 1;
                    control->delta_time = control->count - control->prev_count;
                    control->velocity = delta / (control->delta_time / 1000);

                    control->diff += delta;
                }

                if (fabs(control->diff) < fabs(control->target_count))
                {
                    break;
                }
                else
                {

                    control->motor->set_duty(control->motor, 0);
                    control->state = DONE;
                    break;
                }
            case DONE:

                control->cmd = false;
                DcmControlCommand(control, control->cmd, 0);
                break;
        }

        control->prev_enc = control->curr_enc;
        control->prev_count = control->count;
        return false;
    }
    else
    {
        control->state = IDLE;
        return false;
    }
    return true;
}