
#pragma once

#include <math.h>

/**
 * Compares two floating point values are within a given threshold of each other.
 * 
 * @param a the first floating point value.
 * @param b the second floating point value.
 * @param threshold the threshold.
 * 
 * @return True if "equivalent" within threshold, otherwise false.
 */
#define FLOAT_CMP(a, b, threshold) (fabs((a) - (b)) < (threshold))

/**
 * Compares two floats within a threshold difference of 0.01.
 * 
 * @param a the first floating point value.
 * @param b the second floating point value.
 * 
 * @return True if "equivalent" within 0.01, otherwise false.
 */
#define FLOAT_CMP_H(a, b) FLOAT_CMP(a, b, 0.01)
