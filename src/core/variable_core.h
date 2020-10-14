#ifndef VARIABLE_CORE_H
#define VARIABLE_CORE_H

#include <string>
#include <vector>
#include "narr.h"

template <typename T>
class Function;

namespace pdl {


template <typename T>
class VariableCore
{
private:
    void* _creator = nullptr;

public:
    long _id{0};
    std::string _label;
    NArr<T> _data;
    VariableCore<T> *_grad {nullptr};

    bool _b_grad = false;
    int _generation = 0;

    void backward();

    VariableCore(){
        setID();
    }

    VariableCore(const VariableCore<T> &o);

    VariableCore(NArr<T> data);

    void setID();

    long id();

    int length();

    void clearGrad();

    void setCreator(void* f);

    void addFunction(std::vector<void*> &q, void* f);

    void deepCopy(NArr<T> &na);

    void showData();

    void showGrad();

    std::string data();

    std::string grad();

    std::string getDotGraph();

    std::string getDotFormat();
};

}

#endif // VARIABLE_CORE_H
