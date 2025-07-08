/**
 * Navigator interface to abstract filtering and interpretation
 * of raw sensor data. Provides a generic API for developing a
 * flight state estimation algorithm independent of data sources
 * across different platforms, whether it is real sensors on real
 * hardware or processed data logs on native simulation.
 * 
 * Author: Nick Fan
 */

#pragma once

#include "nav_data.h"

/**
 * A Navigator.
 */
typedef struct Navigator Navigator;
struct Navigator
{
    /**
     * Gets the current estimated altitude.
     * @return The altitude as a double.
     */
    double (*altitude)(Navigator* nav);

    /**
     * Gets the current estimated velocity.
     * @return The velocity as a double.
     */
    double (*velocity)(Navigator* nav);

    /**
     * Gets the current estimated acceleration.
     * @return The acceleration as a double.
     */
    double (*acceleration)(Navigator* nav);

    /**
     * Gets the current measured orientation as a quaternion.
     * @return The quaternion as a QuaternionVec struct pointer.
     */
    const QuaternionVec* (*orientation)(Navigator* nav);

    /**
     * Starts the navigation algorithm.
     * @return True on success, false otherwise.
     */
    bool (*start)(Navigator* nav);

    /**
     * Updates the navigation algorithm, expected to be called
     * every iteration/at the expected update rate.
     * @return True on success, false otherwise.
     */
    bool (*update)(Navigator* nav);

    /**
     * Private context for use by concrete implementations.
     */
    void* priv;
};
