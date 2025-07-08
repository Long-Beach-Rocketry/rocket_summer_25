/**
  * @brief Tests to validate nanopb.
*/
#include <gtest/gtest.h>
#include <cstdio>
extern "C"
{
#include <pb_decode.h>
#include <pb_encode.h>
#include <string.h>
#include "cmd_message.h"
#include "example.pb.h"
#include "pb_cmd.h"
}

#define EXPECT_ARR_EQ(arr1, arr2, size)   \
    do                                    \
    {                                     \
        for (size_t i = 0; i < size; ++i) \
        {                                 \
            EXPECT_EQ(arr1[i], arr2[i]);  \
        }                                 \
    } while (0)

class PBTests : public testing::Test
{
};

/**
 * @brief Test nanopb.
 */
TEST_F(PBTests, nanopb_test)
{
    uint8_t buffer[128];
    size_t message_length;
    bool status;
    {
        SimpleMessage message = SimpleMessage_init_zero;

        pb_ostream_t stream = pb_ostream_from_buffer(buffer, sizeof(buffer));

        message.lucky_number = 13;

        status = pb_encode(&stream, SimpleMessage_fields, &message);
        message_length = stream.bytes_written;
    }
    {
        SimpleMessage message = SimpleMessage_init_zero;

        pb_istream_t stream = pb_istream_from_buffer(buffer, message_length);

        status = pb_decode(&stream, SimpleMessage_fields, &message);

        EXPECT_EQ(13, (int)message.lucky_number);
    }
}
