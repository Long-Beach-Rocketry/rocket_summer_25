
#include <stdio.h>
#include "freertos.h"
#include "main_loop.h"
#include "stm32l4xx_hal.h"
#include "task.h"

#include "servo_app_bsp.h"

#include "gpio.h"
#include "nidec_dcm.h"
#include "st_pwm.h"

/*-----------------------------------------------------------*/

Pwm pwm;
SERVOMotor servo;
Gpio gpio_pwm;
Usart usart;

int main(void)
{

    BSP_Init(&servo, &pwm, &gpio_pwm, 330);

    // DsCommandAngle(&servo, true, 180);

    StPwmDuty(&pwm, 50);

    // create_main_loop(dcm_test, 1000);

    // NidecDcmSetDir(&motor, 0);

    // DcmPidControlCommand(&control, true, 100);

    // create_main_loop(dcm_test, 10);

    /* Start the scheduler to start the tasks executing. */
    vTaskStartScheduler();

    return 0;
}
