
#include "helios_basic_cfg.h"

/**
 * Matrices for storing Kalman Filter model.
 */
MATRIX_T(static, x, 3, 1, {0});
MATRIX_T(static, P, 3, 3, {0});
MATRIX_T(static, A, 3, 3, {0});
MATRIX_T(static, H, 2, 3, {0});
MATRIX_T(static, Q, 3, 3, {0});
MATRIX_T(static, R, 2, 2, {0});

// Model instance.
static SimpleKalmanModel model = {&A, &H, &Q, &R, NULL, NULL};

bool HeliosBasicConfig(SimpleKalman* kalman, double sd_p, double sd_a, double t)
{
    // Vehicle should be stationary on initialization, hence P, V, A are 0
    MATRIX(x_d, 3, 1, {0}, {0}, {0});
    MATRIX(P_d, 3, 3, {100}, {0, 100}, {0, 0, 100});

    MATRIX(A_d, 3, 3, {1, t, t * t / 2}, {0, 1, t}, {0, 0, 1});
    MATRIX(H_d, 2, 3, {1, 0, 0}, {0, 0, 1});

    // White noise * acceleration noise propagation
    MATRIX(Q_d, 3, 3,
           {pow(t, 4) / 4 * sd_a, pow(t, 3) / 2 * sd_a, pow(t, 2) / 2 * sd_a},
           {pow(t, 3) / 2 * sd_a, pow(t, 2) * sd_a, t * sd_a},
           {pow(t, 2) / 2 * sd_a, t * sd_a, sd_a});

    // SD of Baro and SD of Accelerometer, only 2 sensors and their errors are independent
    MATRIX(R_d, 2, 2, {sd_p * 100, 0}, {0, sd_a * 100});

    if (matrix_copy(&x_d, &x) == NULL || matrix_copy(&P_d, &P) == NULL ||
        matrix_copy(&A_d, &A) == NULL || matrix_copy(&H_d, &H) == NULL ||
        matrix_copy(&Q_d, &Q) == NULL || matrix_copy(&R_d, &R) == NULL)
    {
        return false;
    }

    kalman->x = &x;
    kalman->P = &P;
    kalman->model = &model;

    return true;
}
