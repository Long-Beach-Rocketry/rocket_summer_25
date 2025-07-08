
#include <gtest/gtest.h>

extern "C"
{
#include "count_debounce.h"
}

class CountDebounceTest : public testing::Test
{
};

/**
 * @brief A mock GPIO structure to simulate GPIO behavior for testing.
 */
class MockGpio
{
public:
    /**
     * @brief Constructs a MockGpio with initial value set to false (LOW).
     */
    explicit MockGpio() : value(false)
    {
    }

    /**
     * @brief Static function used by the Gpio interface to read the mock pin state.
     * 
     * @param gpio Pointer to the Gpio instance.
     * @return true if the mock pin is HIGH, false if LOW.
     */
    static bool Read(Gpio* gpio)
    {
        return static_cast<MockGpio*>(gpio->priv)->value;
    }

    /**
     * @brief Sets the mock GPIO pin to a desired value.
     * 
     * @param v true for HIGH, false for LOW.
     */
    void set(bool v)
    {
        value = v;
    }

    /**
     * @brief Converts the MockGpio instance into a Gpio struct for use in the CountDebounce API.
     * 
     * @return A configured Gpio struct pointing to this mock instance.
     */
    Gpio toGpio()
    {
        Gpio g;
        g.read = Read;
        g.priv = this;
        return g;
    }

private:
    bool value;
};

/**
 * @test Verifies that CountDebounce initializes in the OFF state.
 */
TEST(CountDebounceTest, InitializesToOff)
{
    CountDebounce db;
    IoInput input;
    MockGpio mock;
    Gpio gpio = mock.toGpio();

    CountDebounce_Init(&db, &input, &gpio, 3);
    EXPECT_EQ(CountDebounce_GetState(&db), DEBOUNCE_OFF);
    EXPECT_FALSE(input.get_state(&input));
}

/**
 * @test Verifies that CountDebounce switches to ON only after reaching the threshold.
 */
TEST(CountDebounceTest, DebouncesOnAfterThreshold)
{
    CountDebounce db;
    IoInput input;
    MockGpio mock;
    Gpio gpio = mock.toGpio();
    CountDebounce_Init(&db, &input, &gpio, 3);

    mock.set(true);  // Simulate button press

    for (int i = 0; i < 2; ++i)
    {
        EXPECT_FALSE(input.get_state(&input));  // Should still be OFF
    }

    EXPECT_TRUE(input.get_state(&input));  // After 3 reads, should flip to ON
    EXPECT_EQ(CountDebounce_GetState(&db), DEBOUNCE_ON);
}

/**
 * @test Verifies that if input flips before reaching the threshold,
 * the counter resets and the state does not change.
 */
TEST(CountDebounceTest, ResetsCounterIfStateBouncesBack)
{
    CountDebounce db;
    IoInput input;
    MockGpio mock;
    Gpio gpio = mock.toGpio();
    CountDebounce_Init(&db, &input, &gpio, 3);

    mock.set(true);
    input.get_state(&input);  // 1
    input.get_state(&input);  // 2

    mock.set(false);          // bounce back
    input.get_state(&input);  // should reset counter

    EXPECT_EQ(CountDebounce_GetState(&db), DEBOUNCE_OFF);
}
