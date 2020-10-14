#ifndef FUNCTION_MANAGER_H
#define FUNCTION_MANAGER_H

#include <string>
#include <vector>
#include <map>

#include "function.h"

namespace pdl {

template <typename T>
class FunctionManager
{
private:
    std::vector<Function<T>*> _list;
    std::map<std::string, Function<T>*> _map;

public:

    ~FunctionManager()
    {
        clear();
    }

    void clear()
    {
        for(auto it = _map.begin(); it != _map.end(); it++)
        {
            delete it->second;
        }
    }

    static FunctionManager* Instance()
    {
        static FunctionManager* instance = new FunctionManager<T>();
        return instance;
    }

    bool clearVariable(std::string &name)
    {
        if(_map.find(name) != _map.end())
        {
            _map[name]->clearVariable();
            return true;
        }
        return false;
    }

    void append(std::string &name, Function<T>* f)
    {
        if (name.empty())
            _list.push_back(f);
        else
            _map[name] = f;
    }

    Add<T>* add(std::string name=""){

        if(clearVariable(name))
        {
            return  static_cast<Add<T>*>(_map[name]);
        }

        auto add = new Add<T>();
        append(name, add);

        return add;
    }


    Sub<T>* sub(std::string name=""){

        if(clearVariable(name))
        {
            return  static_cast<Sub<T>*>(_map[name]);
        }

        auto sub = new Sub<T>();
        append(name, sub);

        return sub;
    }

    Mul<T>* mul(std::string name=""){

        if(clearVariable(name)){
            return  static_cast<Mul<T>*>(_map[name]);
        }

        auto mul = new Mul<T>();
        append(name, mul);

        return mul;
    }

    Div<T>* div(std::string name=""){

        if(clearVariable(name)){
            return  static_cast<Div<T>*>(_map[name]);
        }

        auto div = new Div<T>();
        append(name, div);

        return div;
    }

    Sum<T>* sum(std::string name="") {

        if(clearVariable(name)){
            return  static_cast<Sum<T>*>(_map[name]);
        }

        auto sum = new Sum<T>();
        append(name, sum);

        return sum;
    }

    Pow<T>* pow(int n, std::string name = ""){

        if(clearVariable(name)){
            return  static_cast<Pow<T>*>(_map[name]);
        }

        auto pow = new Pow<T>(n);
        append(name, pow);

        return pow;
    }

    Sin<T>* sin(int n, std::string name = ""){

        if(clearVariable(name)){
            return  static_cast<Sin<T>*>(_map[name]);
        }

        auto f = new Sin<T>(n);
        append(name, f);

        return f;
    }


    Cos<T>* cos(int n, std::string name = ""){

        if(clearVariable(name)){
            return  static_cast<Cos<T>*>(_map[name]);
        }

        auto f = new Cos<T>(n);
        append(name, f);

        return f;
    }


    //
    pdl::Linear<T>* linear(std::string name=""){

        if(clearVariable(name)){
            return  static_cast<pdl::Linear<T>*>(_map[name]);
        }

        auto linear = new pdl::Linear<T>();
        append(name, linear);

        return linear;
    }

    //
    pdl::Sigmoid<T>* sigmoid(std::string name=""){

        if(clearVariable(name)){
            return  static_cast<pdl::Sigmoid<T>*>(_map[name]);
        }

        auto sigmoid = new pdl::Sigmoid<T>();
        append(name, sigmoid);

        return sigmoid;
    }

    //
    MeanSquaredError<T>* mean_squared_error(std::string name=""){

        if(clearVariable(name)){
            return  static_cast<MeanSquaredError<T>*>(_map[name]);
        }

        auto mse = new MeanSquaredError<T>();
        append(name, mse);

        return mse;
    }

    //
    Softmaxld<T>* softmax(std::string name=""){

        if(clearVariable(name)){
            return  static_cast<Softmaxld<T>*>(_map[name]);
        }

        auto sm = new Softmaxld<T>();
        append(name, sm);

        return sm;
    }

    //
    pdl::SoftmaxCrossEntropy<T>* softmax_cross_entropy(std::string name=""){

        if(clearVariable(name)){
            return  static_cast<pdl::SoftmaxCrossEntropy<T>*>(_map[name]);
        }

        auto smce = new pdl::SoftmaxCrossEntropy<T>();
        append(name, smce);

        return smce;
    }

    //
    void info(){
        for(int i = 0; i < _map.size(); i++)
        {
            _map[i]->info();
        }
    }

    //
    std::string label(std::string& name, int no, int type)
    {
        std::string line;
        if(type == 0)
            line = std::to_string(no) +  " [label=\"" + name + "\", color=orange, style=filled]\n";
        else
            line = std::to_string(no) +  " [label=\"" + name + "\", color=lightblue, style=filled, shape=box]\n";

        return  line;
    }

    //
    std::string arrow(int no_src, int no_dst)
    {
        return std::to_string(no_src) + "->" + std::to_string(no_dst) + "\n";
    }

    //
    std::string dot()
    {
        std::string text_label;
        std::string text_arrow;

        int no = 1;
        std::map<void*, int> uniq;
        std::map<int, int> no_map;

        for (const auto& kv : _map)
        {
            auto key = kv.first;

            Function<T>* op = _map[key];
            text_label += label(op->_name, no, 1);
            uniq[op] = no;
            no++;

            for(int j = 0; j < op->_inputs.size(); j++)
            {
                std::string name = "";
                void* ptr = op->_inputs[j];
                if(uniq[ptr] == 0)
                {
                    text_label += label(name, no, 0);
                    uniq[ptr] = no;
                    no++;
                }
            }

            for(int j = 0; j < op->_outputs.size(); j++)
            {
                std::string name = "";
                void* ptr = op->_outputs[j];
                if(uniq[ptr] == 0 )
                {
                    text_label += label(name, no, 0);
                    uniq[ptr] = no;
                    no++;
                }
            }
        }

        for (const auto& kv : _map)
        {
            auto key = kv.first;
            Function<T>* op = _map[key];
            for(int j = 0; j < op->_inputs.size(); j++)
            {
                auto fid = uniq[op];
                auto vid = uniq[op->_inputs[j]];
                text_arrow += arrow(vid, fid);
            }

            for(int j = 0; j < op->_outputs.size(); j++)
            {
                auto fid = uniq[op];
                auto vid = uniq[op->_outputs[j]];
                text_arrow += arrow(fid, vid);
            }
        }

        std::string text = "digraph G {\n" + text_label + text_arrow + "}";
        std::cout << text << "\n";
        return text;
    }
};

template <typename T>
static std::string showGraph()
{
    return FunctionManager<T>::Instance()->dot();
}

template <typename T>
static void clearFunction()
{
    return FunctionManager<T>::Instance()->clear();
}

/*
template <typename T>
static Variable<T> add(Variable<T> &v)
{
    auto add_fc = FunctionManager<T>::Instance()->add("add");
    auto vc = VM->get(v.name());
    auto vc_next = add_fc(vc);

}
*/

#define FM FunctionManager<T>::Instance()

}

#endif // FUNCTION_MANAGER_H
