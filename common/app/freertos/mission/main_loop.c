
#include "main_loop.h"

static StackType_t main_loop_stack[MAIN_LOOP_STACK_SIZE_BYTES];
static StaticTask_t main_loop_buffer;

static TaskHandle_t main_loop;

void (*initialize)(void);
void (*callback)(void);
TickType_t target_loop_ms;

static void loop_func(void* params)
{
    initialize();
    while (1)
    {
        TickType_t start = xTaskGetTickCount();

        callback();

        TickType_t time = xTaskGetTickCount() - start;
        if (time < target_loop_ms)
        {
            vTaskDelay(target_loop_ms - time);
        }
    }
}

size_t create_main_loop(void (*init)(void), void (*func)(void),
                        size_t frequency_hz)
{
    initialize = init;
    callback = func;
    main_loop =
        xTaskCreateStatic(loop_func, "Main Loop", MAIN_LOOP_STACK_SIZE_BYTES,
                          NULL, 1, main_loop_stack, &main_loop_buffer);
    return set_frequency(frequency_hz);
}

size_t set_frequency(size_t frequency_hz)
{
    frequency_hz = (frequency_hz > 1000) ? 1000 : frequency_hz;
    target_loop_ms = 1000 / frequency_hz;
    return target_loop_ms;
}