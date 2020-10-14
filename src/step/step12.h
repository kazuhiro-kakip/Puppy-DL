#ifndef STEP12_H
#define STEP12_H

#include <QDebug>
#include "pdl_core.h"
#include "util/svg_writer.h"

void step12()
{
    auto x = new pdl::VariableCore<double>(pdl::NArr<double>{2.0});
    auto y = new pdl::VariableCore<double>(pdl::NArr<double>{3.0});

    pdl::Add<double> add;

    auto z = add({x,y});
    z->backward();
    z->showData();

    delete x;
    delete y;
}

#endif // STEP12_H
