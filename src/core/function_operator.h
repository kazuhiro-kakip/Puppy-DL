#ifndef FUNCTION_OPERATOR_H
#define FUNCTION_OPERATOR_H

#include "variable.h"
#include "variable_manager.h"
#include "function_manager.h"

namespace  pdl{

template <typename T>
Variable<T> sum(Variable<T> v) {
    return  FM->sum()->operator()({v});
}

template <typename T>
Variable<T> sum(std::string name, Variable<T> v) {
    auto vc = getCoreSafely(v);
    vc  = FM->sum(name)->operator()({vc});
    return Variable<T>(vc->_id);
}

template <typename T>
Variable<T> pow(std::string name, Variable<T> v, int n) {
    auto vc = getCoreSafely(v);
    vc = FM->pow(n, name)->operator()({vc});
    return Variable<T>(vc->_id);
}

template <typename T>
Variable<T> linear(std::string name, Variable<T> x, Variable<T> w, Variable<T> b) {
    auto vc_x =  getCoreSafely(x);
    auto vc_w =  getCoreSafely(w);
    auto vc_b =  getCoreSafely(b);

    auto vc = FM->linear(name)->operator()({vc_x,vc_w,vc_b});
    return Variable<T>(vc->_id);
}

template <typename T>
Variable<T> sigmoid(std::string name, Variable<T> x) {
    auto vc = getCoreSafely(x);
    vc = FM->sigmoid(name)->operator()({vc});
    return Variable<T>(vc->_id);
}

template <typename T>
Variable<T> mean_squared_error(std::string name, Variable<T> y_pred, Variable<T> y) {
    auto vc_yp = getCoreSafely(y_pred);
    auto vc_y = getCoreSafely(y);

    auto vc = FM->mean_squared_error(name)->operator()({vc_yp, vc_y});
    return Variable<T>(vc->_id);
}

template <typename T>
Variable<T> softmax(std::string name, Variable<T> x) {
    auto vc_x = getCoreSafely(x);

    auto vc = FM->softmax(name)->operator()({vc_x});
    return Variable<T>(vc->_id);
}

template <typename T>
Variable<T> softmax_cross_entropy(std::string name, Variable<T> x, Variable<T> t) {
    auto vc_x = getCoreSafely(x);
    auto vc_t = getCoreSafely(t);

    auto vc = FM->softmax_cross_entropy(name)->operator()({vc_x, vc_t});
    return Variable<T>(vc->_id);
}

}


#endif // FUNCTION_OPERATOR_H
