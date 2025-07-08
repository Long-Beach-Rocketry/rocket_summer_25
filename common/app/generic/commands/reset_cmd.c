
#include "reset_cmd.h"

static ResetFunc func;

static const Command reset_cmd = {"Reset", reset, "Performs a system reset."};

void init_reset_cmd(Cli* cli, ResetFunc f)
{
    func = f;
    cli_register_command(cli, &reset_cmd);
}

void reset(int argc, char* argv[])
{
    func();
}