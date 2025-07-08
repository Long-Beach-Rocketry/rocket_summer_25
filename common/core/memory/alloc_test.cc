

#include <gtest/gtest.h>

extern "C"
{
#include "dalloc.h"
#include "prealloc.h"
}

constexpr size_t BUF_SIZE = 1000;

class AllocTest : public testing::Test
{
public:
    uint8_t prealloc_buf[BUF_SIZE];
    Mem mem;
};

struct TestStruct
{
    uint8_t d1;
    uint8_t* d2;
    uint32_t d3;
    size_t d4;
};

struct Overflow
{
    uint8_t buf[BUF_SIZE + 1];
};

void test_alloc(Mem* test_mem)
{
    // Test successful allocation.
    TestStruct* t = ALLOC(test_mem, TestStruct);
    *t = {1, nullptr, 32, 64};
    EXPECT_EQ(t->d1, 1);
    EXPECT_EQ(t->d2, nullptr);
    EXPECT_EQ(t->d3, 32);
    EXPECT_EQ(t->d4, 64);
}

TEST_F(AllocTest, PreallocTest)
{
    // Init.
    InitPrealloc(&mem, prealloc_buf, BUF_SIZE);

    // Test over allocation.
    Overflow* np = ALLOC(&mem, Overflow);
    EXPECT_EQ(np, nullptr);

    test_alloc(&mem);
}

TEST_F(AllocTest, DallocTest)
{
    // Init.
    InitDalloc(&mem);

    // Test disabled.
    TestStruct* t = ALLOC(&mem, TestStruct);
    EXPECT_EQ(t, nullptr);

    SetEnableDalloc(true);
    test_alloc(&mem);
}
