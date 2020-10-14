#ifndef STEP10_H
#define STEP10_H

#include "core.h"

void step10()
{
    using namespace pdl;

    Square<double> square;
    auto x = new VariableCore(NArr<double>{3.0});
    auto y = square({x});
    y->backward();
    x->showGrad();
    delete x;
    clearVariable<double>();
}

void step10_2()
{
    using namespace pdl;

    auto x = variable<double>("x", NArr<double>{3.0});
    auto y = x^2;
    y.backward();
    x.showGradient();
}

#endif // STEP10_H
