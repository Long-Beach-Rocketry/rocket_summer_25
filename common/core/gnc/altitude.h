/**
 * Altitude calculation math.
 */

#pragma once

#include <math.h>

/**
 * Calculates relative altitude given a current and reference pressure.
 * 
 * @param pressure_pa current pressure in pascals.
 * @param sea_level_pa reference pressure in pascals.
 * @return The difference in altitude (meters) from the sea level pressure in pascals.
 */
float altitude(float pressure_pa, float sea_level_pa);