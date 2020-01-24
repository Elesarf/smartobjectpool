#include "../SharedPool.h"
#include "../SomeHeavyObj.h"
#include <gtest/gtest.h>

class GTest_shared : public ::testing::Test
{
public:
    ~GTest_shared(){}
    GTest_shared(){}
    void SetUp(){}
    void TearDown(){}
};

SmartSharedObjectPool<SomeHeavyObj> sharedPool(2, 10);

TEST_F(GTest_shared, alloc)
{
    ASSERT_EQ(sharedPool.size(), 2);
}

TEST_F(GTest_shared, get)
{
    auto obj = sharedPool.getItem();
    obj.get()->some_one = 42;
    obj.get()->some_two = 42;

    ASSERT_EQ(sharedPool.size(), 1);
}
TEST_F(GTest_shared, grow)
{
    auto obj1 = sharedPool.fetchItem();
    auto obj2 = sharedPool.fetchItem();
    auto obj3 = sharedPool.fetchItem();
    auto obj4 = sharedPool.fetchItem();

    ASSERT_EQ(sharedPool.size(), 8);
}
