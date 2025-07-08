
#include <gtest/gtest.h>

extern "C"
{
#include "quaternion.h"
}

#define EXPECT_QUAT_EQ(q1, q2) EXPECT_TRUE(quat_compare(q1, q2))

class QuaternionTest : public testing::Test
{
};

TEST_F(QuaternionTest, QuatTest)
{
    QuaternionVec a = {1, 1, 1, 1};
    QuaternionVec res = {1, 1, 1, 1};
    EXPECT_QUAT_EQ(&a, &res);

    {
        QuaternionVec exp = {1, -1, -1, -1};
        EXPECT_QUAT_EQ(quat_conjugate(&a, &res), &exp);
    }

    {
        QuaternionVec b = {1, 2, 3, 4};
        QuaternionVec exp = {-8, 4, 2, 6};
        EXPECT_QUAT_EQ(quat_multiply(&a, &b, &res), &exp);
    }

    {
        // Measured acceleration quaternion
        QuaternionVec ac = {0, 0, 0, 9.8};
        // Measured orientation quaternion
        QuaternionVec o = {.707, -.707, 0, 0};
        // Orientation conjugate
        QuaternionVec o_c;
        QuaternionVec intrm;
        QuaternionVec exp = {0, 0, 9.8, 0};

        // Extracting vertical acceleration
        quat_conjugate(&o, &o_c);
        quat_multiply(&o, &ac, &intrm);
        quat_multiply(&intrm, &o_c, &res);
        EXPECT_QUAT_EQ(&res, &exp);
    }
}
