#include <gtest/gtest.h>

extern "C"
{
#include "ring_buffer.h"
}

class RingBufferTest : public testing::Test
{
public:
    static constexpr size_t BUF_SIZE = 5;

    RingBuffer buf;
    uint8_t data[BUF_SIZE];
};

TEST_F(RingBufferTest, InitTest)
{
    ring_buffer_init(&buf, data, BUF_SIZE);
    EXPECT_EQ(buf.data, data);
    EXPECT_EQ(buf.head, 0);
    EXPECT_EQ(buf.tail, 0);
    EXPECT_EQ(buf.max_len, BUF_SIZE);
}

TEST_F(RingBufferTest, InsertAndPopTest)
{
    ring_buffer_init(&buf, data, BUF_SIZE);
    for (size_t i = 0; i < BUF_SIZE; ++i)
    {
        EXPECT_TRUE(ring_buffer_insert(&buf, i));
    }

    EXPECT_FALSE(ring_buffer_insert(&buf, 0));

    uint8_t data = 0;
    for (size_t i = 0; i < BUF_SIZE; ++i)
    {
        EXPECT_TRUE(ring_buffer_pop(&buf, &data));
        EXPECT_EQ(i, data);
    }

    EXPECT_FALSE(ring_buffer_pop(&buf, &data));
}
