#include <gtest/gtest.h>

extern "C"
{
#include "fake_dcm.h"
}

class Drv8242DcmTest : public testing::Test
{
public:
    DCMotor motor;
    DCPosControl dev;
    Pwm pwm;
};

TEST_F(Drv8242DcmTest, InitTest)
{
    FakeInit(&motor, &dev);
    //FakeSetDuty(&motor, 50); segfault
    EXPECT_EQ(dev.state, OFF);
    FakeSetEnable(&motor, false);
    EXPECT_EQ(FakeSetEnable(&motor, false), false);
    EXPECT_EQ(FakeSetDir(&motor, false), false);
    EXPECT_EQ(FakeSetEnable(&motor, true), true);
    FakeSetEnable(&motor, true);
    EXPECT_EQ(FakeSetDir(&motor, true), true);
    EXPECT_EQ(dev.state, RUNNING);
}
