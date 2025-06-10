/**
 * @file io_input.h
 * @author Ayleen Perez
 * @brief Generic input interface for application-agnostic digital input handling.
 *
 * This interface allows app logic to read input state using a common structure,
 * regardless of the underlying implementation (e.g., raw GPIO or debounced GPIO).
 */
#pragma once

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

typedef struct IoInput IoInput;

/**
 * @brief Abstract input interface for digital signal sources.
 */
struct IoInput
{
    /**
     * @brief Retrieves the current logical state of the input.
     * 
     * Should return true for "active/high", false for "inactive/low".
     */
    bool (*get_state)(IoInput* in);

    /**
     * @brief Implementation-specific pointer (e.g., CountDebounce*, Gpio*, etc.)
     */
    void* priv;
};
