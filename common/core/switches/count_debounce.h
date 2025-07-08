#pragma once

#include <stdbool.h>
#include <stdint.h>
#include "gpio.h"
#include "io_input.h"

/**
 * @brief Represents the two possible stable states of a debounced input.
 */
typedef enum
{
    DEBOUNCE_OFF = 0, /**< Input is stable and low (released) */
    DEBOUNCE_ON = 1   /**< Input is stable and high (pressed) */
} DebounceState;

/**
 * @brief Implements debounce logic for a GPIO input using the IoInput interface.
 * 
 * Tracks the number of consecutive opposite readings from a GPIO input.
 * If the number of readings exceeds a threshold, the stable state is toggled.
 */
typedef struct
{
    DebounceState state; /**< The current debounced (stable) state */
    uint8_t counter;     /**< Counter for consecutive opposite readings */
    uint8_t
        threshold; /**< Number of consecutive changes before flipping state */
    Gpio* gpio;    /**< Underlying GPIO source being debounced */
} CountDebounce;

/**
 * @brief Initializes the CountDebounce logic and assigns it to an IoInput interface.
 * 
 * @param db Pointer to the CountDebounce instance.
 * @param input Pointer to IoInput interface this debouncer will implement.
 * @param gpio GPIO input source to read from.
 * @param threshold Number of consecutive opposite readings required to flip state.
 */
void CountDebounce_Init(CountDebounce* db, IoInput* input, Gpio* gpio,
                        uint8_t threshold);

/**
 * @brief Returns the current stable debounced state.
 * 
 * @param db Pointer to CountDebounce instance.
 * @return The current DebounceState.
 */
DebounceState CountDebounce_GetState(CountDebounce* db);

/**
 * @brief Returns true if the debounced state is ON.
 * 
 * @param db Pointer to CountDebounce instance.
 * @return true if stable state is ON, false otherwise.
 */
bool CountDebounce_IsPressed(CountDebounce* db);

/**
 * @brief Returns true if the debounced state is OFF.
 * 
 * @param db Pointer to CountDebounce instance.
 * @return true if stable state is OFF, false otherwise.
 */
bool CountDebounce_IsReleased(CountDebounce* db);

/**
 * @brief Implements IoInput->get_state using CountDebounce logic.
 * 
 * Updates the internal state on each call and returns true if the input is stable and ON.
 * 
 * @param input Pointer to IoInput whose `priv` field is a CountDebounce*.
 * @return true if the debounced GPIO state is ON, false otherwise.
 */
bool CountDebounce_GetStateFromIoInput(IoInput* input);
