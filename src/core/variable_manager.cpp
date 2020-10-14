#include "variable_core.h"
#include "variable_manager.h"

template<typename T>
void pdl::VariableManager<T>::clear()
{
    for(auto const& imap: _label_map)
    {
        delete _label_map[imap.first];
    }
    _label_map.clear();


    for(auto const& imap: _ptr_map)
    {
        delete _ptr_map[imap.first];
    }
    _ptr_map.clear();
}

template<typename T>
int pdl::VariableManager<T>::variableSize()
{
    return _label_map.size(); + _ptr_map.size();
}

template<typename T>
pdl::VariableCore<T>* pdl::VariableManager<T>::get(std::string label)
{
    if(_label_map.find(label) != _label_map.end())
    {
        return _label_map[label];
    }
    auto vc = new VariableCore<T>();
    _label_map[label] = vc;
    vc->_label = label;
    return vc;
}

template<typename T>
pdl::VariableCore<T>* pdl::VariableManager<T>::get(long id)
{
    if(_ptr_map.find(id) != _ptr_map.end())
    {
        return _ptr_map[id];
    }

    auto vc = new VariableCore<T>();
    _ptr_map[id] = vc;
    return vc;
}

template<typename T>
pdl::VariableCore<T> *pdl::VariableManager<T>::get(NArr<T> arr)
{
    auto vc = new VariableCore<T>();
    auto id = reinterpret_cast<std::uintptr_t>(vc);
    //long id = reinterpret_cast <long>(vc);
    _ptr_map[id] = vc;

    vc->_id = id;
    vc->_data = arr;

    return vc;
}

template<typename T>
void pdl::VariableManager<T>::showFoward(std::string label)
{
    auto vc = get(label);
    std::cout << label << ":" << vc->_data.str().c_str() << "\n";
}

template<typename T>
void pdl::VariableManager<T>::showGradient(std::string label)
{
    auto vc = get(label);
    std::cout <<  label << ":" << vc->_grad->_data.str().c_str() << "\n";
}

template<typename T>
void pdl::VariableManager<T>::showFoward(long id)
{
    auto vc = get(id);
    std::cout << id << ":" << vc->_data.str().c_str() << "\n";
}

template<typename T>
void pdl::VariableManager<T>::showGradient(long id)
{
    auto vc = get(id);
    std::cout << id << ":" << vc->_grad->_data.str().c_str() << "\n";
}

template<typename T>
std::string pdl::VariableManager<T>::valueText(std::string label)
{
    auto vc = get(label);
    return vc->_data.str();
}

template<typename T>
std::string pdl::VariableManager<T>::valueText(long id)
{
    auto vc = get(id);
    return vc->_data.str();
}

template<typename T>
std::string pdl::VariableManager<T>::gradientText(std::string label)
{
    auto vc = get(label);
    return vc->_grad->_data.str();
}

template<typename T>
std::string pdl::VariableManager<T>::gradientText(long id)
{
    auto vc = get(id);
    return vc->_grad->_data.str();
}

template<typename T>
pdl::VariableCore<T>* pdl::VariableManager<T>::grad(std::string label)
{
    auto vc = get(label);
    return vc->_grad;
}

template<typename T>
pdl::VariableCore<T>* pdl::VariableManager<T>::grad(long id)
{
    auto vc = get(id);
    return vc->_grad;
}

template<typename T>
pdl::NArr<T> &pdl::VariableManager<T>::value(std::string label)
{
    auto vc = get(label);
    return vc->_data;
}

template<typename T>
pdl::NArr<T> &pdl::VariableManager<T>::value(long id)
{
    auto vc = get(id);
    return vc->_data;
}

template<typename T>
void pdl::VariableManager<T>::setValue(std::string label, NArr<T> &na)
{
    auto vc = get(label);
    vc->deepCopy(na);
}

template<typename T>
void pdl::VariableManager<T>::setValue(long id, NArr<T> &na)
{
    auto vc = get(id);
    vc->deepCopy(na);
}

template<typename T>
void pdl::VariableManager<T>::backward(std::string label)
{
    auto vc = get(label);
    vc->backward();
}

template<typename T>
void pdl::VariableManager<T>::backward(long id)
{
    auto vc = get(id);
    vc->backward();
}

template<typename T>
void pdl::VariableManager<T>::clearGrad(std::string label)
{
    auto vc = get(label);
    vc->clearGrad();
}

template<typename T>
void pdl::VariableManager<T>::clearGrad(long id)
{
    auto vc = get(id);
    vc->clearGrad();
}

template<typename T>
int pdl::VariableManager<T>::length(std::string label)
{
    auto vc = get(label);
    return vc->length();
}

template<typename T>
int pdl::VariableManager<T>::length(long id)
{
    auto vc = get(id);
    return vc->length();
}

template class pdl::VariableManager<double>;
