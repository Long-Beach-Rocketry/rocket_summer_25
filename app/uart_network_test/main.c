
#include "FreeRTOS.h"

#include "main_loop.h"
#include "uart_lbr_net.h"
#include "uart_network_bsp.h"
//#include "uart_network_test_app.h"

#define ADDRESS 66
#define RECEIVER_ADDRESS 41
Usart usart;

uint8_t data = 1;
size_t data_size = sizeof(data_size);
network_if interface;

static void loop_func(void)
{
    static size_t package_size;
    static uint8_t package_buffer[MAX_RECEIVE_BUF_SIZE];

    usart.send(&usart, (uint8_t*)"start\n", 6);
    interface.receive(package_buffer, &package_size);
    usart.send(&usart, (uint8_t*)"\n", 1);
    interface.send(RECEIVER_ADDRESS, &data, data_size);
    usart.send(&usart, (uint8_t*)"\n", 1);
}

void NetworkUartTest(Usart* usart)
{
    uart_lbr_init(ADDRESS, &interface, usart);
    create_main_loop(loop_func, 1);
}

int main(void)
{
    BSP_Init(&usart);
    NetworkUartTest(&usart);
    usart.send(&usart, (uint8_t*)"h", 1);
    /* Start the scheduler to start the tasks executing. */
    vTaskStartScheduler();
    return 0;
}
