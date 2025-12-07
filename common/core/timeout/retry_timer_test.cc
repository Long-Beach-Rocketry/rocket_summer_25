
#include <gtest/gtest.h>

extern "C"
{
#include "retry_timer.h"
}

class RetryTimerTest : public testing::Test
{
public:
    static constexpr size_t RETRY_COUNT = 5;
    RetryData rd;
    Timeout timer;

    bool expire()
    {
        static size_t i = 0;
        if (i++ > RETRY_COUNT)
        {
            i = 0;
            return true;
        }
        return false;
    }

    bool pass()
    {
        static size_t i = 0;
        if (i++ == RETRY_COUNT)
        {
            return true;
        }
        else if (i > RETRY_COUNT)
        {
            i = 0;
        }
        return false;
    }

    bool test_expire(Timeout* timer)
    {
        WAIT(timer, expire(), true);
        return false;
    }

    bool test_pass(Timeout* timer)
    {
        WAIT(timer, pass(), false);
        return true;
    }
};

//added to fix linker error on mac circle back on this
constexpr size_t RetryTimerTest ::RETRY_COUNT;

TEST_F(RetryTimerTest, InitTest)
{
    retry_timer_init(&timer, &rd, RETRY_COUNT);
    EXPECT_EQ(rd.counter, 0);
    EXPECT_EQ(rd.reset, RETRY_COUNT);
}

TEST_F(RetryTimerTest, TimeoutTest)
{
    retry_timer_init(&timer, &rd, RETRY_COUNT);
    test_expire(&timer);
    test_pass(&timer);
}
