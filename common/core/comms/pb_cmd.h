/**
 * @file cmd_message.h
 * @brief private data for cmd_message.h specific to nanopb.
 * @author TJ Malaska
 */
#pragma once
#include <pb_decode.h>
#include <pb_encode.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include "cmd_message.h"

typedef struct pb_cmd pb_cmd;
/**
 * @struct pb_cmd
 * @brief Priavte data for cmd_message.h for a protobuffers.
 * This struct holds the struct.
 * This struct will be inited in the app.
 *
 */
struct pb_cmd
{
    size_t buffer_size;
    const pb_msgdesc_t* schema_field;
    void* schema_object;
};
