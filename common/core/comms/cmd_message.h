
#pragma once
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

/**
 * @brief This struct is the interface of the serializing and deserializing structs for lbrnet 
 */
typedef struct cmd_message cmd_message;
struct cmd_message
{
    bool (*get_buf)(cmd_message* self, uint8_t* buffer, size_t size);
    size_t (*get_size)(cmd_message* self);
    bool (*parse)(cmd_message* self, uint8_t* buffer, size_t size);
    //the priv rn will hold the schema information as well as the data
    void* priv;
};

/**
 * @brief serializes the pb struct and puts it into a buffer.
 * 
 * @param self A pointer the current struct.
 */
bool get_buf(cmd_message* self, uint8_t* buffer, size_t size);

/**
 * @brief returns the size of the buffer.
 * 
 * @param self A pointer the current struct.
 */
size_t get_size(cmd_message* self);

/**
 * @brief reads a buffer and converts this to a pb and stores it as a struct in the priv data.
 * 
 * @param self A pointer the current struct.
 */
bool parse(cmd_message* self, uint8_t* buffer, size_t size);
