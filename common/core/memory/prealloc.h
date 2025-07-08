/**
 * Pre-allocated memory implementation of alloc interface.
 */

#pragma once

#include <stdbool.h>
#include <stdint.h>

#include "alloc.h"

/**
 * Initialize a memory allocation interface to use pre-allocated memory.
 * 
 * @param mem the memory allocation interface.
 */
bool InitPrealloc(Mem* mem, uint8_t* data, size_t size);

/**
 * Allocate memory for one-time use from pre-allocated memory.
 * 
 * @param size the number of bytes to allocate.
 * @return a pointer to the start of allocated memory.
 */
void* Prealloc(size_t size);
