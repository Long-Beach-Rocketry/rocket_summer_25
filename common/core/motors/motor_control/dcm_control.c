#include "dcm_control.h"
#include <math.h>
#include "stm32l4xx_hal.h"
#include "pid.h"

void initialize(PID_controller* PID, float kp, float ki, float kd, float setpoint){
    PID->kp = kp;
    PID->ki = ki;
    PID->kd = kd;
    PID->P = 0;
    PID->setpoint = setpoint;
    PID->I = 0;
    PID->D = 0;
    PID->prev_error = 0;
}

float update(PID_controller* PID, float current_value, float dt){
    /*Note: P is the error*/
    PID->P = PID->setpoint - current_value;
    PID->I += PID->P * dt;
    PID->D = (PID->P - PID->prev_error) / dt;
    PID->prev_error = PID->P;
    float equation = (PID->kp * PID->P) + (PID->ki * PID->I) + (PID->kd * PID->D);
    return equation;
}

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
    control->cmd = false;

    control->enc = enc;
}

void DcmControlCommand(DCM_Control* control, bool command, float degrees)
{
    control->cmd = command;
    control->cmd_degrees = degrees;
    StDcmSetEnable(control->motor, command);
    if (command)
    {
        control->state = IDLE;
    }
    // else
    // {
    //     control->curr_pos = 0;
    //     control->diff = 0;
    //     control->prev_pos = 0;
    //     control->state = CCW;
    // }
}

bool DcmControlUpdate(DCM_Control* control)
{
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

                // control->curr_pos = 0;
                control->motor->set_direction(control->motor, control->dir);
                control->motor->set_duty(control->motor, 10);  //10% duty cycle
                control->state = RUNNING;
                break;
            case RUNNING:
                PID = initialize(0.1, 0.01, 0.01, control->cmd_degrees);
                size_t delta;
                if (control->curr_enc >= control->prev_enc)
                    delta = control->curr_enc -
                            control->prev_enc;  // normal forward rotation
                else
                    delta = (uint16_t)(65536 + control->curr_enc -
                                       control->prev_enc);
                control->diff += delta;
                if (fabs(control->diff) < fabs(control->target_count))
                {
                }
                else
                {
                    control->cmd = false;
                    DcmControlCommand(control, control->cmd, 0);
                    control->motor->set_duty(control->motor, 0);
                    // HAL_Delay(500);
                    // control->cmd = true;
                    // DcmControlCommand(control, control->cmd, 100);
                    // control->motor->set_direction(control->motor, 0);
                    control->state = DONE;
                    // control->state = CCW;
                    break;
                }
                control->cmd_degrees = update(PID*, control->cmd_degrees, delta);
            // case CCW:
            //     control->diff += (control->curr_pos - control->prev_pos);
            //     if (fabs(control->diff) < fabs(control->target_count))
            //     {
            //     }
            //     else
            //     {
            //         control->cmd = false;
            //         DcmControlCommand(control, control->cmd, 0);
            //         control->motor->set_duty(control->motor, 0);
            //         control->state = DONE;
            //     }
            case DONE:
                if (!control->cmd)
                {
                    control->curr_enc = 0;
                    control->diff = 0;
                    control->prev_enc = 0;
                    control->state = IDLE;
                }
                break;
        }

        control->prev_enc = control->curr_enc;
    }
    else
    {
        control->state = IDLE;
        return false;
    }
    return true;
}