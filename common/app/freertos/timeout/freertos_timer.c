
#include "freertos_timer.h"

void frt_timer_start(void* data)
{
    FrtTimerData* timer_data = (FrtTimerData*)data;
    TickType_t now = xTaskGetTickCount();
    timer_data->end = now + timer_data->duration;
    timer_data->overflow = now > timer_data->end;
    timer_data->start = now;
}

bool frt_timer_tick(void* data)
{
    FrtTimerData* timer_data = (FrtTimerData*)data;
    TickType_t now = xTaskGetTickCount();
    if (timer_data->overflow)
    {
        return (now > timer_data->start) || (now < timer_data->end);
    }
    else
    {
        return now < timer_data->end;
    }
}

void frt_timer_init(Timeout* timer, FrtTimerData* frt, TickType_t duration_ms)
{
    frt->duration = duration_ms;
    frt->overflow = false;
    frt->start = 0;
    frt->end = 0;

    timer->data = (void*)frt;
    timer->start = frt_timer_start;
    timer->tick = frt_timer_tick;
}

Timeout* make_frt_timer(Mem* mem, TickType_t duration_ms)
{
    Timeout* timer = ALLOC(mem, Timeout);
    FrtTimerData* frt = ALLOC(mem, FrtTimerData);

    frt_timer_init(timer, frt, duration_ms);
    return timer;
}