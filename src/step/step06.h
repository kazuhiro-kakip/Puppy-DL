#ifndef STEP06_H
#define STEP06_H

#include "pdl_core.h"

void step06()
{
    using namespace  pdl;

    std::cout << "step6" << std::endl;

    pdl::Square<double> A;
    pdl::Exp<double> B;
    pdl::Square<double> C;

    auto x = new pdl::VariableCore<double>(pdl::NArr<double>{0.5});
    auto a = A({x});
    auto b = B({a});
    auto y = C({b});

    y->backward();

    std::cout << "y data: " << x->data().c_str() << std::endl;
    std::cout << "x grad: " << x->grad().c_str() << std::endl;

    delete x;
}


#endif // STEP_H
