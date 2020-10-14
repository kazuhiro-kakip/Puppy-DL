#ifndef STEP01_H
#define STEP01_H

#include "core/narr.h"

namespace s01
{
template <typename T>
class Variable
{
public:
    pdl::NArr<T> _data;
    Variable(pdl::NArr<T> data)
    {
        _data = data;
    }
};

}

void step01()
{
    pdl::NArr<double> data({1.0});
    s01::Variable x(data);

    std::cout << "data: " << x._data.str().c_str() << std::endl;
}

#endif // STEP01_H
