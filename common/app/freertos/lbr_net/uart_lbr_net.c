/**
 * @file usart_lbr_net.c
 * @brief 
 * @author TJ Malaska
 */
#include "uart_lbr_net.h"

static Bus bus;
static Usart* comm;
static SemaphoreHandle_t task_semaphore = NULL;
const TickType_t max_block_time = pdMS_TO_TICKS(UINT32_MAX);
StaticSemaphore_t xSemaphoreBuffer;

void uart_lbr_init(uint8_t address, network_if* network_if, Usart* usart)
{

    task_semaphore = xSemaphoreCreateBinaryStatic(&xSemaphoreBuffer);
    lbr_net_node_init(&bus, address);
    comm = usart;
    network_if->receive = receive;
    network_if->send = send;
}

/* sends parcked data over usart */
bool send(uint8_t target_address, uint8_t* data, size_t size)
{
    uint8_t send_buffer[MAX_RECEIVE_BUF_SIZE];
    bus.pack(&bus, send_buffer, MAX_RECEIVE_BUF_SIZE, target_address, data,
             size);
    comm->send(comm, (uint8_t*)"h", 1);  //remove later?
    comm->send(comm, send_buffer, size + PACKET_HEADER_SIZE);
    return true;
}

/* When semaphore given flush. */
bool receive(uint8_t* data_out, size_t* size_out)
{
    if (xSemaphoreTake(task_semaphore, max_block_time) == pdTRUE)
    {
        if (bus.state != FINISHED)
        {
            return false;
        }
        bus.flush_data(&bus, data_out);
        *size_out = bus.package_size;
        return true;
    }
    return false;
}
/* Reads feeds data into the bus and if finished give a semaphore. */
void usart_rx_callback()
{
    uint8_t data = 0;
    if (comm->recv(comm, &data, 1))
    {
        BaseType_t higher_prio_task_woken = pdFALSE;
        bus.read_byte(&bus, data);
        if (bus.state == FINISHED)
        {
            if (task_semaphore == NULL)
            {
                return;
            }
            else
            {
                xSemaphoreGiveFromISR(task_semaphore, &higher_prio_task_woken);
            }
        }
        portYIELD_FROM_ISR(higher_prio_task_woken);
    }
}
