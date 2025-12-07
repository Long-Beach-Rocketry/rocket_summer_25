/**
 * @file uart_lbr_net.h
 * @brief lbr_net interface implementation for usart using protobuffers
 * @author TJ Malaska
 */

/*
notes from nick
the interupt will pass a complete message in and notify that previously blocked state updater should be ready
The main loop should just call funcitons based off of the strcut of states
*/
#pragma once

#include "lbr_net.h"
#include "network_interface.h"

#include <pb_decode.h>  //idk if i should be usiung this i think i should be using the pb_cmd.h or if i should be using protobuffers at all lbr net has nothing to do with protobuffers
#include <pb_encode.h>

#include "FreeRTOS.h"
#include "cmd_message.h"
#include "retry_timer.h"
#include "semphr.h"
#include "task.h"
#include "usart.h"

void usart_rx_callback();

void uart_lbr_init(uint8_t address, network_if* net_if, Usart* usart);
