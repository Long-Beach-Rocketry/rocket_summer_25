/**
 * 
 */

#pragma once

#include "cli.h"
#include "pwm.h"

bool init_pwm_ctl(Cli* cli, Pwm* pwm);
void pwm_ctl_duty(int argc, char* argv[]);
void pwm_ctl_freq(int argc, char* argv[]);
