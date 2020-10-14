#ifndef VARIABLE_OPERATION_H
#define VARIABLE_OPERATION_H

#include <iostream>
#include <queue>

#include "variable.h"
#include "variable_manager.h"
#include "function_manager.h"

namespace pdl {

template <typename T> VariableCore<T>* getCoreSafely(Variable<T> &v)
{
    if(v.name() != "")
        return VariableManager<T>::Instance()->get(v.name());

    else if(v.id() != 0)
        return VariableManager<T>::Instance()->get(v.id());

    return nullptr;
}


template <typename T> Variable<T> operator+(Variable<T> v1, Variable<T> v2) {
    Add<T>* add = FunctionManager<T>::Instance()->add();

    auto vc1 = getCoreSafely(v1);
    auto vc2 = getCoreSafely(v2);

    auto vc =  add->operator()({vc1, vc2});
    return Variable<T>(vc->id());
}


template <typename T> inline Variable<T> operator+(T n, Variable<T> v)
{
    auto vc1 = getCoreSafely(v);
    auto vc2 = VariableManager<T>::Instance()->get(NArr<T>{n});
    Add<T>* add = FunctionManager<T>::Instance()->add();
    auto vc = add->operator()({vc1, vc2});
    return Variable<T>(vc->id());
}

template <typename T> Variable<T> operator+(Variable<T> v, T n)
{
    auto vc1 = getCoreSafely(v.name());
    auto vc2 = VariableCore<T>(NArr<T>{n});
    Add<T>* add = FunctionManager<T>::Instance()->add();

    auto vc = add->operator()({vc1, vc2});
    return Variable<T>(vc->id());
}

template <typename T> Variable<T> operator-(Variable<T> v1, Variable<T> v2) {
    Sub<T>* sub = FunctionManager<T>::Instance()->sub();
    auto vc1 = getCoreSafely(v1);
    auto vc2 = getCoreSafely(v2);
    auto vc = sub->operator()({vc1, vc2});
    return Variable<T>(vc->id());
}

template <typename T> Variable<T> operator-(Variable<T> v, T n)
{
    auto vc1 = getCoreSafely(v);
    auto vc2 = VariableManager<T>::Instance()->get(NArr<T>{n});

    Sub<T>* sub = FunctionManager<T>::Instance()->sub();
    auto vc = sub->operator()({vc1, vc2});
    return Variable<T>(vc->id());
}

template <typename T> Variable<T> operator-(T n, Variable<T> v)
{
    auto vc1 = getCoreSafely(v);
    auto vc2 = VM->get(NArr<T>{n});
    Sub<T>* sub = FM->sub();
    auto vc = sub->operation()({vc1, vc2});
    return Variable<T>(vc->id());
}

template <typename T> Variable<T> operator*(Variable<T> v1, Variable<T> v2) {
    Mul<T>* mul = FunctionManager<T>::Instance()->mul();

    auto vc1 = getCoreSafely(v1);
    auto vc2 = getCoreSafely(v2);
    auto vc = mul->operator()({vc1, vc2});
    return Variable<T>(vc->id());
}

template <typename T> Variable<T> operator*(Variable<T> v, T n)
{
    auto vc1 = getCoreSafely(v.name());
    auto vc2 = VariableManager<T>::Instance()->get(NArr<T>{n});
    Mul<T>* mul = FunctionManager<T>::Instance()->mul();
    auto vc = mul->operator()({vc1, vc2});
    return Variable<T>(vc->id());
}

template <typename T> Variable<T> operator*(T n, Variable<T> v)
{
    auto vc1 = getCoreSafely(v);
    auto vc2 = VariableManager<T>::Instance()->get(NArr<T>{n});
    Mul<T>* mul = FunctionManager<T>::Instance()->mul();
    auto vc = mul->operator()({vc1,vc2});
    return Variable<T>(vc->id());
}


template <typename T> Variable<T>
operator/(Variable<T> v, T n)
{
    auto vc1 = getCoreSafely(v);
    auto vc2 = VariableManager<T>::Instance()->get(NArr<T>{n});
    Div<T>* div = FunctionManager<T>::Instance()->div();
    auto vc = div->operator()({vc1, vc2});
    return Variable<T>(vc->id());
}


template <typename T>
Variable<T> operator/(T n, Variable<T> arr1)
{
    NArr<T> arr = n/arr1._fwd;
    auto vc = getCoreSafely(arr);
    return Variable<T>(vc);
}


template <typename T> Variable<T>
operator^(Variable<T> v, int n) {
    Pow<T>* pow = FunctionManager<T>::Instance()->pow(n);
    auto vc1 = getCoreSafely(v);
    auto vc = pow->operator()({vc1});
    return Variable<T>(vc->id());
}

}

#endif // VARIABLE_OPERATION_H
