#pragma once

typedef struct{
    float kp;
    float ki;
    float kd;
    float setpoint;
    float I;
    float prev_error;
    float P;
    float D;
}PID_controller;

void initialize(PID_controller* PID, float kp, float ki, float kd, float setpoint);

float update(PID_controller* PID, float current_value, float dt);