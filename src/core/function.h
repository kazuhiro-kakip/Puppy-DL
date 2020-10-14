#ifndef FUNCTION_H
#define FUNCTION_H

#include <vector>
#include <string>

#include "narr.h"
#include "narr_operation.h"
#include "config.h"

namespace pdl {

template <typename T>
class VariableCore;

template <typename T>
class Function
{
public:

    std::string _name;
    int _generation = 0;
    std::vector<VariableCore<T>*> _inputs;
    std::vector<VariableCore<T>*> _outputs;

    Function(){}
    ~Function(){}

    virtual std::vector<NArr<T>> foward(std::vector<NArr<T>> &xs) = 0;
    virtual std::vector<VariableCore<T>*> backward(VariableCore<T>* gy) = 0;

    void clearVariable();

    VariableCore<T>* operator()(const std::initializer_list<VariableCore<T>*>& inputs);

    void setGeneration();

    std::string getDotFormat();

    void info();  
};

template <typename T>
class Square : public Function<T>
{
public:
    Square();
    std::vector<NArr<T>> foward(std::vector<NArr<T>> &xs) override;
    std::vector<VariableCore<T>*> backward(VariableCore<T>* gy) override;
};

template <typename T>
class Add : public Function<T>
{
public:

    Add();
    Shape _x0_shape;
    Shape _x1_shape;

    std::vector<NArr<T>> foward(std::vector<NArr<T>> &xs) override;
    std::vector<VariableCore<T>*>  backward(VariableCore<T>* gy) override;
};

template <typename T>
class Sub : public Function<T>
{
public:
    Sub();
    std::vector<NArr<T>> foward(std::vector<NArr<T>> &xs) override;
    std::vector<VariableCore<T>*> backward(VariableCore<T>* gy) override;
};

template <typename T>
class Neg : public Function<T>
{
public:
    Neg();
    std::vector<NArr<T>> foward(std::vector<NArr<T>> &xs)  override;
    std::vector<VariableCore<T>*> backward(VariableCore<T>* gy) override;
};

template <typename T>
class Mul : public Function<T>
{
public:
    Shape _x0_shape;
    Shape _x1_shape;

    Mul();
    std::vector<NArr<T>> foward(std::vector<NArr<T>> &xs) override;
    std::vector<pdl::VariableCore<T>*> backward(pdl::VariableCore<T>* gy) override;
};

template <typename T>
class Div : public Function<T>
{
public:
    Shape _x0_shape;
    Shape _x1_shape;

    Div();
    std::vector<NArr<T>> foward(std::vector<NArr<T>> &xs)  override;
    std::vector<VariableCore<T>*>  backward(VariableCore<T>* gy) override;
};

template <typename T>
class Exp : public Function<T>
{
public:

    Exp();
    std::vector<NArr<T>> foward(std::vector<NArr<T>> &xs) override;
    std::vector<VariableCore<T>*> backward(VariableCore<T>* gy) override;
};

template <typename T>
class Pow : public Function<T>
{
public:
    int _n;
    Pow(int n);
    std::vector<NArr<T>> foward(std::vector<NArr<T>> &xs) override;
    std::vector<VariableCore<T>*>  backward(VariableCore<T>* gy) override;
};

template <typename T>
class Sum : public Function<T>
{
public:
    Axis _axis;
    Shape _x_shape;

    Sum(Axis inAxis = Axis::NONE);
    std::vector<NArr<T>> foward(std::vector<NArr<T>> &x) override;
    std::vector<VariableCore<T>*>  backward(VariableCore<T>* gy) override;
};

template <typename T>
class Linear : public Function<T>
{
public:
    static int _count;
    Linear();
    std::vector<NArr<T>> foward(std::vector<NArr<T>> &xs) override;
    std::vector<VariableCore<T>*> backward(VariableCore<T>* gy) override;
};

template <typename T> int Linear<T>::_count = 0;

template <typename T>
class Sigmoid : public Function<T>
{
public:
    static int _count;

    Sigmoid();
    std::vector<NArr<T>> foward(std::vector<NArr<T>> &xs) override;
    std::vector<VariableCore<T>*> backward(VariableCore<T>* gy) override;
};

template <typename T> int Sigmoid<T>::_count = 0;

template <typename T>
class Reshape : public Function<T>
{
public:
    Shape _shape;
    Shape _x_shape;

    Reshape(Shape shape);
    std::vector<NArr<T>> foward(std::vector<NArr<T>> &x) override;
    std::vector<VariableCore<T>*> backward(VariableCore<T>* gy) override;
};

template <typename T>
class Sin : public Function<T>
{
public:

    Sin();
    std::vector<NArr<T>> foward(std::vector<NArr<T>>& xs) override;
    std::vector<VariableCore<T>*> backward(VariableCore<T>* gy) override;
};

template <typename T>
class Cos : public Function<T>
{
public:

    Cos();
    std::vector<NArr<T>> foward(std::vector<NArr<T>>& xs) override;
    std::vector<VariableCore<T>*> backward(VariableCore<T>* gy) override;
};

template <typename T>
class MeanSquaredError : public Function<T>
{
public:

    MeanSquaredError();

    std::vector<NArr<T>> foward(std::vector<NArr<T>>& xs) override;
    std::vector<VariableCore<T>*> backward(VariableCore<T>* gy) override;
};

template  <typename T>
class Softmaxld : public Function<T>
{
public:

    static int _count;
    Softmaxld();

    std::vector<NArr<T>> foward(std::vector<NArr<T>>& xs) override;
    std::vector<VariableCore<T>*> backward(VariableCore<T>* gy) override;
};

template <typename T>
class SoftmaxCrossEntropy : public Function<T>
{
private:
    Softmaxld<T> _softmax;

public:

    SoftmaxCrossEntropy ();

    std::vector<NArr<T>> foward(std::vector<NArr<T>>& xs) override;
    std::vector<VariableCore<T>*> backward(VariableCore<T>* gy) override;
};

template <typename T> int Softmaxld<T>::_count = 0;
}


#endif // FUNCTION_H
