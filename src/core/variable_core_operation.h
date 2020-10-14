#ifndef VARIABLE_CORE_OPERATION_H
#define VARIABLE_CORE_OPERATION_H

#include "variable_core.h"

using namespace pdl;

template <typename T>
VariableCore<T> operator-(const VariableCore<T> &vc)
{
    auto ret = VariableCore<T>(vc);
    ret.data *= static_cast<T>(-1);
    return ret;
}


#endif // VARIABLE_CORE_OPERATION_H
