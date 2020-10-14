#ifndef TEST_NARR_H
#define TEST_NARR_H

#include <QDebug>
#include "pdl_core.h"

void test_fancy_index()
{
    qDebug() << " --- fancy index --- ";
    auto a = pdl::NArr<double>::arange(12*5).reshape({3,3,4});

    qDebug() << "a:";
    qDebug() << a.shape().str().c_str();
    qDebug() << a.str().c_str();

    auto b = a[{{0,2,4},{},{}}];
    qDebug() << "b = a[[0,2,4],[],[]]";
    qDebug() << b.shape().str().c_str();
    qDebug() << b.str().c_str();
}

void test_c_()
{
    qDebug() << " --- c_ --- ";
    auto a = pdl::NArr<double>::ones(pdl::Shape(3, 2));
    auto b = pdl::NArr<double>::zeros(pdl::Shape(3, 3));
    auto c = pdl::NArr<double>::c_({a,b});
    qDebug() << c.shape().str().c_str();
    qDebug() << c.str().c_str();
}

void test_meshgrid()
{
    qDebug() << " --- meshgrid --- ";
    auto a = pdl::NArr<double>({0,1,2});
    auto b = pdl::NArr<double>({3,4});
    auto c = pdl::NArr<double>({5,6,7});
    auto d = pdl::NArr<double>({9,10});

    auto axis_arr = pdl::NArr<double>::meshgrid({a,b,c,d});
    for(auto &axis : axis_arr)
    {
        qDebug() << "----------------";
        qDebug() << axis.str().c_str();
    }

    std::vector<pdl::NArr<double>>().swap( axis_arr );
}

void test_sum()
{
    qDebug() << " --- sum --- ";
    auto a = pdl::NArr<double>::arange(120).reshape({2,3,4,5});
    qDebug() <<"a:";
    qDebug() << a.str().c_str();

    qDebug() <<"b = a.sum({1,2},true})";

    auto b = a.sum(std::vector<unsigned>({1,2}), true);
    qDebug() <<"\nb:";
    qDebug() << b.str().c_str();
}

void test_dot()
{
    qDebug() << " --- dot --- ";

    /*
    If both a and b are 1-D arrays, it is inner product of vectors (without complex conjugation).
    If both a and b are 2-D arrays, it is matrix multiplication, but using matmul or a @ b is preferred.
    If either a or b is 0-D (scalar), it is equivalent to multiply and using numpy.multiply(a, b) or a * b is preferred.
    If a is an N-D array and b is a 1-D array, it is a sum product over the last axis of a and b.
    If a is an N-D array and b is an M-D array (where M>=2), it is a sum product over the last axis of a and the second-to-last axis of b:
    */

    //A, B が 1 次元配列の場合は、内積になる
    //A, B が 2 次元配列の場合は、行列積になる
    //A, B のどちらかがスカラの場合は、掛け算となる
    //A が 2 次元配列、B が 1 次元配列の場合

    auto a = pdl::NArr<double>({0,2,3,1,2,2,4,0,1,0,4,1,3,1,4,3,4,4,3,3,3,1,3,1}).reshape({2,3,4});
    auto b = pdl::NArr<double>({2,1,3,4,3,1,4,4,3,0,0,2,4,2,2,1}).reshape({2,4,2});
    auto c = pdl::NArr<double>::dot(a,b);

    qDebug() << c.str().c_str();
}

void test_base_operation_with_broadcast()
{
    qDebug() << " --- base oparation (broadcast) ----";

    //(n, m) 対 スカラ値(単一の値)
    {   pdl::NArr<int> ab;

        //std::vector<int> data = {2,4,6,8,10,12};
        pdl::NArr<int> a = pdl::NArr<int>({2,4,6,8,10,12});//.reshape({2,3});
        a.reshape({2,3});

        qDebug() << "a";
        qDebug() << a.str().c_str();

        qDebug() << "a + 1";
        auto p = a + 1;
        qDebug() << p.str().c_str();

        qDebug() << "a - 1";
        auto s = a - 1;
        qDebug() << s.str().c_str();

        qDebug() << "a * 2";
        auto m = a * 2;
        qDebug() << m.str().c_str();

        qDebug() << "a / 2";
        auto d = a / 2;
        qDebug() << d.str().c_str();
    }

    //(n, m) 対 (n, 1)
    {
        auto a = pdl::NArr<double>({2,4,6,8,10,12});
        a.reshape({2,3});
        auto b = pdl::NArr<double>({10,100});
        b.reshape({2,1});

        qDebug() << "a";
        qDebug() << a.str().c_str();

        qDebug() << "a + b";
        auto p = a + b;
        qDebug() << p.str().c_str();

        qDebug() << "a - b";
        auto s = a - b;
        qDebug() << s.str().c_str();

        qDebug() << "a * b";
        auto m = a * b;
        qDebug() << m.str().c_str();

        qDebug() << "a / b";
        auto d = a / b;
        qDebug() << d.str().c_str();
    }

    //(n, 1) 対 (1, m)
    {
        auto a = pdl::NArr<double>({2,4,6});
        a.reshape({1,3});
        auto b = pdl::NArr<double>({10,100});
        b.reshape({2,1});

        qDebug() << "a";
        qDebug() << a.str().c_str();

        qDebug() << "b";
        qDebug() << b.str().c_str();

        qDebug() << "a + b";
        auto p = a + b;
        qDebug() << p.str().c_str();
    }

    //テンソルの階数が異なる場合
    {
        auto a = pdl::NArr<int>({2,4,6,8,10,12});
        a.reshape({2,3});
        auto b = pdl::NArr<int>({10,100,1000,10000});
        b.reshape({4,1,1});
        auto c = a + b;
        qDebug() << "c.shape";
        qDebug() << c.str().c_str();
    }
}

void test_narr()
{
    qDebug() << "===== test array =====";
    //test_sum();

    auto x =  pdl::NArr<double>({{2,4,6},{8,10,12}});
    auto x_max = x.max(1);

    auto y = x - x_max;
    //y = pdl::NArr<double>::exp(y);

    /*
    auto y_tmp = y.sum(1);
    y /= y.sum(1);
    */
}


#endif
