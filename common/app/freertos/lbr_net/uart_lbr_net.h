/*
notes from nick
the interupt will pass a complete message in and notify that previously blocked state updater should be ready
The main loop should just call funcitons based off of the strcut of states
*/
#pragma once

#include "lbr_net.h"
#include "network_interface.h"

#include <pb_decode.h>
#include <pb_encode.h>

#include "FreeRTOS.h"
#include "cmd_message.h"
#include "retry_timer.h"
#include "semphr.h"
#include "task.h"
#include "usart.h"

void usart_rx_callback();

void uart_lbr_init(uint8_t address, network_if* net_if, Usart* usart);

void lbr_net_process_task();