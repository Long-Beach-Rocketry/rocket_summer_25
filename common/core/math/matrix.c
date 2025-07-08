
#include "matrix.h"

Matrix* matrix_copy(Matrix* src, Matrix* dest)
{
    if (src == NULL || dest == NULL)
    {
        return NULL;
    }

    for (size_t i = 0; i < (src)->rows; ++i)
    {
        for (size_t j = 0; j < (src)->cols; j++)
        {
            MAT_GET(dest, i, j) = MAT_GET(src, i, j);
        }
    }

    return dest;
}

Matrix* matrix_multiply(Matrix* A, Matrix* B, Matrix* C)
{
    if (A == NULL || B == NULL || C == NULL || A->cols != B->rows ||
        C->rows * C->cols != A->rows * B->cols)
    {
        return NULL;
    }

    for (size_t i = 0; i < A->rows; i++)
    {
        for (size_t j = 0; j < B->cols; j++)
        {
            C->data[i * C->cols + j] = 0;
            for (size_t k = 0; k < A->cols; k++)
            {
                MAT_GET(C, i, j) += MAT_GET(A, i, k) * MAT_GET(B, k, j);
            }
        }
    }

    return C;
}

Matrix* matrix_transpose(Matrix* A, Matrix* AT)
{
    if (A == NULL || AT == NULL || A->rows * A->cols != AT->cols * AT->rows)
    {
        return NULL;
    }

    for (size_t i = 0; i < A->rows; i++)
    {
        for (size_t j = 0; j < A->cols; j++)
        {
            MAT_GET(AT, j, i) = MAT_GET(A, i, j);
        }
    }

    return AT;
}

Matrix* matrix_add(Matrix* A, Matrix* B, Matrix* C)
{
    if (A == NULL || B == NULL || C == NULL || A->rows != B->rows ||
        A->cols != B->cols || A->rows != C->rows || A->cols != C->cols)
    {
        return NULL;
    }

    for (size_t i = 0; i < A->cols * A->rows; i++)
    {
        C->data[i] = A->data[i] + B->data[i];
    }

    return C;
}

Matrix* matrix_subtract(Matrix* A, Matrix* B, Matrix* C)
{
    if (A == NULL || B == NULL || C == NULL || A->rows != B->rows ||
        A->cols != B->cols || A->rows != C->rows || A->cols != C->cols)
    {
        return NULL;
    }

    for (size_t i = 0; i < A->rows * A->cols; i++)
    {
        C->data[i] = A->data[i] - B->data[i];
    }

    return C;
}

Matrix* matrix_inverse(Matrix* A, Matrix* AInv)
{
    if (A == NULL || AInv == NULL || A->rows != AInv->rows ||
        A->cols != AInv->cols)
    {
        return NULL;
    }

    float d;
    size_t i, j, k;
    size_t n = A->rows;

    MAT_AUGMENT(A, aug_mat);
    Matrix* aug = &aug_mat;

    // Fill in identity
    for (i = 0; i < n; i++)
    {
        for (j = 0; j < 2 * n; j++)
        {
            if (j == (i + n))
            {
                MAT_GET(aug, i, j) = 1;
            }
            else if (j >= n)
            {
                MAT_GET(aug, i, j) = 0;
            }
        }
    }

    // Perform partial pivoting
    for (i = n - 1; i > 0; i--)
    {
        if (MAT_GET(aug, i - 1, 0) < MAT_GET(aug, i, 0))
        {
            for (j = 0; j < n * 2; j++)
            {
                d = MAT_GET(aug, i, j);
                MAT_GET(aug, i, j) = MAT_GET(aug, i - 1, j);
                MAT_GET(aug, i - 1, j) = d;
            }
        }
    }

    // Reduce the matrix to diagonal matrix (upper triangular)
    for (i = 0; i < n; i++)
    {
        for (j = 0; j < n * 2; j++)
        {
            if (j != i)
            {
                d = MAT_GET(aug, j, i) / MAT_GET(aug, i, i);
                for (k = 0; k < n * 2; k++)
                {
                    MAT_GET(aug, j, k) -= MAT_GET(aug, i, k) * d;
                }
            }
        }
    }

    // Reduce to a unit matrix (diagonal ones)
    for (i = 0; i < n; i++)
    {
        d = MAT_GET(aug, i, i);
        for (j = 0; j < n * 2; j++)
        {
            MAT_GET(aug, i, j) = MAT_GET(aug, i, j) / d;
        }
    }

    // Extract result
    size_t c = 0;
    for (i = 0; i < n; i++)
    {
        for (j = n; j < n * 2; j++)
        {
            AInv->data[c++] = MAT_GET(aug, i, j);
        }
    }

    return AInv;
}

bool matrix_compare(Matrix* A, Matrix* B)
{
    if (A == NULL || B == NULL)
    {
        return NULL;
    }

    if (A->rows != B->rows || A->cols != B->cols)
    {
        return false;
    }

    for (size_t i = 0; i < A->rows; ++i)
    {
        for (size_t j = 0; j < A->cols; ++j)
        {
            if (FLOAT_CMP_H(MAT_GET(A, i, j), MAT_GET(B, i, j)))
            {
                return false;
            }
        }
    }

    return true;
}

char* matrix_to_string(Matrix* mat)
{
    static char str[MAX_STR_LENGTH];
    char temp[50];
    size_t pos = 0;

    str[0] = '\0';  // Initialize the string

    if (mat == NULL)
    {
        strcat(str, "[null]\n");
        return str;
    }

    for (size_t i = 0; i < mat->rows; i++)
    {
        strcat(str, "[ ");
        pos += 2;

        for (size_t j = 0; j < mat->cols; j++)
        {
            snprintf(temp, sizeof(temp), "%.4f ", mat->data[i * mat->cols + j]);

            // Check if adding this number would exceed the buffer
            if (pos + strlen(temp) >=
                MAX_STR_LENGTH - 3)  // -3 for "]\n" and null terminator
            {
                strcat(str, "...]");
                return str;
            }

            strcat(str, temp);
            pos += strlen(temp);
        }

        strcat(str, "]\n");
        pos += 2;
    }

    return str;
}
