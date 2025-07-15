
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

Pwm pwm_one, pwm_two;
DCMotor motor_one, motor_two;
DCM_Control control_one, control_two;
QEnc enc_one, enc_two;
Gpio brake_one, brake_two, direction_one, direction_two, gpio_pwm_one,
    gpio_pwm_two, channel_a_one, channel_a_two, channel_b_one, channel_b_two,
    led;
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

    DcmControlUpdate(&control_one);
    // usart.send(&usart, &data[0], 0);

    // HAL_Delay(100);
}

void pwm_test(void)
{
    static size_t x = 0;

    pwm_one.setDuty(&pwm_one, x);

    x = (x < 100) ? x + 1 : 0;
}

int main(void)
{

    BSP_Init_motor_one(&enc_one, &channel_a_one, &channel_b_one, &led,
                       &motor_one, &control_one, &brake_one, &direction_one,
                       &gpio_pwm_one, &pwm_one, &usart);

    // BSP_Init_motor_two(&enc_two, &channel_a_two, &channel_b_two, &motor_two,
    //                    &control_two, &brake_two, &direction_two, &gpio_pwm_two,
    //                    &pwm_two, &usart);

    control_one.dir = 1;
    control_two.dir = 1;

    DcmControlCommand(&control_one, true, 30080);

    create_main_loop(pwm_test, 10);

    // StDcmSetDir(&motor, 0);

    // DcmControlCommand(&control, true, 100);

    // create_main_loop(dcm_test, 10);

    /* Start the scheduler to start the tasks executing. */
    vTaskStartScheduler();

    return 0;
}
