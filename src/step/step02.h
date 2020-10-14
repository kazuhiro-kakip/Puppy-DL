#ifndef STEP02_H
#define STEP02_H

#include "core/narr.h"

namespace s02
{
template <typename T>
class Variable
{
public:
    pdl::NArr<T> _data;
    Variable(pdl::NArr<T> &data)
    {
        _data = data;
    }
};

template <typename T>
class Function
{
public:
    Variable<T> operator()(Variable<T> v)
    {
        auto x = v._data;
        auto y = this->foward(x);
        auto output = Variable(y);
        return output;
    }

    virtual pdl::NArr<T> foward(pdl::NArr<T> na) = 0;
};

template <typename T>
class Square : public Function<T>
{
public:
    Square(){}
    pdl::NArr<T> foward(pdl::NArr<T> na) override
    {
        return na*na;
    }
};

}

void step02()
{
    auto value = pdl::NArr<double>({10});
    s02::Variable x = s02::Variable(value);
    s02::Square<double> f;
    auto y = f(x);
    std::cout << "data: " << y._data.str().c_str() << std::endl;
}

#endif // STEP02_H
