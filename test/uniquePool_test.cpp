#include "../UniquePool.h"
#include "../SomeHeavyObj.h"
#include <gtest/gtest.h>

class GTest_unique : public ::testing::Test
{
public:
    ~GTest_unique(){}
    GTest_unique(){}
    void SetUp(){}
    void TearDown(){}
};

SmartUniqueObjectPool<SomeHeavyObj> uniquePool;

TEST_F(GTest_unique, alloc_unique)
{
    for(int i = 0; i < 100; ++i)
        uniquePool.add(std::unique_ptr<SomeHeavyObj>(new SomeHeavyObj(i, i)));

    ASSERT_EQ(uniquePool.size(), 100);
}

TEST_F(GTest_unique, get_unique)
{
    auto obj = uniquePool.acquire();
    obj.get()->some_one = 42;
    obj.get()->some_two = 42;

    ASSERT_EQ(uniquePool.size(), 99);
}
TEST_F(GTest_unique, localGet_unique)
{

    {
        auto obj = uniquePool.acquire();
        obj.get()->some_one = 42;
        obj.get()->some_two = 42;
        ASSERT_EQ(uniquePool.size(), 99);
    }

    {
        auto obj1 = uniquePool.acquire();
        auto obj2 = uniquePool.acquire();
        auto obj3 = uniquePool.acquire();
        auto obj4 = uniquePool.acquire();
        auto obj5 = uniquePool.acquire();
    }

    ASSERT_EQ(uniquePool.size(), 100);
}
