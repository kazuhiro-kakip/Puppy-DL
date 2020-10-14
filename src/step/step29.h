#ifndef STEP29_H
#define STEP29_H

#include "pdl_core.h"
#include "util/svg_writer.h"

namespace s29{

pdl::Variable<double> f(pdl::Variable<double> &x)
{
    auto y = (x^4.0) - 2.0 * (x^2.0);
    return y;
}

pdl::NArr<double> gx2(pdl::NArr<double>& x)
{
    auto y =  -4.0 + (12.0 * (x^2.0));
    return y;
}

}

void step29()
{
    using namespace pdl;
    using namespace s29;

    NArr<double> value({2.0});
    auto x = variable<double>("x", value);

    auto iters = 10;
    for(auto i = 0; i < iters; i++)
    {
        std::cout << "no: " << i << " x:" << x.data().str().c_str() << std::endl;
        auto y = f(x);
        x.cleargrad();
        y.backward();

        x.data() -= x.grad()->_data/gx2(x.data());
    }

    clearVariable<double>();
}

#endif // STEP29_H
