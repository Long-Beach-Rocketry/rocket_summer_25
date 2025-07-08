#pragma once
#include <pb_decode.h>
#include <pb_encode.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include "cmd_message.h"

/**
 * @brief the priv data for cmd_message for a pb. Has the struct. This struct will be inited in the app
 */
typedef struct pb_cmd pb_cmd;
struct pb_cmd
{
    size_t buffer_size;
    const pb_msgdesc_t* schema_field;
    void* schema_object;
    //no more mutex
};