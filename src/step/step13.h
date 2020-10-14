#ifndef STEP13_H
#define STEP13_H

#include "core.h"
#include "util/svg_writer.h"

//
void step13()
{
    using namespace pdl;

    auto x =  new VariableCore<double>(NArr<double>{ 2.0 });
    auto y =  new VariableCore<double>(NArr<double>{ 3.0 });

    Add<double> f1;
    Square<double> f2;
    Square<double> f3;

    auto z = f1({f2({x}), f3({y})});
    z->backward();

    z->showData();
    x->showGrad();
    y->showGrad();

    delete x;
    delete y;
}

#endif // STEP13_H
