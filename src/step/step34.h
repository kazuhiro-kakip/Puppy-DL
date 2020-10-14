#ifndef STEP34_H
#define STEP34_H

#include "pdl_core.h"
#include "util/svg_writer.h"

void step34_1()
{
    pdl::NArr<double> value({1.0});
    auto x = new pdl::VariableCore<double>(value);
    auto fsin = pdl::Sin<double>();
    auto y = fsin.operator()({x});
    y->backward();

    for(int i = 0; i < 3; i++)
    {
        auto gx = x->_grad;
        x->clearGrad();
        gx->backward();
        std::cout << x->grad().data() << std::endl;
    }
    pdl::clearVariable<double>();
}

void step34_2()
{
    auto value = pdl::NArr<double>::linespace(-7.0, 7.0, 200);
    auto x = new pdl::VariableCore<double>(value);
    auto fsin = pdl::Sin<double>();
    auto y = fsin.operator()({x});
    y->backward();

    std::vector<std::vector<double>> logs;
    logs.push_back(y->_data.as_vector());

    for(auto i = 0; i < 3; i++)
    {
        logs.push_back(x->_grad->_data.as_vector());
        auto gx = x->_grad;
        x->clearGrad();
        gx->backward();
    }

    pdl::clearVariable<double>();
}

#endif // STEP34_H
