#ifndef STEP04_H
#define STEP04_H

#include "core/narr.h"
#include "core/narr_operation.h"

namespace s04
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

template <typename T>
pdl::NArr<T> numerical_diff(Variable<T> (*f)(Variable<T>), s04::Variable<T> &x, double eps=1.0e-4)
{
    auto x0 = s04::Variable<T>(x._data - eps);
    auto x1 = s04::Variable<T>(x._data + eps);
    auto y0 = f(x0);
    auto y1 = f(x1);
    auto dy = y1._data - y0._data;
    return dy/(2.0*eps);
}

template <typename T>
Variable<T> f(Variable<T> x)
{
    s04::Square<double> A;
    s04::Exp<double> B;
    s04::Square<double> C;

    auto a = A(x);
    auto b = B(a);
    auto y = C(b);
    return y;
}

}

void step04()
{
    auto value = pdl::NArr<double>({0.5});
    auto x = s04::Variable(value);
    auto dy = s04::numerical_diff(s04::f, x);
    std::cout << "data: " << dy.str().c_str() << std::endl;
}

#endif // STEP04_H
