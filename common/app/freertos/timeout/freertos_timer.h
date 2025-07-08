/**
 * FreeRTOS implementation of timer interface.
 * Times out after counting x millisecond ticks.
 * 
 * Author: Nick Fan
 */

#pragma once

#include "timeout.h"

#include "FreeRTOS.h"
#include "task.h"

#include "alloc.h"

/**
 * Data necessary for each timer instance.
 */
typedef struct
{
    TickType_t start;
    TickType_t end;
    TickType_t duration;
    bool overflow;
} FrtTimerData;

/**
 * Start function for Timeout interface.
 */
void frt_timer_start(void* data);

/**
 * Tick function for Timeout interface.
 */
bool frt_timer_tick(void* data);

/**
 * Initializes a Timeout interface as a FreeRTOS Timer, that expires
 * after a number of ticks pass. Make sure your FreeRTOS is configured
 * for 1ms tick.
 * 
 * @param timer the timeout interface to initialize.
 * @param frt the data for a FreeRTOS timer to use for the instance.
 * @param duration_ms number of milliseconds to wait.
 */
void frt_timer_init(Timeout* timer, FrtTimerData* frt, TickType_t duration_ms);

Timeout* make_frt_timer(Mem* mem, TickType_t duration_ms);
