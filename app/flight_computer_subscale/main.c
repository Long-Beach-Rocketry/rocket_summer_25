
#include "FreeRTOS.h"
#include "task.h"

#include "flight_computer_subscale_bsp.h"

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

    volatile uint32_t c = 0;
    while (c < 5000000)
    {
        c += 1;
    }

    //HAL_Delay(500);  //bc we thing the cap is too big

    SubscaleAppCreateRayne(&usart, &spi, &i2c, &red_led, SystemReset);
    ConfigIrq();
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
