#ifndef OPTIMIZER_H
#define OPTIMIZER_H

#include "layer.h"

namespace pdl {

template <typename T = double>
class Optimizer
{
private:
    Model<T> *_model;
    double _lr;
public:

    Optimizer(Model<T> *model, double lr= 0.2)
    {
        _model = model;
        _lr = lr;
    }

    void update()
    {
        auto params = _model->getParameters();
        for(auto i = 0; i < params.size(); i++)
        {
            auto dp = params[i].grad()->_data * _lr;
            auto p_new = params[i].data() - dp;
            params[i].setFoward(p_new);
        }
    }

    std::string fowardText()
    {
        std::string text;
        auto params = _model->getParameters();
        for(auto i = 0; i < params.size(); i++)
        {
            auto label =  params[i].name();
            text += label + ":" + params[i].dataText() + "\n";
        }
        return text;
    }
};

}

#endif // OPTIMIZER_H
