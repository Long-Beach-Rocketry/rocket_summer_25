/**
 * 
 */

#pragma once

#include "cli.h"

typedef void (*ResetFunc)(void);

void init_reset_cmd(Cli* cli, ResetFunc f);
void reset(int argc, char* argv[]);
