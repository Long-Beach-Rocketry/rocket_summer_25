/**
 * @brief this file sets up the Quadrature encoder driver for use with other encoders
 * 
 */

#pragma once

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
//#include "stm32l4xx.h"

/**
 * 
 * @brief this struct creates needed function for encoder operation
 * 
 */
typedef struct QEnc QEnc;

struct QEnc
{
    /**
     * 
     * @brief this function gets the current timer count from the encoder
     * 
     * @param qenc a pointer to the current QEnc instance
     * 
     * @return an int with the current tick count
     */
    size_t (*getTicks)(QEnc* qenc);

    /**
     * 
     * @brief A private context parameter for concrete implementations to use.
     */
    void* priv;
};