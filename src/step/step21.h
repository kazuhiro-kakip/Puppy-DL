#ifndef STEP33_H
#define STEP33_H

#include "core.h"
#include "util/svg_writer.h"

inline pdl::Variable<double> f(pdl::Variable<double> &x)
{
    auto y = (x^4.0) - 2.0 * (x^2.0);
    return y;
}

void step33()
{
    using namespace pdl;

    NArr<double> nx({2.0});

    auto x = variable<double>("x", nx);

    auto lr = 0.001;
    auto iters = 10;
    for(auto i = 0; i < iters; i++)
    {
        std::cout << "no: " << i << " x:" << x.data().str().c_str() << std::endl;

        auto y = f(x);
        x.cleargrad();
        y.backward();

        auto gx = variable<double>("gx", x.grad());
        std::cout << "no: " << i << " gx:" << gx.data().str().c_str() << std::endl;

        x.cleargrad();
        gx.backward();

        auto gx2 = variable<double>("gx2", x.grad());

        std::cout << "no: " << i << " gx2:" << gx2.data().str().c_str() << std::endl;

        x.data() -= gx.data() / gx2.data();
    }
    clearVariable<double>();
}

#endif // STEP33_H
