/**
 * Implementation of a simple Kalman Filter with a configurable model.
 * Based heavily on the theory described in the following article:
 * https://www.bzarg.com/p/how-a-kalman-filter-works-in-pictures/
 * 
 * Author: Nick Fan
 */

#pragma once

#include <stdbool.h>
#include "matrix.h"

/**
 * A model of the Kalman filter. The following matrices help
 * configure the filter based on how it expects state to change
 * over time and how it should adapt to change.
 */
typedef struct SimpleKalmanModel SimpleKalmanModel;
struct SimpleKalmanModel
{
    /**
     * The state transition (or model) matrix. This matrix
     * transforms the previous state vector (x) to the new,
     * predicted state vector. A basic kinematic model for position
     * estimation with a state vector of 
     *                        [ position ]
     *                        [ velocity ]
     * would be:
     *                      [ 1  , delta_t ]
     *                      [ 0  ,    1    ]
     */
    Matrix* A;

    /**
     * The observation/measurement matrix. This matrix is used to
     * transition from state to measurement space. In other words,
     * it describes how your sensor inputs correspond to the state.
     * Perhaps your state vector tracks position and velocity, but
     * you only have a single sensor that tells you position. In this
     * case, your H matrix would be: [ 1 , 0 ]
     * (where 1 extracts position, and 0 ignores the velocity).
     */
    Matrix* H;

    /**
     * The process noise covariance matrix. This represents the
     * uncertainty in the dynamics of the system. Given a perfect
     * model that tracks position and velocity, uneven terrain
     * or gusts of wind in real life can alter the expected
     * trajectory of your object. Using Gaussian white noise as
     * a model here is somewhat common.
     */
    Matrix* Q;

    /**
     * The measurement noise covariance matrix. This represents the
     * uncertainty in the sensor readings used as inputs to the model.
     * For example, you may have a sensor for reporting position with
     * an accuracy of +- 3 meters. Providing the filter with knowledge
     * of such expected deviations can help smooth out sensor noise.
     */
    Matrix* R;

    /**
     * An optional function pointer that can be used to update the
     * matrices every iteration of the Kalman filter. For example,
     * if timesteps aren't the same between every iteration, or sensor
     * measurement deviations vary with temperature, it's possible to
     * update the corresponding variables at runtime.
     * 
     * If initialized as NULL, it won't be used. 
     */
    bool (*update)(SimpleKalmanModel* model);

    /**
     * Private context for implementation specific details, only used
     * if consumed by the update() function.
     */
    void* priv;
};

/**
 * A Simple Kalman Filter.
 */
typedef struct
{
    /**
     * The state vector. This represents the current or predicted state
     * of the model. For a basic position and velocity tracking filter,
     * it may look like:
     *                        [ position ]
     *                        [ velocity ]
     */
    Matrix* x;

    /**
     * The state error covariance matrix. This represents the filter's
     * uncertainty in its estimates. The diagonal values represent the
     * variance of each state component, whereas the others represent
     * the covariance between the respective stat components they intersect.
     */
    Matrix* P;

    /**
     * The filter model.
     */
    SimpleKalmanModel* model;
} SimpleKalman;

/**
 * Prediction step of the Kalman Filter.
 * 
 * @param k a pointer to the Kalman Filter instance.
 * @return True on success, false otherwise.
 */
bool SimpleKalmanPredict(SimpleKalman* k);

/**
 * Estimate (update) the filter using sensor readings.
 * 
 * @param k a pointer to the Kalman Filter instance.
 * @param z the measurement vector, holding data from sensors.
 * @return True on success, false otherwise.
 */
bool SimpleKalmanEstimate(SimpleKalman* k, Matrix* z);
