
#include "FreeRTOS.h"
#include "task.h"
#include "usart_cli.h"

#include "pwm_app_bsp.h"
#include "pwm_ctl.h"
#include "reset_cmd.h"

#include "gpio.h"
#include "pwm.h"

/*-----------------------------------------------------------*/

Cli cli;
Usart usart;
Pwm pwm;
Gpio led_gpio;

int main(void)
{
    BSP_Init(&usart, &pwm, &led_gpio);
    create_cli_task(&cli, &usart, NULL, 0);
    init_pwm_ctl(&cli, &pwm);

    /* Start the scheduler to start the tasks executing. */
    vTaskStartScheduler();

    return 0;
}
