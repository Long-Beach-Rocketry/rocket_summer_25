
#include "FreeRTOS.h"
#include "task.h"

#include "usart_hw_bsp.h"

#include "gpio.h"
#include "usart.h"

/*-----------------------------------------------------------*/

Usart cli_usart;
Usart comm_usart;
Gpio led_gpio;

int main(void)
{
	
    BSP_Init(&cli_usart, &comm_usart, &led_gpio);

    /* Start the scheduler to start the tasks executing. */
    vTaskStartScheduler();

    while (1);

    return 0;
}
