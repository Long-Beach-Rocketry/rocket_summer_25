
#include "FreeRTOS.h"
#include "task.h"

#include "subscale_bsp.h"

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
Gpio led_gpio;

int main(void)
{
    volatile uint32_t c = 0;
    while (c < 1000000)
    {
        c += 1;
    }
    BSP_Init(&usart, &spi, &i2c, &led_gpio);
    SubscaleAppCreateRayne(&usart, &spi, &i2c, &led_gpio, SystemReset);
    // uint32_t count = 1000;
    // // while (1)
    // // {
    // //     count += 1;
    // //     if (count % 100000 == 1)
    // //     {
    // //         led_gpio.toggle(&led_gpio);
    // //     }
    // // }

    /* Start the scheduler to start the tasks executing. */
    vTaskStartScheduler();

    while (1);
    return 0;
}
