/**
 * Navigation algorithm using a generic nav suite interface
 * and Simple Kalman Filter.
 * 
 * Author: Nick Fan
 */

#pragma once

#include "acceleration.h"
#include "altitude.h"
#include "nav_data.h"
#include "navigator.h"
#include "simple_kalman.h"

/**
 * Helios Navigation Algorithm.
 */
typedef struct
{
    /**
     * Navigation sensor data interface. Provides
     * raw sensor readings.
     */
    NavData* data;

    /**
     * Simple Kalman Filter instance.
     */
    SimpleKalman kalman;

    /**
     * The pressure measured at base altitude to
     * use as reference for altitude calculation.
     */
    double base_pressure;

    /**
     * Flag to indicate whether algorithm has been
     * started and initialized appropriately.
     */
    bool started;
} Helios;

/**
 * Initializes a Navigator interface with a Helios implementation.
 * 
 * @param nav the navigator interface.
 * @param helios a helios struct to use for the instance.
 * @param nav_data a navigation suite for pulling raw sensor data from.
 * @return True on success, false otherwise.
 */
bool HeliosNavigatorInit(Navigator* nav, Helios* helios, NavData* nav_data);

/**
 * Interface functions - see Navigator interface:
 * altitude - HeliosAltitude
 * velocity - HeliosVelocity
 * acceleration - HeliosAcceleration
 * orientation - HeliosOrientation
 * start - HeliosStart
 * update - HeliosUpdate
 */
bool HeliosStart(Navigator* nav);
bool HeliosUpdate(Navigator* nav);
double HeliosAltitude(Navigator* nav);
double HeliosVelocity(Navigator* nav);
double HeliosAcceleration(Navigator* nav);
const QuaternionVec* HeliosOrientation(Navigator* nav);
