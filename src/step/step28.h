#ifndef STEP28_H
#define STEP28_H

#include "pdl_core.h"
#include "util/svg_writer.h"

namespace s28
{
//
pdl::Variable<double> rosenbrockFormula(pdl::Variable<double> &x0, pdl::Variable<double> &x1)
{
    auto y = (100.0 * ((x1 - (x0^2))^2)) + ((x0 - 1.0)^2);
    return y;
}
}

//
void step28()
{
    using namespace pdl;

    NArr<double> nx0({0.0});
    NArr<double> nx1({2.0});

    auto x0 = variable<double>("x0", nx0);
    auto x1 = variable<double>("x1", nx1);

    auto lr = 0.001;
    auto iters = 1000;
    for(auto i = 0; i < iters; i++)
    {
        std::cout << "x0: " << x0.data().str().c_str() << " x1:" << x1.data().str().c_str() << std::endl;

        auto y = rosenbrockFormula(x0, x1);
        x0.cleargrad();
        x1.cleargrad();
        y.backward();

        x0.data() -= lr * x0.grad();
        x1.data() -= lr * x1.grad();
    }

    clearVariable<double>();
}


#endif // STEP28_H
