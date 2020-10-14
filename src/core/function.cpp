#include "function.h"
#include "variable_core.h"
#include "variable_manager.h"
#include "function_manager.h"

using namespace pdl;

template <typename T>
void pdl::Function<T>::clearVariable()
{
    _inputs.clear();
    _outputs.clear();
}

template <typename T>
void pdl::Function<T>::setGeneration()
{
    int g = 0;
    for(int i = 0; i < _inputs.size(); i++)
    {
        if(g < _inputs[i]->_generation)
            g = _inputs[i]->_generation;
    }
    _generation = g;
}

template<typename T>
std::string pdl::Function<T>::getDotFormat()
{
    auto fid = reinterpret_cast<std::uintptr_t>(this);
    std::string text = std::to_string(fid);
    text += "[ label=\"" + _name + "\", color=lightblue, style=filled, shape=box]\n";

    for(auto i = 0; i < this->_inputs.size(); i++)
    {
        auto vid = _inputs[i]->id();
        text += std::to_string(vid) + "->" + std::to_string(fid) + "\n";
    }

    for(auto i = 0; i < this->_outputs.size(); i++)
    {
        auto vid = _outputs[i]->id();
        text += std::to_string(fid) + "->" + std::to_string(vid) + "\n";
    }
    return text;
}

template<typename T>
void pdl::Function<T>::info()
{
    std::cout << "-----function(inputs)-----" << std::endl;
    for(int i = 0; i < _inputs.size(); i++)
    {
        if(_inputs[i]->_label != "")
        {
            std::cout << _inputs[i]->_label << std::endl;
        }
        else
        {
            std::cout << _inputs[i]->_id << std::endl;
        }
    }

    std::cout << "-----function(outputs)-----" << std::endl;
    for(int i = 0; i < _outputs.size(); i++)
    {
        if(_outputs[i]->_label != "")
        {
            std::cout << _outputs[i]->_label << std::endl;
        }
        else
        {
            std::cout << _outputs[i]->_id << std::endl;
        }
    }
}

template <typename T>
pdl::VariableCore<T>* pdl::Function<T>::operator()(const std::initializer_list<pdl::VariableCore<T>*>& inputs)
{
    _inputs.insert(_inputs.begin(), inputs.begin(), inputs.end());

    std::vector<NArr<T>> xs;
    //foreach (auto& v : _inputs)
    for(auto i = 0; i < _inputs.size(); i++){
        auto v = _inputs[i];
        xs.push_back(v->_data);
    }

    setGeneration();

    std::vector<NArr<T>> ys = foward(xs);

	VariableCore<T>* outputs = nullptr;

    for(int i = 0; i < ys.size(); i++)
    {
        auto na = ys.begin()[i];
        auto v = VM->get(na);
        v->setCreator(this);
        _outputs.push_back(v);

        if(i == 0)
            outputs = v;
    }
    return  outputs;
}

template <typename T>
pdl::Square<T>::Square()
{
    this->_name = "square";
}

template<typename T>
std::vector<NArr<T>> pdl::Square<T>::foward(std::vector<NArr<T>> &x)
{
    return {x[0]*x[0]};
}

template <typename T>
std::vector<pdl::VariableCore<T>*> pdl::Square<T>::backward(pdl::VariableCore<T>* gy)
{
    /*
    auto fmul1 = new Mul<T>();
    auto fmul2 = new Mul<T>();

    auto v1 = fmul1->operator()({this->_inputs[0], gy});*/
    auto v1 = FM->mul("")->operator()({this->_inputs[0], gy});

    NArr<T> na({2});
    auto v2 = VM->get(na);

    auto v3 = FM->mul("")->operator()({v1,v2});
    return {v3};
}


template<typename T>
pdl::Mul<T>::Mul()
{
    this->_name = "mul";
}

template<typename T>
std::vector<NArr<T>> Mul<T>::foward(std::vector<NArr<T>> &x)
{
    _x0_shape = x[0].shape();
    _x1_shape = x[1].shape();
    return {x[0]*x[1]};
}

template<typename T>
std::vector<pdl::VariableCore<T>*> Mul<T>::backward(pdl::VariableCore<T> *gy)
{
    auto x0 = this->_inputs[0];
    auto x1 = this->_inputs[1];

    //auto fmul1 = new Mul();
    //auto fmul2 = new Mul();

    auto gx0 = FM->mul("")->operator()({gy, x1});
    auto gx1 = FM->mul("")->operator()({gy, x0});
    //auto gx0 = gy*x1->_data;
    //auto gx1 = gy*x0->_data;

    if(_x0_shape != _x1_shape)
    {
        //maybe error
        //gx0 = gx0.sumTo(_x0_shape);
        //gx1 = gx1.sumTo(_x1_shape);
    }
    return {gx0,gx1};
}


template<typename T>
pdl::Pow<T>::Pow(int n){
    _n = n;
    this->_name = "pow";
}

template<typename T>
std::vector<NArr<T>> Pow<T>::foward(std::vector<NArr<T> > &x)
{
    return {x[0].power(_n)};
}

template<typename T>
std::vector<pdl::VariableCore<T>*> Pow<T>::backward(pdl::VariableCore<T>* gy)
{
    auto x = this->_inputs[0];

    auto fpow = new Pow<T>(_n-1);
    //auto fmul1 = new Mul<T>();
    //auto fmul2 = new Mul<T>();

    auto v1 = fpow->operator()({x});
    auto v2 = FM->mul("")->operator()({v1, gy});
    /* NArr<T> na({2});
    auto v2 = VM->get(na);*/
    NArr<T> na({static_cast<T>(_n)});
    auto v4 = VM->get(na);
    auto v3 = FM->mul("")->operator()({v4, v2});
    return {v3};
}


template<typename T>
Sub<T>::Sub(){ this->_name = "sub"; }

template<typename T>
std::vector<NArr<T> > Sub<T>::foward(std::vector<NArr<T> > &xs)
{
    return {xs[0] - xs[1]};
}

template<typename T>
std::vector<pdl::VariableCore<T> *> Sub<T>::backward(pdl::VariableCore<T> *gy)
{
    auto gx0 = gy;

    auto fneg = new Neg<T>();
    auto gx1 = fneg->operator()({gy});

    auto x0_shape = this->_inputs[0]->_data.shape();
    auto x1_shape = this->_inputs[1]->_data.shape();

    /*maybe error
    if(x0_shape != x1_shape)
    {
        gx0 = gx0.sumTo(x0_shape);
        gx1 = gx1.sumTo(x1_shape);
    }*/
    return {gx0, gx1};
}

template<typename T>
Add<T>::Add()
{
    this->_name = "add";
}

template<typename T>
std::vector<NArr<T>> Add<T>::foward(std::vector<NArr<T> > &xs)
{
    std::vector<NArr<T>> y;

    auto x0 = xs.begin()[0];
    auto x1 = xs.begin()[1];
    _x0_shape = x0.shape();
    _x1_shape = x1.shape();
    return {x0 + x1};
    //return y;
}

template<typename T>
std::vector<pdl::VariableCore<T>*> Add<T>::backward(pdl::VariableCore<T> *gy)
{
    auto gx0(gy);
    auto gx1(gy);

    /*maybe error
    if(_x0_shape != _x1_shape)
    {
        gx0 = gx0.sumTo(_x0_shape);
        gx1 = gx1.sumTo(_x1_shape);
    }
    */

    return {gx0, gx1};
}

template<typename T>
Neg<T>::Neg()
{
    this->_name = "neg";
}

template<typename T>
std::vector<NArr<T>> Neg<T>::foward(std::vector<NArr<T>> &x)
{
    return {-x[0]};
}

template<typename T>
std::vector<pdl::VariableCore<T> *> Neg<T>::backward(pdl::VariableCore<T>* gy)
{
    auto fneg = new Neg<T>();
    auto v1 = fneg->operator()({gy});

    return {v1};
}

template<typename T>
Div<T>::Div()
{
    this->_name = "div";
}

template<typename T>
std::vector<pdl::NArr<T>> Div<T>::foward(std::vector<NArr<T>> &x)
{
    _x0_shape = x[0].shape();
    _x1_shape = x[1].shape();

    return {x[0]/x[1]};
}

template<typename T>
std::vector<pdl::VariableCore<T>*> Div<T>::backward(pdl::VariableCore<T> *gy)
{
    auto x0 = this->_inputs[0];
    auto x1 = this->_inputs[1];

    auto fdiv1 = new Div<T>();
    auto fdiv2 = new Div<T>();

    //auto fmul1 = new Mul<T>();
    //auto fmul2 = new Mul<T>();

    auto gx0 = fdiv1->operator()({gy,x1});
    auto x2 =  FM->mul("")->operator()({x1, x1});
    auto v3 = fdiv2->operator()({x0,x2});
    auto gx1 = FM->mul("")->operator()({gy, v3});
    //auto gx1 = gy*((-x0->_data)/x2);

    /*maybe error
    if(_x0_shape != _x1_shape)
    {
        gx0 = gx0.sumTo(_x0_shape);
        gx1 = gx1.sumTo(_x1_shape);
    }*/

    return {gx0, gx1};
}

template<typename T>
Exp<T>::Exp()
{
    this->_name="exp";
}

template<typename T>
std::vector<NArr<T>> Exp<T>::foward(std::vector<NArr<T> > &x)
{
    return {NArr<T>::exp(x[0])};
}

template<typename T>
std::vector<pdl::VariableCore<T>*> Exp<T>::backward(pdl::VariableCore<T> *gy)
{
    //auto fMul = new Mul<T>();
    auto fExp = new Exp<T>();

    auto gx_exp = fExp->operator()({this->_inputs[0]});
    auto gx = FM->mul("")->operator()({gx_exp, gy});
    return {gx};
}

template<typename T>
Sum<T>::Sum(Axis inAxis){
    _axis = inAxis;
    this->_name = "sum";
}

template<typename T>
std::vector<NArr<T>> Sum<T>::foward(std::vector<NArr<T>> &x)
{
    _x_shape = x[0].shape();
    return {x[0].sum(_axis)};
}

template<typename T>
std::vector<pdl::VariableCore<T>*> Sum<T>::backward(pdl::VariableCore<T> *gy)
{
    NArr<T> gx(_x_shape);
    gx.fill(gy(0,0));
    return {gx};
}

template<typename T>
Linear<T>::Linear()
{
    this->_name = "linear";
}

template<typename T>
std::vector<NArr<T>> Linear<T>::foward(std::vector<NArr<T>> &xarr)
{
    std::string dir = std::string(NPY_FILE_DIR) + "linear\\";

    std::vector<pdl::NArr<T>> ys;
    pdl::NArr<T> y;

    auto x = xarr[0];
    auto w = xarr[1];

    // qDebug() << x.shape().str().c_str();
    //qDebug() << "------ w ------";
    //qDebug() << w.str().c_str();

    y = NArr<T>::dot(x,w);


    //qDebug() << y.str().c_str();

    if( xarr.size() == 3)
    {
        auto b = xarr[2];
        y += b;

        if(g_b_debug)
        {
            auto fname = dir + "b_" + std::to_string(Linear::_count);
            //saveFloatArray(fname, b);
        }
    }
    else
    {
        throw std::runtime_error("argument is not allowed.");
    }

    if(g_b_debug)
    {
        auto fname = dir + "x_" + std::to_string(Linear::_count);
        //saveFloatArray(fname, x);

        fname = dir + "w_" + std::to_string(Linear::_count);
        //saveFloatArray(fname, w);

        fname = dir + "y_" + std::to_string(Linear::_count);
        //saveFloatArray(fname, y);

        Linear::_count++;
    }

    return {y};
}

template<typename T>
std::vector<pdl::VariableCore<T> *> Linear<T>::backward(pdl::VariableCore<T> *gy)
{
    //qDebug() << "-------- Linear backward start ------------";
    auto x = this->_inputs[0]->_data;
    auto w = this->_inputs[1]->_data;

    auto wt = w.transpose();
    auto gx = NArr<T>::dot(gy->_data, wt);

    auto xt = x.transpose();
    auto gw = NArr<T>::dot(xt, gy->_data);

    if(this->_inputs.size() == 3)
    {
		pdl::VariableCore<T>* b = this->_inputs[2];
        auto shape = b->_data.shape();
        auto gb = gy->_data.sumTo(shape);

        //qDebug() << "-------- Linear backward end ------------";
        return {VM->get(gx), VM->get(gw), VM->get(gb)};
    }
    else
    {
        //qDebug() << "-------- Linear backward end ------------";
        return {VM->get(gx), VM->get(gw)};
    }
}

template<typename T>
Sigmoid<T>::Sigmoid(){
    this->_name = "sigmoid";
}

template<typename T>
std::vector<pdl::NArr<T>> Sigmoid<T>::foward(std::vector<NArr<T> > &xs)
{
    auto x = xs[0];
    auto t = NArr<T>::tanh(x * 0.5) * 0.5;
    auto y =  t + 0.5;

    if(g_b_debug)
    {
        std::string dir = std::string(NPY_FILE_DIR) + "sigmoid\\";

        auto fname = dir + "x_" + std::to_string(Sigmoid::_count);
        //saveFloatArray(fname, x);

        fname = dir + "y_" + std::to_string(Sigmoid::_count);
        //saveFloatArray(fname, y);

        Sigmoid::_count++;
    }

    return {y};
}

template<typename T>
std::vector<pdl::VariableCore<T>*> Sigmoid<T>::backward(pdl::VariableCore<T> *gy)
{
    auto y = this->_outputs[0]->_data;//!!!!
    auto gx = gy->_data * y * (1.0 - y);
    return {VM->get(gx)};
}

template<typename T>
Reshape<T>::Reshape(Shape shape){
    _shape = shape;
    this->_name = "reshape";
}

template<typename T>
std::vector<NArr<T>> Reshape<T>::foward(std::vector<NArr<T> > &x)
{
    _x_shape = x[0].shape();
    auto y = x[0].reshape(_shape);
    return {y};
}

template<typename T>
std::vector<pdl::VariableCore<T> *> Reshape<T>::backward(pdl::VariableCore<T> *gy)
{
    NArr<T> gx(gy);
    gx.reshape(_x_shape);
    return {gx};
}

template<typename T>
Sin<T>::Sin(){
    this->_name = "sin";
}

template<typename T>
std::vector<NArr<T>> Sin<T>::foward(std::vector<NArr<T> > &xs)
{
    auto x = xs[0];
    return {x.sin()};
}

template<typename T>
std::vector<pdl::VariableCore<T> *> Sin<T>::backward(pdl::VariableCore<T> *gy)
{
    auto x = this->_inputs[0];

    //auto fmul = new Mul<T>();
    auto fcos = new Cos<T>();
    auto v1 = fcos->operator()({x});
    auto v2 = FM->mul("")->operator()({gy, v1});
    //auto gx = gy * cos(x);
    return {v2};
}

template<typename T>
Cos<T>::Cos(){
    this->_name = "cos";
}

template<typename T>
std::vector<NArr<T> > Cos<T>::foward(std::vector<NArr<T> > &xs)
{
    auto x = xs[0];
    return {x.cos()};
}

template<typename T>
std::vector<pdl::VariableCore<T> *> Cos<T>::backward(pdl::VariableCore<T> *gy)
{
    auto x = this->_inputs[0];

    auto fneg = new Neg<T>();
    auto fsin = new Sin<T>();
    //auto fmul = new Mul<T>();

    auto v1 = fneg->operator()({gy});
    auto v2 = fsin->operator()({x});

    auto gx = FM->mul("")->operator()({v1, v2});// -1.0 * gy * x.sin();
    return {gx};
}

template<typename T>
MeanSquaredError<T>::MeanSquaredError(){
    this->_name = "MeanSquaredError";
}

template<typename T>
std::vector<NArr<T>> MeanSquaredError<T>::foward(std::vector<NArr<T> > &xs)
{
    NArr<T> diff = xs[0] - xs[1];
    NArr<T> diff_sq = diff*diff;
    NArr<T> sum = diff_sq.sum();
    double length = diff.size();
    NArr<T> y = sum / length;

    return {y};
}

template<typename T>
std::vector<pdl::VariableCore<T>*> MeanSquaredError<T>::backward(pdl::VariableCore<T> *gy)
{
    auto x0 = this->_inputs[0]->_data;
    auto x1 = this->_inputs[1]->_data;
    auto diff = x0 - x1;

    auto gy_bc = NArr<T>(diff.shape());
    gy_bc.fill(gy->_data(0,0));
    auto gx0 = gy_bc * diff * (2.0/diff.size());
    auto gx1 = - gx0;
    return {VM->get(gx0), VM->get(gx1)};
}

template<typename T>
Softmaxld<T>::Softmaxld()
{
    this->_name = "Softmaxld";
}

template<typename T>
std::vector<NArr<T> > Softmaxld<T>::foward(std::vector<NArr<T> > &xs)
{
    auto x = xs[0];
    auto x_max = x.max(1);
    auto y = x - x_max;

    y = NArr<T>::exp(y);

    auto y_tmp = y.sum(1);
    y /= y.sum(1);

    if(g_b_debug)
    {
        std::string dir = std::string(NPY_FILE_DIR) + "softmax//";

        auto fname = dir + "_x" + std::to_string(Softmaxld::_count);
        //saveFloatArray(fname, x);


        fname = dir + "_y" + std::to_string(Softmaxld::_count);
        //saveFloatArray(fname, y);

        Softmaxld::_count++;
    }


    return {y};
}

template<typename T>
std::vector<pdl::VariableCore<T> *> Softmaxld<T>::backward(pdl::VariableCore<T> *gy)
{
    auto y = this->_outputs[0]->_data;
    auto gx = y * gy->_data;
    auto sumdx = gx.sum();
    gx -= y * sumdx;

    if(g_b_debug)
    {

        std::cout << "Softmaxld::backward-->" << std::endl;
        std::cout << "y.shape" << std::endl;
        std::cout << y.shape().str().c_str() << std::endl;
        std::cout << "gx" << std::endl;
        std::cout << gx.str().c_str() << std::endl;

    }
    return {VM->get(gx)};
}

template<typename T>
SoftmaxCrossEntropy<T>::SoftmaxCrossEntropy()
{
    this->_name = "SoftmaxCrossEntropy";
}

template<typename T>
std::vector<NArr<T> > SoftmaxCrossEntropy<T>::foward(std::vector<NArr<T> > &xs)
{
    NArr<T> x = xs[0];
    NArr<T> t = xs[1];

    //qDebug() << x.str().c_str();
    //qDebug() << t.str().c_str();

    Shape shape = x.shape();
    auto N = shape.array()[0];

    auto log_z = x.logsumexp(1);

    auto log_p = x - log_z;

    log_p = log_p[{NArr<T>::arange(N).as_vector(), t.as_vector()}];
    NArr<T> log_p_sum = - log_p.sum();
    auto y = log_p_sum/(double)N;

    //qDebug() << y.str().c_str();

    return {y};
}

template<typename T>
std::vector<pdl::VariableCore<T> *> SoftmaxCrossEntropy<T>::backward(pdl::VariableCore<T> *gy)
{
    auto x = this->_inputs[0];
    auto t = this->_inputs[1];

    auto N = x->_data.shape().array()[0];
    auto CLS_NUM  = x->_data.shape().array()[1];

    NArr<T> x_data = x->_data;
    std::vector<NArr<T>>xs;
    xs.push_back(x_data);

    gy->_data *= 1/(double)N;

    auto y = _softmax.foward(xs)[0];

    NArr<T> eye = NArr<T>::eye(CLS_NUM);
    auto t_onehot = eye[{ t->_data.as_vector()}];

    y = (y - t_onehot) * gy->_data;



    return {VM->get(y)};
}

template class pdl::Function<double>;
template class pdl::Square<double>;
template class pdl::Pow<double>;

template class pdl::Add<double>;
template class pdl::Sub<double>;
template class pdl::Mul<double>;
template class pdl::Div<double>;
template class pdl::Exp<double>;
template class pdl::Cos<double>;
template class pdl::Sin<double>;

template class pdl::SoftmaxCrossEntropy<double>;
template class pdl::Softmaxld<double>;
template class pdl::MeanSquaredError<double>;
template class pdl::Linear<double>;
template class pdl::Sigmoid<double>;
