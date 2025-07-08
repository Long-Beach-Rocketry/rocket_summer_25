/**
 * Navigation data collection interface.
 */

#pragma once

#include "gnc_constants.h"
#include "quaternion.h"

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

/**
 * Three axis vector of float values.
 */
typedef struct
{
    /** The x-cooredinate. */
    float x;

    /** The y-cooredinate. */
    float y;

    /** The z-cooredinate. */
    float z;
} ThreeAxisVec;

/**
 * Three axis vector of euler angles.
 */
typedef struct
{
    /** The x-cooredinate. */
    int16_t x;

    /** The y-cooredinate. */
    int16_t y;

    /** The z-cooredinate. */
    int16_t z;
} EulerVec;

/**
 * Navigation data interface.
 */
typedef struct NavData NavData;
struct NavData
{
    /** Barometer pressure measurement. */
    float pressure;

    /** IMU/accelerometer measurement. */
    ThreeAxisVec accel;

    /** IMU orientation quaternion measurement. */
    QuaternionVec quat;

    /** Temperature measurement. */
    float temperature;

    /** Tick time. */
    size_t tick;

    /**
     * Updates the navigation data points with sensor measurements.
     * 
     * @param data a self pointer.
     * @return True on success, false otherwise.
     */
    bool (*update)(NavData* data);

    /**
     * Private context for use in concrete implementations.
     */
    void* priv;
};
