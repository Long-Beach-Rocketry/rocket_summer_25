
#include <stdio.h>
#include "freertos.h"
#include "main_loop.h"
#include "stm32l4xx_hal.h"
#include "task.h"

#include "dcm_app_bsp.h"

#include "gpio.h"
#include "st_dcm.h"
#include "st_pwm.h"

/*-----------------------------------------------------------*/

Pwm pwm;
DCMotor motor;
DCM_Control control;
QEnc enc;
Gpio brake, direction, gpio_pwm, channel_a, channel_b;
Usart usart;

void dcm_test(void)
{
    // static const uint8_t data[] = "hi";
    // if (control.diff <= control.target_count)
    //     DcmControlUpdate(&control);
    // else
    // {
    //     // DCM_Control_Init(&control, &motor, 100);
    //     DcmControlCommand(&control, true, 300);
    //     control.dir = 0;
    // }

    DcmControlUpdate(&control);
    // usart.send(&usart, &data[0], 0);

    // HAL_Delay(100);
}

int main(void)
{

    BSP_Init(&enc, &channel_a, &channel_b, &motor, &control, &brake, &direction,
             &gpio_pwm, &pwm, &usart);

    DcmControlCommand(&control, true, 20000);

    create_main_loop(dcm_test, 10);

    // StDcmSetDir(&motor, 0);

    // DcmControlCommand(&control, true, 100);

    // create_main_loop(dcm_test, 10);

    /* Start the scheduler to start the tasks executing. */
    vTaskStartScheduler();

    return 0;
}
