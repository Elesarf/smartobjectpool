#include <iostream>
#include <cassert>
#include <chrono>

#include "UniquePool.h"
#include "SharedPool.h"
#include "SomeHeavyObj.h"

void changeValue(SmartSharedObjectPool<SomeHeavyObj>::ptr_type a)
{
    a->some_one = 42;
}

int main()
{    
    int count = 10000;
    auto begin = std::chrono::steady_clock::now();
    SmartSharedObjectPool<SomeHeavyObj> pool(10, 2);
    for(int i = 0; i < count; ++i)
    {
        auto obj = pool.fetchItem();
        obj.get()->some_one = 12.4;

        auto obj2 = pool.getItem();
        obj2.get()->some_one = 12.4;
    }

    auto obj = pool.getItem();
    obj->some_one = 12.4;
    changeValue(obj);

    auto end = std::chrono::steady_clock::now();
    auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(end - begin);
    std::cout << "elapsed " << elapsed.count() << " ms" << std::endl;

    return 0;
}
