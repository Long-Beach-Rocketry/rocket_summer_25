/**
 * Driver and application initialization memory allocation interface.
 */

#pragma once

#include <stdlib.h>

/**
 * Allocates memory for a given type.
 * 
 * @param mem the memory allocation interface.
 * @param type the data type.
 * 
 * @return a pointer to the allocated instance.
 */
#define ALLOC(mem, type) (type*)(mem)->alloc(sizeof(type))

/**
 * Allocates a consecutive number (array) of a given type.
 * 
 * @param mem the memory allocation interface.
 * @param type the data type.
 * 
 * @return a pointer to the contiguous block of memory.
 */
#define ALLOC_N(mem, type, num) (type*)(mem)->alloc(sizeof(type) * num)

/**
 * Implementation of memory, for one-time allocation of
 * data structures meant to be used for the entire program
 * lifetime, such as drivers.
 */
typedef struct Mem Mem;
struct Mem
{
    /**
     * One-time allocate a given number of bytes.
     * 
     * @param size the number of bytes to allocate.
     * @return a pointer to the start of allocated memory.
     */
    void* (*alloc)(size_t size);
};
