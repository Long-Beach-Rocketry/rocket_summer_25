/**
 * @file cmd_message.h
 * @brief interface for serialized structs.
 * @author TJ Malaska
 */

#pragma once
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

typedef struct cmd_message cmd_message;
/**
 * @brief serialize deserializes structs.
 * cmd_message is the interface to serialize and deserialize structs for lbrnet.
 * @var priv holds specific information to the deserializaiton method.
 */
struct cmd_message
{
    bool (*get_buf)(cmd_message* self, uint8_t* buffer, size_t size);
    size_t (*get_size)(cmd_message* self);
    bool (*parse)(cmd_message* self, uint8_t* buffer, size_t size);
    void* priv;
};

/**
 * @brief serializes the pb struct and puts it into a buffer.
 * @param buffer pointer to output buffer.
 * @param size size of output buffer.
 * @return true if function successfully executed.
 */
bool get_buf(cmd_message* self, uint8_t* buffer, size_t size);

/**
 * @brief returns the size of the serialized struct. 
 */
size_t get_size(cmd_message* self);

/**
 * @brief reads a buffer and converts this to a pb and stores it as a struct in the priv data.
 * @param self A pointer the current struct.
 * @param buffer is the input to what is parsed.
 * @param size is the size of the data itself not the data allocated.
 */
bool parse(cmd_message* self, uint8_t* buffer, size_t size);
