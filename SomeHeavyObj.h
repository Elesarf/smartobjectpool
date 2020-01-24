#ifndef SOMEHEAVYOBJ_H
#define SOMEHEAVYOBJ_H

#include <iostream>

struct SomeHeavyObj
{
    SomeHeavyObj():
        some_one(0.0),
        some_two(0.0)
    {
        std::cout << "Construct" << std::endl;
    }

    SomeHeavyObj(double one, double two):
        some_one(one),
        some_two(two)
    {}

    ~SomeHeavyObj()
    {
        std::cout << "Destruct" << std::endl;
    }

    double some_one{0.0};
    double some_two{0.0};
};

#endif // SOMEHEAVYOBJ_H
