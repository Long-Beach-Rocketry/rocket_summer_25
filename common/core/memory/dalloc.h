/**
 * Malloc implementation of alloc interface.
 */

#pragma once

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

#include "alloc.h"

/**
 * Initialize a memory allocation interface to use dynamic memory allocation.
 * 
 * @param mem the memory allocation interface.
 */
void InitDalloc(Mem* mem);

/**
 * Set the enabled state of dynamic memory allocation through the interface.
 * Allows for disabling of use after initialization time.
 * 
 * @param enable true to enable, false to disable.
 */
void SetEnableDalloc(bool enable);

/**
 * Dynamically allocate memory for one-time use.
 * 
 * @param size the number of bytes to allocate.
 * @return a pointer to the start of allocated memory.
 */
void* Dalloc(size_t size);
