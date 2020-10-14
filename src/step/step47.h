#ifndef STEP47_H
#define STEP47_H

#include "pdl_core.h"
#include "util/svg_writer.h"

void step47()
{
    using namespace pdl;
    auto model = MLP<double>("MLP", {10,3});

    NArr<double> nx ({{0.2, -0.4}, {0.3, 0.5}, {1.3, -3.2}, {2.1, 0.3}});
    NArr<double> nt ({2,0,1,0});

    auto x = variable<double>("x", nx);
    auto t = variable<double>("t", nt);
    auto y = model(x);

    std::cout << y.dataText() << std::endl;

    auto loss =  softmax_cross_entropy("sce", y, t);
    std::cout << "loss: " << loss.dataText().c_str() << '\n';

    clearVariable<double>();
    clearFunction<double>();
}


#endif // STEP47_H
