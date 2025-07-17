/**
 * 
 * @brief this file redefines already stated functions
 * 
 */

#pragma once

#include "gpio.h"
#include "qenc.h"
#include "stm32l476xx.h"

typedef struct StQenc StQenc;

struct StQenc
{
    TIM_TypeDef* instance;
    size_t count;
    Gpio* channel_a;
    Gpio* channel_b;
};

typedef enum
{
    ONE = 0,
    TWO,
    THREE,
    FOUR
} QencCH;

/**
 * 
 * @brief this function initializes settings for a QEnc object
 * 
 * @param enc A pointer to the current QEnc instance.
 * @param st_enc A pointer to the current StQenc object.
 * @param channel_a A pointer to a Gpio object.
 * @param channel_b A pointer to a Gpio object.
 * @param base_addr The base address of the desired HAL_TIM to be used.
 * @param channel_choice_a Channel A selection. Takes enum type as input. Input values: ONE, TWO, THREE, FOUR.
 * @param channel_choice_b Channel B selection. Takes enum type as input. Input values: ONE, TWO, THREE, FOUR.
 * 
 */
void StEncInit(QEnc* enc, StQenc* st_enc, Gpio* channel_a, Gpio* channel_b,
               size_t base_addr, QencCH channel_choice_a,
               QencCH channel_choice_b);
/**
 * 
 * @brief this function gets the current timer count from the encoder
 * 
 * @param qenc a pointer to the current QEnc instance
 * 
 * @return an int with the current tick count
 */
size_t StGetEncCount(QEnc* qenc);