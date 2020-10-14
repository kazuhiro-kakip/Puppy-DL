#ifndef VARIABLE_H
#define VARIABLE_H

#include <string>
#include <iostream>
#include "variable_manager.h"

namespace pdl {

template <typename T>
class Variable
{
private:
    std::string _name = "";
    long _id = 0;

public:

    Variable(){}

    Variable(std::string name){
        setName(name);
    }

    Variable(long id){
        _id = id;
    }

    void setName(std::string name){
        _name = name;
    }

    std::string name()
    {
        return _name;
    }

    void setID(long id)
    {
        _id = id;
    }

    long id()
    {
        return _id;
    }

    void showValue()
    {
        if(_name != "")
            VariableManager<T>::Instance()->showFoward(_name);
        else
            VariableManager<T>::Instance()->showFoward(_id);
    }

    void showGradient()
    {
        if(_name != "")
            VariableManager<T>::Instance()->showFoward(_name);
        else
            VariableManager<T>::Instance()->showGradient(_id);
    }

    std::string dataText()
    {
        if(_name != "")
            return VariableManager<T>::Instance()->valueText(_name);
        else
            return VariableManager<T>::Instance()->valueText(_id);
    }

    std::string gradText()
    {
        if(_name != "")
            return VariableManager<T>::Instance()->gradientText(_name);
        else
            return VariableManager<T>::Instance()->gradientText(_id);
    }

    VariableCore<T>* grad()
    {
        if(_name != "")
            return VariableManager<T>::Instance()->grad(_name);
        else
            return VariableManager<T>::Instance()->grad(_id);
    }

    NArr<T>& data()
    {
        if(_name != "")
            return VariableManager<T>::Instance()->value(_name);
        else
            return VariableManager<T>::Instance()->value(_id);
    }

    void setFoward(NArr<T> &na)
    {
        if(_name != "")
            VariableManager<T>::Instance()->setValue(_name, na);
        else
            VariableManager<T>::Instance()->setValue(_id, na);
    }

    void backward()
    {
        if(_id != 0)
            VariableManager<T>::Instance()->backward(_id);
    }

    void cleargrad()
    {
        if(_name != "")
            VariableManager<T>::Instance()->clearGrad(_name);
        else
            VariableManager<T>::Instance()->clearGrad(_id);
    }

    int length()
    {
        if(_name != "")
            return VariableManager<T>::Instance()->length(_name);
        else
            return VariableManager<T>::Instance()->length(_id);
    }
};

template <typename T>
static Variable<T> variable(const std::string &name, const NArr<T> &arr)
{
    auto vc = VariableManager<T>::Instance()->get(name);
    Variable<T> v;
    v.setName(name);
    v.setID(vc->id());
    vc->_data = arr;
    return v;
}

template <typename T>
static int variableCount()
{
    return VariableManager<T>::Instance()->variableSize();
}

template <typename T>
static void clearVariable()
{
    VariableManager<T>::Instance()->clear();
}



}

#endif // VARIABLE_H
