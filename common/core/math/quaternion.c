
#include "quaternion.h"

QuaternionVec* quat_multiply(QuaternionVec* q1, QuaternionVec* q2,
                             QuaternionVec* out)
{
    out->w = q1->w * q2->w - q1->x * q2->x - q1->y * q2->y - q1->z * q2->z;
    out->x = q1->w * q2->x + q1->x * q2->w + q1->y * q2->z - q1->z * q2->y;
    out->y = q1->w * q2->y - q1->x * q2->z + q1->y * q2->w + q1->z * q2->x;
    out->z = q1->w * q2->z + q1->x * q2->y - q1->y * q2->x + q1->z * q2->w;

    return out;
}

QuaternionVec* quat_conjugate(QuaternionVec* quat, QuaternionVec* out)
{
    out->w = quat->w;
    out->x = -quat->x;
    out->y = -quat->y;
    out->z = -quat->z;

    return out;
}

bool quat_compare(QuaternionVec* q1, QuaternionVec* q2)
{
    return (FLOAT_CMP_H(q1->w, q2->w)) && (FLOAT_CMP_H(q1->x, q2->x)) &&
           (FLOAT_CMP_H(q1->y, q2->y)) && (FLOAT_CMP_H(q1->z, q2->z));
}
