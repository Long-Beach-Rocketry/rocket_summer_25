/**
 * Needed for static allocation.
 */

#ifdef __cplusplus
extern "C"
{
#endif

#include "FreeRTOS.h"
#include "task.h"

    void vApplicationGetIdleTaskMemory(StaticTask_t** ppxIdleTaskTCBBuffer,
                                       StackType_t** ppxIdleTaskStackBuffer,
                                       uint32_t* pulIdleTaskStackSize);

    /* USER CODE BEGIN GET_IDLE_TASK_MEMORY */
    static StaticTask_t xIdleTaskTCBBuffer;
    static StackType_t xIdleStack[configMINIMAL_STACK_SIZE];

    void vApplicationGetIdleTaskMemory(StaticTask_t** ppxIdleTaskTCBBuffer,
                                       StackType_t** ppxIdleTaskStackBuffer,
                                       uint32_t* pulIdleTaskStackSize)
    {
        *ppxIdleTaskTCBBuffer = &xIdleTaskTCBBuffer;
        *ppxIdleTaskStackBuffer = &xIdleStack[0];
        *pulIdleTaskStackSize = configMINIMAL_STACK_SIZE;
        /* place for user code */
    }

#ifdef __cplusplus
}
#endif