#ifndef NARR_OPERATION_H
#define NARR_OPERATION_H

#include <cassert>
#include <stdexcept>

#include "narr.h"

namespace pdl {

template <typename T>
inline bool preBroadCast(NArr<T> &na1, const NArr<T> &na2, NArr<T> &na2_bc)
{
    Shape shape_bc;
    if(Shape::enableBroadcast(na1.shape(), na2.shape(), shape_bc))
    {
        na1.extendDimension(shape_bc.ndim());

        na2_bc = na2;
        na2_bc.extendDimension(shape_bc.ndim());

        na1.broadcastTo(shape_bc);
        na2_bc.broadcastTo(shape_bc);

        return true;
    }
    else
    {
        return false;
    }
}

template <typename T>
NArr<T>& operator+=(NArr<T> &na1, const NArr<T> &na2)
{
    if(na1.shape() == na2.shape())
    {
        auto size = na1.size();
        for(int i = 0; i < size; i++)
        {
            auto value = na1(i) + na2(i);
            na1.setValue(i, value);
        }
        return na1;
    }
    else
    {
        NArr<T> na2_bc;
        if(preBroadCast(na1, na2, na2_bc))
        {
            na1 += na2_bc;
            return na1;
        }
        else
        {
            throw std::logic_error("could not be broadcast. check shapes.");
        }
    }
}

template <typename T>
NArr<T>& operator+=(NArr<T> &na, T scale)
{
    auto size = na.size();
    for(int i = 0; i < size; i++)
    {
        na(i) += scale;
    }

    return na;
}

template <typename T>
NArr<T>& operator-=(NArr<T> &na1, const NArr<T> &na2)
{
    if(na1.shape() == na2.shape())
    {
        auto arr_size = na1.size();
        for(int i = 0; i < arr_size ; i++)
        {
            auto value = na1(i) - na2(i);
            na1.setValue(i, value);
        }
        return na1;
    }
    else
    {
        NArr<T> na2_bc;
        if(preBroadCast(na1, na2, na2_bc))
        {
            na1 -= na2_bc;

            //qDebug() << QString::number((std::uintptr_t)(na2_bc.begin())) << " : ??? :" << na2_bc.size() ;

            return na1;
        }
        else
        {
            throw std::logic_error("could not be broadcast. check shapes.");
        }
    }
}

template <typename T>
NArr<T>& operator-=(NArr<T> &na, T scale)
{
    auto size = na.size();
    for(int i = 0; i < size; i++)
    {
        na(i) -= scale;
    }

    return na;
}


template<typename T>
NArr<T>& operator*=(NArr<T>& na1, const NArr<T>& na2)
{
    if(na1.shape() == na2.shape())
    {
        auto size = na1.size();
        for(int i = 0; i < size; i++)
            na1.setValue(i, na1(i)*na2(i) );
        return na1;
    }
    else
    {
        NArr<T> na2_bc;
        if(preBroadCast(na1, na2, na2_bc))
        {
            na1 *= na2_bc;
            return na1;
        }
        else
        {
            throw std::logic_error("could not be broadcast. check shapes.");
        }
    }
}

template<typename T>
NArr<T>& operator*=(NArr<T>& na, double scale)
{
    for(int i = 0; i < na.size(); i++)
        na.setValue(i, na(i)*scale);

    return na;
}

template<typename T>
NArr<T>& operator/=(NArr<T>& na1, const NArr<T>& na2)
{
    if(na1.shape() == na2.shape())
    {
        auto size = na1.size();
        for(int i = 0; i < size; i++)
            na1.setValue(i, na1(i)/na2(i));
        return na1;
    }
    else
    {
        NArr<T> na2_bc;
        if(preBroadCast(na1, na2, na2_bc))
        {
            na1 /= na2_bc;
            return na1;
        }
        else
        {
            throw std::logic_error("could not be broadcast. check shapes.");
        }
    }
}

template<typename T>
NArr<T>& operator/=(NArr<T>& na, double scale)
{
    for(int i = 0; i < na.size(); i++)
        na.setValue(i, na(i)/scale);

    return na;
}

template <typename T>
NArr<T> operator+(const NArr<T> &na1, const NArr<T> &na2)
{
    auto ret = NArr<T>(na1);
    return ret += na2;
}

template <typename T>
NArr<T> operator+(const NArr<T> &na, T v)
{
    NArr<T> ret(na);
    return ret += v;
}

template <typename T>
NArr<T> operator+(T v, const NArr<T> &na)
{
    NArr ret(na);
    return ret += v;
}

template <typename T>
NArr<T> operator-(const NArr<T> &na1, const NArr<T> &na2)
{
    NArr<T> ret(na1);
    return ret -= na2;
}

template <typename T>
NArr<T> operator-(NArr<T> &na, T v)
{
    NArr ret(na);
    return ret -= v;
}


template <typename T>
NArr<T> operator-(T scale, NArr<T> &na)
{
    auto retArr = na * (-1.0);
    retArr += scale;
    return retArr;
}

template<typename T>
NArr<T> operator-(const NArr<T>& na) noexcept
{
    auto retArr = NArr<T>(na);
    retArr *= static_cast<T>(-1);
    return retArr;
}


template <typename T>
NArr<T> operator*(const NArr<T> &na1, const NArr<T> &na2)
{
    auto retArr = NArr<T>(na1);
    return retArr *= na2;
}

template <typename T>
NArr<T> operator*(const NArr<T> &na, T scale) noexcept
{
    auto ret_arr = NArr<T>(na);
    return  ret_arr *= scale;
}

template <typename T>
NArr<T> operator*(T scale, const NArr<T> &na) noexcept
{
    auto ret_arr = NArr<T>(na);
    return  ret_arr *= scale;
}

template <typename T>
NArr<T> operator/(const NArr<T> &na1, const NArr<T> &na2)
{
    auto retArr = NArr<T>(na1);
    retArr /= na2;
    return retArr;
}

template <typename T>
NArr<T> operator/(const NArr<T> &na, T scale)
{
    auto ret_arr = NArr<T>(na);
    return  ret_arr /= scale;
}

/*
template <typename T>
NArr<T> operator/(NArr<T> na, T scale)
{
    //auto ret_arr = NArr<T>(na);
    return  na /= scale;
}*/

template <typename T>
NArr<T> operator/(T scale, NArr<T> &na)
{
    auto ret_arr = NArr<T>(na);
    return  ret_arr /= scale;
}

template <typename T>
NArr<T> operator^(NArr<T> &na, double n) {
    auto ret_arr = NArr<T>(na);
    return ret_arr.power(n);
}

template<typename T>
NArr<T> zeros(const Shape& shape)
{
    NArr<T> na(shape);
    na.fill(0);
    return na;
}

namespace random
{
template <typename T>
NArr<T>& rand(const Shape &shape)
{
    NArr<T> arr(shape);
    for(int i = 0; i < arr.size(); i++)
    {
        auto value = std::rand() / double(RAND_MAX);
        arr.setValue(i, value);
    }
    return arr;
}

template <typename T>
NArr<T> randN(const Shape &shape)
{
    std::random_device seed_gen;
    std::default_random_engine engine(seed_gen());

    std::normal_distribution<> dist(0.0, 1.0);

    NArr<T> arr(shape);
    for(std::size_t n = 0; n < arr.size(); n++)
    {
        auto value = dist(engine);
        arr.setValue(n, value);
    }
    return arr;
}
}

}

#endif // NARR_OPERATION_H
