/**
 * @file network_interface.h
 * @brief interface to use lbr_net across differnt platforms
 * @author TJ Malaska
 */
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include "cmd_message.h"
typedef struct network_if network_if;
struct network_if
{
    bool (*send)(uint8_t target_address, uint8_t* data, size_t size);
    bool (*receive)(uint8_t* data_out, size_t* size_out);
};
