#ifndef STEP03_H
#define STEP03_H

#include "core/narr.h"

namespace s03
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
    pdl::NArr<T> foward(pdl::NArr<T> na) override
    {
        return na*na;
    }
};

template <typename T>
class Exp : public Function<T>
{
public:
    pdl::NArr<T> foward(pdl::NArr<T> na) override
    {
        return na.exp();
    }
};

}

void step03()
{
    auto value = pdl::NArr<double>({0.5});
    s03::Variable x = s03::Variable(value);
    s03::Square<double> A;
    s03::Exp<double> B;
    s03::Square<double> C;

    auto a = A(x);
    auto b = B(a);
    auto y = C(b);
    std::cout << "data: " << y._data.str().c_str() << std::endl;
}

#endif // STEP03_H
