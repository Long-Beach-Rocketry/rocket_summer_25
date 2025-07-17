/**
 * 
 * @brief this file sets up the motor control driver for use with the encoder
 * 
 */

#pragma once

#include <stdint.h>
#include "gpio.h"
#include "main_loop.h"
#include "nidec_dcm.h"
#include "pwm.h"
#include "st_enc.h"

typedef enum
{
    IDLE = 0,
    RUNNING,
    DONE

} DCState;

/**
 * 
 * @brief this struct defines variables that are used for needed calculations
 * 
 */
typedef struct DCM_Control DCM_Control;

struct DCM_Control
{
    DCState state;

    bool cmd;
    //0 for CCW 1 for CW
    bool dir;
    float cmd_degrees;
    size_t curr_pos;
    volatile uint16_t curr_enc;
    uint16_t prev_enc;
    size_t target_count;
    float velocity;
    volatile size_t count;
    size_t prev_count;
    size_t diff;
    int16_t delta_time;
    // int16_t delta;
    int16_t target_velocity;
    int16_t velcoity_error;
    uint8_t constant;
    int16_t pid_signal;
    uint8_t integral_constant;
    int16_t integral_error;

    DCMotor* motor;
    QEnc* enc;
};

/**
 * 
 * @brief this function initializes settings for a DCM_Control object
 * 
 * @param control a pointer to the current DCM_Control instance
 * @param motor a pointer to the current DCMotor instance
 * @param enc a pointer to the current QEnc instance
 * 
 */
void DCM_PID_Control_Init(DCM_Control* control, DCMotor* motor, QEnc* enc);

/**
 * 
 * @brief this function commands the motor a certain amount of degrees
 * 
 * @param control a pointer to the current DCM_Control instance
 * @param command true to enable motor and command, false to disable motor and command
 * @param degrees desired amount of degrees to command motor to rotate
 *  
 * */
void DcmPidControlCommand(DCM_Control* control, bool command, float degrees);

/**
 * 
 * 
 * @brief this function checks the motor's status every call. it caclulates the motor's distance traveled since last commanded and disables the motor if traveled target distance
 * 
 * @param control a pointer to the current DCM_Control instance
 */
bool DcmPidControlUpdate(DCM_Control* control);
