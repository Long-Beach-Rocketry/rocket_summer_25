
/**
 * This software is a generic bus designed to serialize data over UART for board to board communication.
 * Allows for one byte addressing and checksum error checking.
 * Format for the packets one byte each:
 *      start transmission, address, datalength, data(as long as the package size), checksum   
 * Author: TJ Malaska
 */
#pragma once
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

#define MAX_RECEIVE_BUF_SIZE 1028U
#define CHECKSUM_SIZE 1
#define CHECKSUM_BITS 256
#define START_BYTE_SIZE 1
#define ADDRESS_SIZE 1
#define DATA_LEN_SIZE 1
/* The amount of bytes of the extra packing information. */
#define PACKET_HEADER_SIZE \
    (CHECKSUM_SIZE + START_BYTE_SIZE + ADDRESS_SIZE + DATA_LEN_SIZE)

/* Possible states. */
typedef enum
{
    IDLE,
    ACKNOWLEDGED,
    READ_ADDRESS,
    READ_LEN,
    READ_DATA,
    VALIDATE,
    FINISHED
} BusState;

/* 
Control flags for protocol.
*/
typedef enum
{
    START_TRANSMISSION = '!',
    //ACK = '+',
    //NACK = '-'
} ControlFlags;

/**
 * @brief This is the bus struct which can format and parse messages as well as store a state.
 */
typedef struct Bus Bus;
struct Bus
{
    /* Max data package size. */
    uint8_t package_size;
    /* bus state */
    BusState state;
    /* Current checksum. */
    uint32_t sum;
    /* Current index of the receive buffer. */
    uint8_t receive_index;
    /* The address of the bus. */
    uint8_t address;
    char receive_buffer[MAX_RECEIVE_BUF_SIZE];
    bool (*pack)(Bus* self, uint8_t* buffer, uint16_t buffer_size,
                 uint8_t target, uint8_t* data, uint8_t data_size);
    void (*read_byte)(Bus* self, uint8_t data);
    uint8_t (*get_package_size)(Bus* self);
    void (*flush_data)(Bus* self, uint8_t* buffer);
};

/**
 * @brief Converts data into a formatted message and clears/writes to a buffer. (change order and name buffers to input output)
 * 
 * @param self A pointer the current sending bus struct.
 * @param buffer A pointer a buffer that we will store the output in.
 * @param buffer_size The size of the output buffer.
 * @param target The address of what we want to use in the message.
 * @param data A pointer to the data buffer.
 * @param data_size The size of the data buffer.
 * @return True if success and false if the data is longer then the buffer size.
 */
bool pack(Bus* self, uint8_t* buffer, uint16_t buffer_size, uint8_t target,
          uint8_t* data, uint8_t data_size);

/**
 * @brief Reads a byte stores in receive buffer and updates the bus state.
 * @param self A pointer to the bus struct. 
 * @param data The byte that is going to be read.
 */
void read_byte(Bus* self, uint8_t data);

/**
 * @brief Clears and sends data from receive buffer to another buffer.
 * @param self A pointer to the bus struct.
 * @param buffer A pointer to the destination buffer.
 */
void flush_data(Bus* self, uint8_t* buffer);

/**
 * @brief Return the package size.
 * @param self A pointer to the bus struct.
 * @return Package size.
 */
uint8_t get_package_size(Bus* self);

/**
 * @brief Initialized bus struct.
 * @param self A pointer to the bus struct.
 * @param address The address we want to assign to the struct.
 * 
 * Sets receive and package size to 0.
 * Sets state to IDLE.
 * Initializes all functions
 */
void lbr_net_node_init(Bus* sender, uint8_t address);
