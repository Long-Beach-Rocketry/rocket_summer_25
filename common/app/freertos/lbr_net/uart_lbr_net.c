#include "uart_lbr_net.h"

static Bus bus;
static Usart* comm;
static SemaphoreHandle_t task_semaphore = NULL;
static const pb_msgdesc_t* self_pb;  // idk if i need this
StaticSemaphore_t xSemaphoreBuffer;

bool send(uint8_t target_address, cmd_message* cmd_message)
{
    uint8_t message_buffer[MAX_RECEIVE_BUF_SIZE];
    uint8_t data[MAX_RECEIVE_BUF_SIZE];
    uint8_t pb_size = cmd_message->get_size(cmd_message);
    cmd_message->get_buf(cmd_message, data, MAX_RECEIVE_BUF_SIZE);
    bus.pack(&bus, message_buffer, MAX_RECEIVE_BUF_SIZE, target_address, data,
             pb_size);
    comm->send(comm, message_buffer, pb_size + PACKET_HEADER_SIZE);
    return true;
}

void lbr_net_process_task()
{
    const TickType_t max_block_time = pdMS_TO_TICKS(UINT32_MAX);
    task_semaphore = xSemaphoreCreateBinaryStatic(&xSemaphoreBuffer);
    while (1)
    {
        if (xSemaphoreTake(task_semaphore, max_block_time) == pdTRUE)
        {
        }
    }
}

void usart_rx_callback()
{
    uint8_t data = 0;
    if (comm->recv(comm, &data, 1))
    {
        BaseType_t higher_prio_task_woken = pdFALSE;
        bus.read_byte(&bus, data);
        higher_prio_task_woken = pdFALSE;
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

bool receive(uint8_t* buff, size_t size)
{
}

void uart_lbr_init(uint8_t address, network_if* network_if, Usart* usart)
{
    lbr_net_node_init(&bus, address);
    comm = usart;
    network_if->receive = &receive;
    network_if->send = &send;
}
