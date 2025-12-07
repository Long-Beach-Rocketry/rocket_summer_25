
#include "FreeRTOS.h"
#include "task.h"

#include "flight_computer_subscale_bsp.h"
#include "stm32l4xx_hal.h" /** TODO: delete later with the Hal delay */

#include "gpio.h"
#include "i2c.h"
#include "spi.h"
#include "usart.h"

#include "debug_app.h"
#include "subscale_app_rayne.h"

/*-----------------------------------------------------------*/

Usart usart;
Spi spi;
I2c i2c;
Gpio red_led;
Gpio green_led;
Gpio blue_led;

int main(void)
{
    BSP_Init(&usart, &spi, &i2c, &red_led, &green_led, &blue_led);

    // TODO: make a app bsp for so we can share app create but they pull from different things TODODODODODO

    SubscaleAppCreateRayne(&usart, &spi, &i2c, &red_led, &green_led, &blue_led,
                           SystemReset);
    ConfigIrq();

    /* Start the scheduler to start the tasks executing. */
    vTaskStartScheduler();

    while (1);
    return 0;
}
