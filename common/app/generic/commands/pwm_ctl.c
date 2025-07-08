
#include "pwm_ctl.h"

#define NUM_PWM_CTL_CMDS (2)

static Pwm* pwm_dev;
static Cli* cmd_line;
static bool pwm_state;

static const Command commands[NUM_PWM_CTL_CMDS] = {
    {"PwmDuty", pwm_ctl_duty,
     "Sets duty cycle of PWM as a percentage, i.e. [ PwmDuty 75 ] for 75 "
     "percent."},
    {"PwmFreq", pwm_ctl_freq,
     "Sets frequency of PWM in Hz, i.e. [ PwmFreq 25000 ] for 25KHz."},
};

bool init_pwm_ctl(Cli* cli, Pwm* pwm)
{
    pwm_dev = pwm;
    cmd_line = cli;
    pwm_state = false;
    for (size_t i = 0; i < NUM_PWM_CTL_CMDS; ++i)
    {
        if (!cli_register_command(cli, &commands[i]))
        {
            return false;
        }
    }

    return true;
}

void pwm_ctl_duty(int argc, char* argv[])
{
    float duty_cycle;
    sscanf(argv[1], "%f", &duty_cycle);
    if (!pwm_dev->set_duty(pwm_dev, duty_cycle))
    {
        cmd_line->comm->fwrite(cmd_line->comm, "Error setting PWM duty cycle!");
    }
    else
    {
        cmd_line->comm->fwrite(cmd_line->comm,
                               "PWM duty cycle set to %f percent.", duty_cycle);
    }
}

void pwm_ctl_freq(int argc, char* argv[])
{
    size_t freq;
    sscanf(argv[1], "%zu", &freq);
    if (!pwm_dev->set_freq(pwm_dev, freq))
    {
        cmd_line->comm->fwrite(cmd_line->comm, "Error setting PWM frequency!");
    }
    else
    {
        cmd_line->comm->fwrite(cmd_line->comm, "PWM frequency set to %zu Hz.",
                               freq);
    }
}