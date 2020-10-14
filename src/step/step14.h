#ifndef STEP14_H
#define STEP14_H

#include "core.h"
#include "util/svg_writer.h"

void step14_1()
{
    using namespace pdl;
    auto x =  new VariableCore<double>(NArr<double>{ 3.0 });

    Add<double> f1, f2, f3;
    auto z = f1({x,x});

    z->backward();
    z->showData();
    x->showGrad();

    x->clearGrad();
    auto y2 = f3({x, f2({x,x})});
    y2->backward();

    y2->showData();
    x->showGrad();

    delete x;
}

#endif // STEP14_H
