#include "variable_core.h"
#include "function.h"
#include <set>
//#include <QDebug>

using namespace pdl;
template <typename T>
void pdl::VariableCore<T>::backward()
{
    bool b_high_order_dif = true;

    Function<T>* f = reinterpret_cast<Function<T>*>(this->_creator);

    //auto f = this->_creator;
    if(f == nullptr)
        return;

    if(!this->_b_grad)
    {
        auto shape = this->_data.shape();
        auto na_grad = NArr<T>(shape);
        na_grad.fill(1.0);
        auto id = reinterpret_cast<std::uintptr_t>(this);
        auto label = "grad_" + std::to_string(id);
        _grad = new VariableCore<T>(na_grad);
        _b_grad = true;
    }

    std::vector<void*> funcs;
    addFunction(funcs, f);

    while(funcs.size() != 0)
    {
        f = reinterpret_cast<Function<T>*>(funcs.back());
        funcs.pop_back();

        for(int i = 0; i < f->_outputs.size(); i++)
        {
            //qDebug() << f->_name.c_str();

            auto gys = f->_outputs[i]->_grad;
            //qDebug() << "shape:" << gys.shape().str().c_str();

            auto gxs = f->backward(gys);


            for(int j = 0; j < gxs.size(); j++)
            {
                if(!f->_inputs[j]->_b_grad)
                {
                    f->_inputs[j]->_grad = gxs.begin()[j];
                    f->_inputs[j]->_b_grad = true;
                }
                else
                {
                    if(b_high_order_dif)
                    {
                        auto fadd = new Add<T>();
                        f->_inputs[j]->_grad = fadd->operator()({f->_inputs[j]->_grad, gxs.begin()[j]});
                        //f->_inputs[j]->_grad->_data += gxs.begin()[j]->_data;

                    }
                    else
                    {
                        f->_inputs[j]->_grad->_data += gxs.begin()[j]->_data;
                    }
                }
            }
            for(int i = 0; i < f->_inputs.size(); i++)
            {
                auto x = f->_inputs[i];
                if(x->_creator != nullptr)
                    addFunction(funcs, x->_creator);
            }
        }
    }
}

template<typename T>
pdl::VariableCore<T>::VariableCore(const VariableCore<T> &o)
{
    auto data =  o._data;

    if(!data.isEmpty())
    {
        this->_data = NArr<T>(o._data);
    }

    if(!o._grad->_data.isEmpty())
    {
        _grad = new VariableCore<T>(o._grad->_data);

        //_grad = NArr<T>(o._grad->_data);
    }

    _b_grad = o._b_grad;
    _generation = o._generation;
    //_input = o._input;
    //_output  = o._output;

    _id = o._id;
    setID();
}

template<typename T>
pdl::VariableCore<T>::VariableCore(NArr<T> data)
{
    _data = data;
    setID();
}

template<typename T>
void pdl::VariableCore<T>::setID()
{
    _id = reinterpret_cast<std::uintptr_t>(this);
}

template<typename T>
long pdl::VariableCore<T>::id()
{
    return _id;
}

template<typename T>
int pdl::VariableCore<T>::length()
{
    return _data.size();
}

template<typename T>
void pdl::VariableCore<T>::clearGrad()
{
    _b_grad = false;

    if(_grad != nullptr)
    {
        _grad->_data.fill(0.0);
    }
}

template<typename T>
void pdl::VariableCore<T>::setCreator(void *f)
{
    _creator = f;
    this->_generation = reinterpret_cast<Function<T>*>(f)->_generation + 1;
}

template<typename T>
void pdl::VariableCore<T>::addFunction(std::vector<void*> &q, void *f)
{
    for(int i = 0; i < q.size(); i++)
    {
        if (q[i] == f)
            return;
    }

    if(q.size() == 0)
    {
        q.push_back(f);
    }
    else
    {
        auto g = reinterpret_cast<Function<T>*>(f)->_generation;
        for(int i = 0; i < q.size(); i++)
        {
            if ( g < reinterpret_cast<Function<T>*>(q[i])->_generation)
            {
                q.insert(q.begin() + i, f);
                return;
            }
        }
        q.push_back(f);
    }
}

template<typename T>
void pdl::VariableCore<T>::deepCopy(NArr<T> &na)
{
    _data.deepCopy(na);
}

template<typename T>
void pdl::VariableCore<T>::showData()
{
    auto text = _data.str();
    std::cout << text.c_str() << std::endl;
}

template<typename T>
void pdl::VariableCore<T>::showGrad()
{
    auto text = _grad->_data.str();
    std::cout << text.c_str() << std::endl;
}

template<typename T>
std::string pdl::VariableCore<T>::data()
{
    return _data.str();
}

template<typename T>
std::string pdl::VariableCore<T>::grad()
{
    return _grad->_data.str();
}

template<typename T>
std::string pdl::VariableCore<T>::getDotGraph()
{
    std::string text;
    Function<T>* f = reinterpret_cast<Function<T>*>(this->_creator);
    text = this->getDotFormat();
    std::cout << text.c_str() << std::endl;

    if(f == nullptr)
        return "";

    std::set<void*> funcs;
    funcs.insert(funcs.end(), f);

    while(funcs.size() != 0)
    {
        auto it = funcs.end();
        it--;

        f = reinterpret_cast<Function<T>*>(*it);
        funcs.erase(f);

        text += f->getDotFormat();
        std::cout << f->getDotFormat().c_str() << std::endl;

        for(int i = 0; i < f->_inputs.size(); i++)
        {
            auto x = f->_inputs[i];
            text += x->getDotFormat();
            std::cout << x->getDotFormat().c_str() << std::endl;

            if(x->_creator != nullptr)
                funcs.insert(funcs.end(), x->_creator);
        }
    }

    return "digraph g {\n" + text + "}";
}

template<typename T>
std::string VariableCore<T>::getDotFormat()
{
    std::string text = std::to_string(_id) + " [";
    if(_label != "")
    {
        text += "label=\""+ _label + "\", ";
    }

    text += "color=orange, style=filled]\n";
    return text;
}



template class pdl::VariableCore<double>;
