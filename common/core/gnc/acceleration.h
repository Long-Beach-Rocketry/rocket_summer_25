/**
 * Utility functions for transforming 3-axis acceleration vectors,
 * typically read from an IMU sensor.
 */

#pragma once

#include "nav_data.h"

/**
 * Rotates 3-axis acceleration vector using a given quaternion representing
 * current orientation.
 * 
 * @param[in] accel the 3-axis acceleration reading prior to conversion.
 * @param[in] orientation the orientation as a quaternion.
 * @param[out] result the resulting, reoriented 3-axis acceleration.
 * @return a pointer to the result acceleration vector for usage convenience. 
 */
ThreeAxisVec* accel_rotate(ThreeAxisVec* accel, QuaternionVec* orientation,
                           ThreeAxisVec* result);
