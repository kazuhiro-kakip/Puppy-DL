#ifndef VARIABLE_MANAGER_H
#define VARIABLE_MANAGER_H

#include <map>
#include <vector>
#include <string>

#include "narr.h"

template  <typename T>
class VariableCore;

namespace  pdl{

template <typename T>
class VariableManager
{
private:
    std::map<std::string, VariableCore<T>*> _label_map;
    std::map<long, VariableCore<T>*> _ptr_map;

public:

    static VariableManager* Instance()
    {
        static VariableManager* instance = new VariableManager<T>();
        return instance;
    }

    ~VariableManager()
    {
       clear();
    }

    void clear();

    int variableSize();

    VariableCore<T>* get(std::string label);

    VariableCore<T>* get(long id);

    VariableCore<T>* get(NArr<T> arr);

    void showFoward(std::string label);

    void showGradient(std::string label);

    void showFoward(long id);

    void showGradient(long id);

    std::string valueText(std::string label);

    std::string valueText(long id);

    std::string gradientText(std::string label);

    std::string gradientText(long id);

    VariableCore<T>* grad(std::string label);

    VariableCore<T>* grad(long id);

    NArr<T>& value(std::string label);

    NArr<T>& value(long id);

    void setValue(std::string label, NArr<T> &na);

    void setValue(long id, NArr<T> &na);

    void backward(std::string label);

    void backward(long id);

    void clearGrad(std::string label);

    void clearGrad(long id);

    int length(std::string label);

    int length(long id);
};

#define VM VariableManager<T>::Instance()

}


#endif // VARIABLE_MANAGER_H
