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