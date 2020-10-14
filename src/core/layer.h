#ifndef LAYER_H
#define LAYER_H

#include "narr.h"
#include "narr_operation.h"
#include "function_manager.h"
#include "function_operator.h"
#include "util/io.h"

namespace pdl {

template <typename T = double>
class Layer
{
protected:
    size_t _in_size;
    size_t _out_size;
    std::string _name;

public:

    std::vector<Variable<T>> _params;

    Layer(){}

    void getParameters(std::vector<Variable<T>> &params)
    {
        for(int i = 0; i < _params.size(); i++)
            params.push_back(_params[i]);
    }

    virtual Variable<T> operator()(Variable<T> x) = 0;
};


template <typename T = double>
class LinearLayer : public Layer<T>
{
private:
    bool _b_init_weight = false;
    static int _count;

    Variable<T> _w;
    Variable<T> _b;

public:

    LinearLayer(std::string name, size_t out_size)
    {
        this->_name = name;
        this->_out_size = out_size;
    }

    Variable<T> operator()(Variable<T> x)
    {
        if(!_b_init_weight)
        {
            auto x_vc = getCoreSafely(x);

            this->_in_size = x_vc->_data.shape().array()[1];
            Shape shape(this->_in_size, this->_out_size);

            auto rand_nv = random::randN<T>(shape);
            auto cnst = sqrt(1.0/(double)this->_in_size);
            auto nw = rand_nv * cnst;

            _w = variable<T>(this->_name + "w", nw);

            NArr<T> nb(Shape(1, this->_out_size));
            nb.fillZero();

            _b = variable<T>(this->_name + "b", nb);

            this->_params.push_back(_w);
            this->_params.push_back(_b);

            _b_init_weight = true;
        }

        //qDebug() << _w.foward().str().c_str();

        auto y = linear(this->_name, x, _w, _b);
        return y;
    }

    //for debug
    void setWeight(T* arr, Shape &shape)
    {
        auto nw = NArr<T>(arr, shape);
        //_w.setFoward(na);
        _w = variable<T>(this->_name + "w", nw);

        NArr nb(Shape(1, this->_out_size));
        nb.fillZero();

        _b = variable<T>(this->_name + "b", nb);

        this->_params.push_back(_w);
        this->_params.push_back(_b);

        _b_init_weight = true;

        if(g_b_debug)
        {
            std::string dir = std::string(NPY_FILE_DIR) + "layers//";

            auto fname = dir + "init_w_" + std::to_string(LinearLayer::_count);
            saveFloatArray(fname, _w);
            LinearLayer::_count = LinearLayer::_count + 1;

        }
      }

    void setWeight(NArr<T>& nw)
    {
       // auto nw = NArr<T>(arr, shape);
        //_w.setFoward(na);
        _w = variable<T>(this->_name + "w", nw);

        NArr nb(Shape(1, this->_out_size));
        nb.fillZero();

        _b = variable<T>(this->_name + "b", nb);

        this->_params.push_back(_w);
        this->_params.push_back(_b);

        _b_init_weight = true;

        if(g_b_debug)
        {
            std::string dir = std::string(NPY_FILE_DIR) + "layers//";

            auto fname = dir + "init_w_" + std::to_string(LinearLayer::_count);
            saveFloatArray(fname, nw);
            LinearLayer::_count = LinearLayer::_count + 1;

        }
    }
};

template <typename T> int LinearLayer<T>::_count = 0;
//
template <typename T = double>
class SigmoidLayer : public Layer<T>
{
public:
    SigmoidLayer(std::string name)
    {
        this->_name = name;
    }

    Variable<T> operator()(Variable<T> x)
    {
        auto y = sigmoid(this->_name, x);
        return y;
    }
};

template <typename T = double>
class Model
{
public:
    std::vector<Layer<T>*> _layers;
    std::vector<Variable<T>> getParameters()
    {
        std::vector<Variable<T>> params;
        for(auto i = 0; i < _layers.size(); i++)
        {
            _layers[i]->getParameters(params);
        }
        return params;
    }
};

//
template <typename T = double>
class MLP : public Model<T>
{
    std::string _name;

public:

    MLP(std::string name, std::vector<unsigned> size_arr)
    {
        _name = name;

        auto it = size_arr.begin();
        int cnt = 0;
        for(; it!= size_arr.end(); it++)
        {
            auto out_size = *it;
            auto id = std::to_string(cnt+1);

            auto linear_layer = new LinearLayer<T>(name + "_linear_" + id, + out_size);
            this->_layers.push_back(linear_layer);

            if(cnt != size_arr.size()-1)
            {
                auto sigmoid_layer = new SigmoidLayer<T>(name + "_sigmoid_" + id);
                this->_layers.push_back(sigmoid_layer);
            }

            cnt++;
        }
        //setDummy();
    }

    void setDummy()
    {
        auto w1 = loadFloatArray("D:\\Temp\\numpy\\layers\\init_w_0.npy");
        auto w2 = loadFloatArray("D:\\Temp\\numpy\\layers\\init_w_1.npy");

        auto l1 = static_cast<LinearLayer<T>*>(this->_layers[0]);
        auto l2 = static_cast<LinearLayer<T>*>(this->_layers[2]);
        Shape s1({2,10});
        Shape s2({10,3});

        l1->setWeight(w1);
        l2->setWeight(w2);
    }

    Variable<T> operator()(Variable<T> x)
    {
        Variable<T> y = x;

        for(auto i = 0; i < this->_layers.size(); i++)
        {
            y = this->_layers[i]->operator()(y);
        }
        return y;
    }

    void cleargrads()
    {
        auto params = this->getParameters();
        for( auto i = 0; i < params.size(); i++)
            params[i].cleargrad();
    }
};

}

#endif // LAYER_H
