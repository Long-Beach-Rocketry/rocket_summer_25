
#include "simple_kalman.h"

bool SimpleKalmanPredict(SimpleKalman* k)
{
    /**
     * Update if implementation is present and fail if unsuccessful, otherwise skip.
     */
    if (k->model->update != NULL && !k->model->update(k->model))
    {
        return false;
    }

    // Predict the new state using the state transition model.
    {
        VAR_MATRIX(xp, k->x->rows, k->x->cols);
        if (matrix_copy(matrix_multiply(k->model->A, k->x, &xp), k->x) == NULL)
        {
            return false;
        }
    }

    // Calculate P prediction, Pk- = A * Pk-1 * AT + Q.
    VAR_MATRIX(A_P, k->P->rows, k->P->cols);
    VAR_MATRIX(AT, k->P->rows, k->P->cols);

    matrix_multiply(k->model->A, k->P, &A_P);
    matrix_transpose(k->model->A, &AT);
    if (matrix_add(matrix_multiply(&A_P, &AT, k->P), k->model->Q, k->P) == NULL)
    {
        return false;
    }

    return true;
}

bool SimpleKalmanEstimate(SimpleKalman* k, Matrix* z)
{
    VAR_MATRIX(gain, k->x->rows, z->rows);

    // Compute K (Kalman gain).
    {
        VAR_MATRIX(HT, k->model->H->cols, k->model->H->rows);
        VAR_MATRIX(P_HT, k->P->rows, HT.cols);

        matrix_transpose(k->model->H, &HT);
        if (matrix_multiply(k->P, &HT, &P_HT) == NULL)
        {
            return false;
        }

        // (H * Pk * HT + R )^-1
        VAR_MATRIX(H_P, k->model->H->rows, k->P->cols);
        VAR_MATRIX(H_P_HT_R, k->model->R->rows, k->model->R->cols);
        VAR_MATRIX(inv, H_P_HT_R.cols, H_P_HT_R.rows);

        matrix_multiply(k->model->H, k->P, &H_P);
        matrix_multiply(&H_P, &HT, &H_P_HT_R);
        matrix_add(&H_P_HT_R, k->model->R, &H_P_HT_R);
        matrix_inverse(&H_P_HT_R, &inv);

        // Finish calculating K.
        if (matrix_multiply(&P_HT, &inv, &gain) == NULL)
        {
            return false;
        }
    }

    // Compute new state estimate (x).
    {
        VAR_MATRIX(H_x, k->model->H->rows, k->x->cols);
        VAR_MATRIX(correc, gain.rows, H_x.cols);
        matrix_subtract(z, matrix_multiply(k->model->H, k->x, &H_x), &H_x);
        matrix_multiply(&gain, &H_x, &correc);
        matrix_add(k->x, &correc, k->x);
    }

    // Compute and update P (error covariance).
    {
        VAR_MATRIX(K_H, gain.rows, k->model->H->cols);
        VAR_MATRIX(K_H_P, K_H.rows, k->P->cols);
        matrix_multiply(&gain, k->model->H, &K_H);
        matrix_multiply(&K_H, k->P, &K_H_P);
        if (matrix_subtract(k->P, &K_H_P, k->P) == NULL)
        {
            return false;
        }
    }

    return true;
}
