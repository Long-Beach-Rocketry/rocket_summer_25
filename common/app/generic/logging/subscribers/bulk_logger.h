/**
 * Log subscriber to buffer log entries in RAM, and only save
 * to the internal log subscriber once every n entries.
 * Allows for better compacted data, and can be post processed
 * to expand back to the original dataset after retrieval.
 */

#pragma once

#include "logging.h"

#include <string.h>

/**
 * Bulk Logger Instance.
 */
typedef struct
{
    /**
     * Number entries to buffer together before logging
     * to the internal logger.
     */
    size_t divisor;

    /**
     * Pointer to an external buffer to store combined
     * entries in.
     */
    uint8_t* buf;

    /**
     * Size of the external buffer provided.
     */
    size_t buf_size;

    /**
     * Tracks the current number of entries buffered.
     */
    size_t curr_iters;

    /**
     * The current index in the buffer that data is stored up to.
     */
    size_t buf_idx;

    /**
     * The separator to use to split buffered log entries.
     */
    uint8_t seperator;

    /**
     * Internal logger to write to after buffering entries.
     */
    LogSubscriber* internal;
} BulkLogger;

/**
 * Initializes a log subscriber as a Bulk Logger.
 * 
 * @param sub the subscriber interface to initialize as a Bulk Logger.
 * @param bulk_log an instance to initialize.
 * @param internal the subscriber to log to internally after buffering.
 * @param divisor the number of times to buffer entries before logging.
 * @param separator the character separator to split entries with.
 * @param backing_buf an external buffer to store entries in.
 * @param buf_size the size of the backing buffer provided.
 */
void BulkLoggerInit(LogSubscriber* sub, BulkLogger* bulk_log,
                    LogSubscriber* internal, size_t divisor, uint8_t separator,
                    uint8_t* backing_buf, size_t buf_size);

/**
 * Interface functions - see LogSubscriber.
 * 
 * clear - BulkLoggerClear
 * write - BulkLoggerWrite
 * retrieve_all - BulkLoggerRetrieve
 */
bool BulkLoggerClear(LogSubscriber* sub);
bool BulkLoggerWrite(LogSubscriber* sub, const uint8_t* data, size_t size);
bool BulkLoggerRetrieve(LogSubscriber* sub, Send* sender);
