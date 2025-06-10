#include <gtest/gtest.h>
#include <stdio.h>

extern "C"
{
#include "fake_dcm.h"
#include "fake_dcm_control.h"
#include "fake_enc.h"
}

class DcmControlTest : public testing::Test
{
public:
    DCM_Control dev;
    DCPosControl motor_ctrl;
    MotorRotoationCtrler qenc_ctrl;
    QEncPriv st_qenc;
    DCMotor motor;
    QEnc qenc;
    uint32_t base_addr = 0;
};

TEST_F(DcmControlTest, InitTest)
{
    FakeInit(&motor, &motor_ctrl);
    QEnc_Init(&qenc, &qenc_ctrl, &st_qenc, &motor, base_addr);
    DCM_Control_Init(&dev, &motor_ctrl, &qenc_ctrl);
    FakeSetEnable(&motor, true);
    config(&dev);

    EXPECT_TRUE(qenc_ctrl.command_rotate(&qenc_ctrl, 10));
    EXPECT_EQ(qenc_ctrl.state, idle);
    motor_update(&dev);
    EXPECT_EQ(qenc_ctrl.state, rotating);
    fake_increment(&qenc, 5);
    motor_update(&dev);
    EXPECT_EQ(qenc_ctrl.state, rotating);  //still rotating after incrementing 5
    fake_increment(&qenc, 5);
    motor_update(&dev);
    EXPECT_EQ(qenc_ctrl.state, idle);  //idle after reaching 10;
    EXPECT_FALSE(dev.cmd_enable);
}

TEST_F(DcmControlTest, OverflowTest)
{
    FakeInit(&motor, &motor_ctrl);
    QEnc_Init(&qenc, &qenc_ctrl, &motor);
    DCM_Control_Init(&dev, &motor_ctrl, &qenc_ctrl);
    FakeSetEnable(&motor, true);
    config(&dev);

    qenc_ctrl.command_rotate(&qenc_ctrl, 75000);
    motor_update(&dev);

    int i = 0;
    int count = 0;
    size_t increment;
    while (i < 15000)
    {
        increment = fake_increment(&qenc, 5);
        motor_update(&dev);
        if (qenc_ctrl.diff <= 75000)
        {
            count++;
        }

        i++;
    }

    EXPECT_EQ(count, 15000);     //number of iterations
    EXPECT_EQ(increment, 9465);  //(15000 - 13107) * 5 = 9465
    EXPECT_EQ(qenc_ctrl.diff, 75000);

    size_t decrement;
    count = 0;
    i = 0;
    FakeSetEnable(&motor, true);
    config(&dev);
    qenc_ctrl.command_rotate(&qenc_ctrl, -75100);
    motor_update(&dev);

    while (i < 30020)
    {
        decrement = fake_decrement(&qenc, 5);
        motor_update(&dev);
        if (qenc_ctrl.diff >= (-75100))
        {
            count++;
        }
        i++;
    }
    EXPECT_EQ(count, 30020);
    EXPECT_EQ(qenc_ctrl.diff, -75100);
    EXPECT_EQ(decrement, 55535);
    EXPECT_EQ(qenc_ctrl.ticks_needed, -75100);
}
