/**
 * Quaternion math library.
 */

#pragma once

#include "matrix.h"

/**
 * A quaternion instance.
 */
typedef struct
{
    /** The w component. */
    float w;

    /** The x component. */
    float x;

    /** The y component. */
    float y;

    /** The z component. */
    float z;
} QuaternionVec;

/**
 * Multiples two quaternions. Multiplication is NOT commutative.
 * 
 * @param[in] q1 the first quaternion.
 * @param[in] q2 the second quaternion.
 * @param[out] out the output quaternion.
 * @return a pointer to quaternion out (the result) for usage convenience.
 */
QuaternionVec* quat_multiply(QuaternionVec* q1, QuaternionVec* q2,
                             QuaternionVec* out);

/**
 * Conjugates a quaternion.
 * 
 * @param[in] quat the quaternion.
 * @param[out] out the output quaternion.
 * @return a pointer to quaternion out (the result) for usage convenience.
 */
QuaternionVec* quat_conjugate(QuaternionVec* quat, QuaternionVec* out);

/**
 * Compares two quaternions for near equivalency.
 * 
 * @param[in] q1 the first quaternion.
 * @param[in] q2 the second quaternion.
 * @return True if the quaternions are equivalent within a float threshold,
 *         otherwise false.
 */
bool quat_compare(QuaternionVec* q1, QuaternionVec* q2);
