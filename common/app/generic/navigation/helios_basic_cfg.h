/**
 * A basic Kalman filter model for altitude estimation,
 * to configure a Helios navigator.
 * 
 * Author: Nick Fan
 */

#pragma once

#include "simple_kalman.h"

#include <math.h>

/**
 * Configures a Simple Kalman Filter's model, state, and covariance
 * matrices using given seeding constants.
 * 
 * @param kalman the Kalman filter to configure.
 * @param sd_p standard deviation of position.
 * @param sd_a standard deviation of acceleration.
 * @return True on success, false otherwise.
 */
bool HeliosBasicConfig(SimpleKalman* kalman, double sd_p, double sd_a,
                       double t);
