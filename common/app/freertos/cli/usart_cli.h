/**
 * Usart example CLI.
 * 
 * Author: Nick Fan
 */

#pragma once

#include "FreeRTOS.h"
#include "task.h"

#include "retry_timer.h"
#include "ring_buffer.h"
#include "usart.h"

#include "cli.h"

#define CLI_VER "usart-cli-v0.1"
#define PASSWORD "LBR"
#define MAX_SEND_LEN 300
#define CLI_TERMINATION_CHAR '\n'

/**
 * Usart RX Ready callback, to be called in 
 * USARTx_IRQHandler().
 */
void cli_usart_rx_callback();

/**
 * Creates a FreeRTOS CLI processing task using a given USART/core
 * clock frequency. A list of commands can be registered to add
 * functionality to the command line.
 * 
 * @param cmdline the command line instance to initialize and use.
 * @param cli_usart the usart driver to use.
 * @param commands a list of commands to register.
 * @param num_commands the size of the command list.
 * 
 * @returns True if all commands are registered and the task is created,
 *          otherwise false.
 */
bool create_cli_task(Cli* cmdline, Usart* cli_usart, Command* commands,
                     size_t num_commands);
