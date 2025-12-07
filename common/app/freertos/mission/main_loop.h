

#pragma once

#include "FreeRTOS.h"
#include "task.h"

#define MAIN_LOOP_STACK_SIZE_BYTES 0x1000

size_t create_main_loop(void (*init)(void), void (*func)(void),
                        size_t frequency_hz);
size_t set_frequency(size_t frequency_hz);