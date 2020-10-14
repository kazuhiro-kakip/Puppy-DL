#ifndef STEP33_H
#define STEP33_H

#include "pdl_core.h"
#include "util/svg_writer.h"

namespace s33
{

inline pdl::Variable<double> f(pdl::Variable<double> &x)
{
    auto y = (x^4.0) - (2.0 * (x^2));
    return y;
}

}

void step33()
{
    using namespace s33;

    using namespace pdl;

    NArr<double> nx({2.0});

    auto x = variable<double>("x", nx);

    //auto lr = 0.001;
    auto iters = 10;
    for(auto i = 0; i < iters; i++)
    {
        std::cout << "no: " << i << " x:" << x.data().str().c_str() << std::endl;

        auto y = s33::f(x);
        x.cleargrad();
        y.backward();

        auto gx = x.grad();
        std::cout << "no: " << i << " gx:" << x.grad()->_data.str().c_str() << std::endl;
        auto g1 = x.grad()->_data;
        x.cleargrad();
        gx->backward();

        std::cout << "no: " << i << " gx2:" << x.grad()->_data.str().c_str() << std::endl;
        auto g2 = x.grad()->_data;

        x.data() -= g1 / g2;
        std::cout << "no: " << i << " x.data:" << x.data().str().c_str() << std::endl;
    }
    clearVariable<double>();
}



#endif // STEP33_H
