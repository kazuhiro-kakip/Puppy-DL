#ifndef NARR_H
#define NARR_H

#include <cstdlib>
#include <initializer_list>
#include <iostream>
#include <random>
#include <string>
#include <vector>
#include <map>
#include <math.h>
#include <chrono>

#include <numeric>


#include "narr_auxiliary.h"
#include "config.h"

namespace pdl{

template <typename T = double>
class NArr
{
protected:
    Shape _shape;
    uint32_t _arr_size{0};
    T* _arr {nullptr};

public:

    typedef T*			iterator;
    typedef const T*	const_iterator;

    static long _use_memory;

    NArr() noexcept{}

    NArr(const Shape shape) noexcept :
        _shape(shape),
        _arr_size(shape.size())
      //_arr(new T[_arr_size])
    {
        _arr = newMemory(_arr_size);
    }

    NArr(unsigned row, unsigned col) noexcept :
        _shape({row, col}),
        _arr_size(row*col)
      //_arr(new T[_arr_size])
    {
        _arr = newMemory(_arr_size);
    }

    NArr(unsigned col) noexcept :
        _shape({col}),
        _arr_size(col)
      //_arr(new T[_arr_size])
    {
        _arr = newMemory(_arr_size);
    }

    /* copy constructor */
    NArr(const NArr &na) noexcept :
        _shape(na.shape()),
        _arr_size(na._arr_size)
      //_arr(new T[_arr_size])
    {
        _arr = newMemory(_arr_size);
        std::copy(na.cbegin(), na.cend(), begin());
    }

    /* move constructor */
    NArr(const NArr &&na) noexcept :
        _shape(na._shape),
        _arr_size(na._arr_size)
      //_arr(new T[_arr_size])
    {
        _arr = newMemory(_arr_size);
        std::copy(na.cbegin(), na.cend(), begin());
    }

    NArr(const T* arr, unsigned row, unsigned col) noexcept :
        _shape({row, col}),
        _arr_size(row*col)
      //_arr(new T[_arr_size])
    {
        _arr = newMemory(_arr_size);
        std::copy(arr, arr + _arr_size, begin());
    }


    NArr(const T* arr, unsigned arr_size) noexcept :
        //_shape(arr_size),
        _arr_size(arr_size)
      //_arr(new T[_arr_size])
    {
        _arr = newMemory(_arr_size);
        std::copy(arr, arr + _arr_size, begin());
    }


    NArr(const T* arr, const Shape &shape) noexcept :
        _shape(shape),
        _arr_size(shape.size())
      //_arr(new T[_arr_size])
    {
        _arr = newMemory(_arr_size);
        std::copy(arr, arr + _arr_size, begin());
    }

    explicit NArr(const std::initializer_list<T> &arr) noexcept :
        _shape(static_cast<unsigned>(arr.size())),
        _arr_size(_shape.size())
      //_arr(new T[_arr_size])
    {
        _arr = newMemory(_arr_size);
        std::copy(arr.begin(), arr.end(), begin());
    }

    explicit NArr(const std::initializer_list<T> &arr, const Shape &shape) noexcept :
        _shape(shape),
        _arr_size(shape.size())
      //_arr(new T[_arr_size])
    {
        _arr = newMemory(_arr_size);
        std::copy(arr.begin(), arr.end(), begin());
    }


    NArr(const std::initializer_list<std::initializer_list<T>>& rows) noexcept
    {
        size_t size = 0;
        unsigned col = 0;
        //unsigned row = 0;
        for(auto& row : rows)
        {
            size += row.size();
            if(col == 0)
            {
                col = row.size();
            }
            else if(row.size() != col)
            {
                throw("error: colum size is not constant.");
            }
        }

        //_shape = Shape(rows.size(), col);
        //_arr_size = _shape.size();
        //_arr = newMemory(_shape.size());
        newArray(Shape(rows.size(), col));

        uint32_t row_id = 0;
        for(auto& row : rows)
        {
            auto ptr = _arr + row_id * col;
            std::copy(row.begin(), row.end(), ptr);
            row_id++;
        }
    }

    ~NArr(){
        deleteArray();
    }

    static int getUseMemory()
    {
        return _use_memory;
    }

    void deepCopy(NArr &na)
    {
        if(na._arr_size == this->_arr_size)
        {
            std::copy(na.begin(), na.end(), begin());
        }
        else
        {
            newArray(na._shape);
            std::copy(na.begin(), na.end(), begin());
        }
    }

    bool isEmpty()
    {
        if(_shape.size() == 0)
            return true;
        else
            return false;
    }


    int size() const
    {
        return _arr_size;
    }

    static inline T* newMemory(int size)
    {
        _use_memory += size;
        auto arr = new T[size];
        //qDebug() << QString::number((std::uintptr_t)(arr)) << " : new :" << size ;
        return arr;
    }


    inline void newArray(const Shape &shape)
    {
        deleteArray();
        _shape = shape;
        _arr_size = _shape.size();
        _arr = newMemory(_arr_size);//new T[_arr_size ];

    }

    inline void deleteArray()
    {
        if(_arr != nullptr)
        {
            //qDebug() << QString::number((std::uintptr_t)(_arr)) << " : delete :" << _arr_size;

            delete [] _arr;
            //qDebug() << "delete:" << _arr;
            _arr = nullptr;
            _use_memory -= _arr_size;
            _arr_size = 0;
        }

        _shape.clear();
    }

    /* operator */
    T& operator()(int r, int c) noexcept
    {
        return _arr[r*_shape.column() + c];
    }

    T& operator() (int n) const
    {
        return _arr[n];
    }

    NArr& operator=(const NArr &n)
    {
        if(&n != this && n._arr_size != this->_arr_size)
        {
            newArray(n._shape);
        }
        /*
        else if(n.shape() != this->shape())
        {
            _shape = n._shape;
            _arr_size = n._arr_size();
        }*/

        for(int i = 0; i < _shape.size(); i++)
            _arr[i] = n._arr[i];

        return *this;
    }

    NArr& operator=(T v)
    {
        if(this->_arr_size == 1)
        {
            _arr[0] = v;
            _shape = Shape();
        }
        else
        {
            auto arr = newMemory(1);
            arr[0] = v;
            setArray(arr, 1, Shape());
        }
        return *this;
    }


    /* Fancy Index */
    NArr operator[] (std::vector<std::vector<T>> arr_2d) const
    {
        if(_shape.ndim() < arr_2d.size())
        {
            throw("array size is too large.");
        }

        std::vector<std::vector<int>> id_arr;

        for (auto& arr_1d : arr_2d)
        {
            std::vector<int> ids;
            for (auto& id : arr_1d)
            {
                ids.push_back(id);
            }
            id_arr.push_back(ids);
        }

        Shape shape_new;
        T* arr_new;
        FancyIndex<T> fi = FancyIndex<T>(_shape.array(), id_arr);
        fi.getArray(_arr, arr_new, shape_new);
        //_use_memory += shape_new.size();

        NArr<T> na(arr_new, shape_new);
        delete [] arr_new;

        return na;
    }

    Shape shape() const noexcept
    {
        return _shape;
    }

    NArr& reshape(Shape shape)
    {
        this->_shape = shape;
        return *this;
    }

    void setValue(int r, int c, T v)
    {
        _arr[r*_shape.column() + c] = v;
    }

    void setValue(int n, T v)
    {
        _arr[n] = v;
    }

    void setArray(T* arr, int size, const Shape& shape)
    {
        deleteArray();
        _arr = arr;
        _shape = shape;
        _arr_size = size;
    }

    void fill(T v)
    {
        std::fill(begin(), end(), v);
    }

    void fillZero()
    {
        fill(0);
    }

    static NArr zeros(Shape shape)
    {
        NArr na(shape);
        na.fill(0);
        return na;
    }

    static NArr ones(Shape shape)
    {
        NArr na(shape);
        na.fill(1);
        return na;
    }

    static NArr linespace(T s, T e, int num)
    {
        auto dx = (e - s)/num;

        Shape shape(num);
        NArr na(shape);

        for(auto i = 0; i < num; i++)
        {
            na.begin()[i] = s + i*dx;
        }
        return na;
    }

    NArr max(int axis)
    {
        auto cur_arr_size = _shape.size();

        Shape shape_new;

        std::vector<unsigned> dim_arr = _shape.array();
        auto k_max = 1;
        for(int i = 0; i < axis; i++)
            k_max *= dim_arr[i];

        auto j_max = cur_arr_size/(k_max * dim_arr[axis]);
        auto new_arr_size = cur_arr_size / dim_arr[axis];
        auto grp_step = cur_arr_size / k_max;
        auto new_arr = new T[new_arr_size];
        //qDebug() << QString::number((std::uintptr_t)(arr)) << " : new :" << size ;

        int mid;
        for(auto k = 0; k < k_max; k++)
        {
            for(auto j = 0; j < j_max; j++)
            {
                new_arr[j+k*j_max] = find_max(_arr, j + k*grp_step, j_max, dim_arr[axis], mid);
            }
        }

        shape_new = _shape;
        shape_new.setDimensionSize(axis, 1);

        NArr na(new_arr, shape_new);
        delete [] new_arr;
        return na;
    }

    T max()
    {
        auto max_value = -1.0e+10;
        for(auto i = 0; i < _shape.size(); i++)
        {
            if(max_value < _arr[i])
            {
                max_value = _arr[i];
            }
        }
        return max_value;
    }

    T min()
    {
        auto min_value = 1.0e+10;
        for(auto i = 0; i < _shape.size(); i++)
        {
            if(_arr[i] < min_value)
            {
                min_value = _arr[i];
            }
        }
        return min_value;
    }

    NArr<T> transpose() noexcept
    {
        if (_shape.ndim() == 2)
        {
            NArr<T> arr_t(Shape(_shape._cols, _shape._rows));
            for (int row = 0; row < _shape._rows; ++row)
            {
                for (int col = 0; col < _shape._cols; ++col)
                {
                    //qDebug() << this->operator()(row, col);
                    arr_t(col, row) = operator()(row, col);
                    //qDebug() << row;
                }
            }

            //qDebug() << arr_t.str().c_str();

            return arr_t;
        }
        else
        {
            auto shape_dst = _shape.reverseArray();
            auto shape = _shape.array();

            auto multi_src = 1;
            auto multi_dst = 1;
            std::vector<int> split_src, split_dst;
            std::vector<int> block_size, block_size_dst;

            for(size_t i = 0; i < shape.size(); i++)
            {
                multi_src = multi_src * shape[i];
                split_src.push_back(multi_src);

                multi_dst = multi_dst * shape_dst[i];
                split_dst.push_back(multi_dst);
            }

            auto total_size = multi_src;

            for(size_t i = 0; i < split_src.size(); i++)
            {
                block_size.push_back(total_size/split_src[i]);
                block_size_dst.push_back(total_size/split_dst[i]);
            }

            NArr<T> arr_dst(shape_dst);
            arr_dst.fillZero();

            int *id_src = new int[shape.size()];
            int *id_dst = new int[shape.size()];

            for(int  j = 0; j < total_size; j++)
            {
                int pre = j;
                for(int i = 0; i < block_size.size(); i++)
                {
                    auto ni = pre/block_size[i];
                    pre = pre%block_size[i];
                    id_src[i] = ni;
                }

                int id_1d = 0;
                for(int i = 0; i < shape.size(); i++)
                {
                    auto id = shape.size() - 1 - i;
                    id_dst[i] = id_src[id];
                    id_1d += id_dst[i]*block_size_dst[i];
                }
                arr_dst(id_1d) = _arr[j];
            }

            delete [] id_src;
            delete [] id_dst;

            return arr_dst;
        }
    }

    void addToArray(T *src, T *dst, unsigned src_start, unsigned dst_start, unsigned size)
    {
        for(size_t i = 0; i < size; i++)
        {
            dst[dst_start + i] += src[src_start + i];
        }
    }

    void squeeze(std::vector<unsigned> axis)
    {
        _shape.squeeze(axis);
    }

    void squeeze(unsigned axis)
    {
        _shape.squeeze(axis);
    }

    void squeeze()
    {
        _shape.squeeze();
    }

    NArr sum(std::vector<unsigned> axis, bool keep_dim)
    {
        NArr<T> ret = *this;
        for(int i = 0; i < axis.size(); i++)
        {
            ret = ret.sum(axis[i]);
            if(!keep_dim)
                ret.squeeze();
        }
        return ret;
    }

    NArr sum(int axis = Axis::NONE)
    {
        if(axis == Axis::NONE)
        {
            Shape shp(1,1);
            auto ret_arr = NArr(shp);

            T value = 0;
            for(int i = 0; i < _shape.size(); i++)
                value += _arr[i];

            ret_arr(0,0) = value;
            return ret_arr;
        }
        else
        {
            auto shape_dst = _shape.array();
            //std::vector<int> block_size;
            std::vector<int> split;

            auto mul_shape = 1;
            for(size_t i = 0; i < shape_dst.size(); i++)
            {
                mul_shape = mul_shape * shape_dst[i];
                split.push_back(mul_shape);
            }

            auto total_arr_size = split[split.size()-1];
            auto new_size = total_arr_size/shape_dst[axis];
            auto child_arr_size = total_arr_size / split[axis];

            split.insert(split.begin(), 1);
            auto level_node_num = split[axis];
            auto level_arr_size = total_arr_size / level_node_num;
            unsigned child_num = shape_dst[axis];

#ifdef DEBUG
            {
                qDebug() << "level_node_num:" << level_node_num;
                qDebug() << "level_arr_size:" << level_arr_size;
                qDebug() << "child_num:" << child_num;
                qDebug() << "child_arr_size:" << child_arr_size;
            }
#endif

            shape_dst[axis] = 1;

            NArr<T> arr_dst(shape_dst);
            arr_dst.fillZero();

            for(size_t j = 0; j < level_node_num; j++)
            {
                for(size_t i = 0; i < child_num; i++)
                {
                    addToArray(this->_arr, arr_dst.begin(), child_arr_size*i + level_arr_size*j, j*child_arr_size, child_arr_size);
                }
            }

            return arr_dst;
        }
    }

    NArr logsumexp(int axis=1)
    {
        //qDebug() << "--logsumexp--";
        auto m = this->max(axis);
        auto y = *this - m;
        y = y.exp();
        auto s = y.sum(axis);
        s = s.log();
        m += s;
        return m;
    }

    NArr sumTo(Shape &shape)
    {
        /*
        auto axis = shp.findAxis(1);
        if(0 <= axis)
            return sum(axis);

        else
            return *this;
       */

        auto ndim = shape.ndim();

        auto lead = _shape.ndim() - ndim;
        std::vector<unsigned> lead_axis;
        for(int i = 0; i < lead; i++)
            lead_axis.push_back(i);


        auto arr = shape.array();
        std::vector<unsigned> axis;
        for(int i = 0; i < arr.size(); i++)
        {
            auto sx = arr[i];
            if(sx == 1)
                axis.push_back(i + lead);
        }

        for(int i = 0; i < axis.size(); i++)
            lead_axis.push_back(axis[i]);

        auto y = sum(lead_axis, true);

        if (lead > 0)
            y.squeeze(lead_axis);

        return y;
    }

    NArr& broadcastTo(Shape &shape_dst)
    {
        if(_shape == shape_dst)
            return *this;

        Shape shape_new;
        Shape::broadcast(_shape, shape_dst, shape_new);

        auto arr_size_cur = _shape.size();
        auto arr_size_new = shape_new.size();
        //T *arr_new = new T[arr_size_new];
        T* arr_new = newMemory(arr_size_new);

        auto shape_src = _shape.array();
        auto shape_bc = shape_new.array();
        BroadcastTree<T> bt(shape_src, shape_bc);
        bt.set(_arr, arr_new);

        setArray(arr_new, shape_new.size(), shape_new);
        return *this;
    }

    static NArr dot(NArr &n1, NArr &n2)
    {
        auto d1 = n1._shape.ndim();
        auto d2 = n2._shape.ndim();

        auto col_id = n1.shape().column();
        auto row_id = n2.shape().row();

        if(col_id != row_id)
        {
            throw std::runtime_error("shape is not consistent!");
        }

        if(d1 == 2 && d2 == 2)
        {
            auto row1 = n1.shape().row();
            auto col1 = n1.shape().column();

            auto row2 = n2.shape().row();
            auto col2 = n2.shape().column();

            if(col1 == row2)
            {
                auto shape = Shape(row1, col2);
                NArr na(shape);
                na.fillZero();

                //using namespace std;
                //chrono::system_clock::time_point start, end;
                //start = chrono::system_clock::now();

                auto n2_tsp = n2.transpose();
                //qDebug() << n2_tsp.str().c_str();

                for (auto i = 0; i < row1; ++i)
                {
                    for (auto j = 0; j < col2; ++j)
                    {
                        na(i, j) = std::inner_product(n2_tsp.cbegin(j), n2_tsp.cend(j), n1.cbegin(i), double{ 0 });
                    }
                }

                //end = chrono::system_clock::now();

                //double time = static_cast<double>(chrono::duration_cast<chrono::microseconds>(end - start).count() / 1000.0);
                //qDebug() << "time:" << time;

                return na;
            }

            /*
            auto row = n1.shape()[1];
            auto col = n1.shape()[0];
            for(auto j = 0; j < row; j++)
            {
                auto sum = 0.0;
                for(auto c = 0; c < col; col++)
                {
                    sum += n1(j,c)*n2(c, )
            }*/
        }


        n1._shape.setupBlock();
        n2._shape.setupBlock();

        Shape shape_new;
        //auto size1 = n1.getArraySize();

        for(auto i = 0; i < d1 - 1; i++)
        {
            shape_new.appendDimension(n1.shape().array()[i]);
        }

        //auto size2 = n2.getArraySize();
        for(auto i = 0; i < d2; i++)
        {
            if ( i != d2 - 2)
                shape_new.appendDimension(n2.shape().array()[i]);
        }

        shape_new.setupBlock();
        auto arr_size = shape_new.size();

        //auto buf_size = shape_new.ndim();
        int* mul_id = new int[shape_new.ndim()];
        int* mul_id1 = new int[n1._shape.ndim()];
        int* mul_id2 = new int[n2._shape.ndim()];

        auto arr = new T[arr_size];
        for(auto i = 0; i < arr_size; i++)
        {
            shape_new.indexToMulti(i, mul_id);

            auto id = 0;

            for(auto j = 0; j < d1-1; j++)
            {
                mul_id1[j] = mul_id[id];
                id++;
            }

            for(auto j = 0; j < d2; j++)
            {
                if(j == d2-2)
                    continue;

                mul_id2[j] = mul_id[id];
                id++;

            }

            auto value = 0.0;
            for(auto j = 0; j < col_id; j++)
            {
                mul_id1[d1-1] = j;
                mul_id2[d2-2] = j;

                unsigned id1 = n1._shape.indexTo1D(mul_id1);
                unsigned id2 = n2._shape.indexTo1D(mul_id2);

                value += n1._arr[id1]*n2._arr[id2];
            }

            arr[i] = value;
        }

        delete[] mul_id;
        delete[] mul_id1;
        delete[] mul_id2;

        NArr na(arr, shape_new);
        delete [] arr;
        return na;
    }

    NArr argmax(int axis)
    {
        if(_shape.ndim() < axis)
        {
            throw(std::logic_error("axis is invalid."));
        }

        auto col = _shape.column();
        auto loop = size() / col;

        T* arr = new T[loop];

        int mid;
        for(auto i = 0; i < loop; i++)
        {
            find_max(_arr, col*i, 1, col, mid);
            arr[i] = mid;
        }

        NArr na(arr, loop);
        delete [] arr;
        return na;
    }

    void extendDimension(int n)
    {
        _shape.extendDimension(n);
        _arr_size = _shape.size();
    }

    std::string str_corner9()
    {
        std::string lines[7];
        auto col = _shape._cols;
        auto row = _shape._rows;
        lines[3] = "...";

        for(int j = 0; j < 3; j++)
        {
            std::string first1, second1;
            std::string first2, second2;

            for(int i = 0; i < 3; i++)
            {
                first1 += std::to_string(operator()(j, i)) + ",";
                second1 += "," + std::to_string(operator()(j, col - 4 + i));


                first2 += std::to_string(operator()(row - 4 + j, i)) + ",";
                second2 += "," + std::to_string(operator()(row - 4 + j, col - 4 + i));
            }

            lines[j] = first1 + "..." + second1;
            lines[4 + j] = first2 + "..." + second2;
        }

        std::string text;
        for(auto i = 0; i < 7; i++)
        {
            text += lines[i] + "\n";
        }
        return text;
    }

    std::string str()
    {
        if(1000 < _arr_size)
        {
            return str_corner9();
        }

        std::string text;
        auto col = _shape.column();
        auto dims = _shape.array();
        unsigned count = 0;
        auto shape = _shape.array();
        to_string(0, shape, text, count);
        return text;
    }

    static NArr<T> arange(unsigned N)
    {
        T * arr = new T[N];

        for(auto i = 0; i < N; i++)
            arr[i] = i;

        Shape shape({N});
        NArr na(arr, shape);

        delete [] arr;
        return na;
    }

    static NArr<T> arange(T v0, T v1, T skip)
    {
        unsigned div = (v1-v0)/skip;

        T * arr = new T[div];

        for(auto i = 0; i < div; i++)
            arr[i] = v0 + i*skip;

        Shape shape({div});
        NArr na(arr, shape);
        delete [] arr;
        //na.setArray(arr, shape);
        return na;
    }

    static NArr<T> eye(unsigned N)
    {
        unsigned size = N*N;
        T *arr = new T[size];
        std::fill(arr, arr + size, 0);

        for(auto j = 0; j < N; j++)
        {
            arr[j*N + j] = 1;
        }

        Shape shape({N,N});
        NArr na(arr, shape);

        delete [] arr;
        return na;
    }

    std::vector<T> as_vector()
    {
        std::vector<T> arr;
        auto size = _shape.size();
        for(int i = 0; i < size; i++)
            arr.push_back(_arr[i]);
        return arr;
    }

    static std::vector<NArr<T>> meshgrid(const std::vector<NArr<T>>& axis_arr)
    {
        std::vector<NArr<T>> mg;
        Shape shape_new;

        if(axis_arr.size() == 0)
        {
            return mg;
        }
        else if(axis_arr.size() == 1)
        {
            auto na = axis_arr[0];
            mg.push_back(NArr(na.begin(), Shape(na.size())));
            return mg;
        }
        else
        {
            auto count = 0;
            for(auto& axis : axis_arr)
            {
                if(count == 1)
                {
                    shape_new.insertDimension(0, axis.size());
                }
                else
                {
                    shape_new.appendDimension(axis.size());
                }

                count++;
            }
        }

        auto arr_size = shape_new.size();

        auto block_num = 1;
        for(int i = 0; i < axis_arr.size(); i++)
        {
            block_num *= shape_new.array()[i];
            auto block_size = arr_size / block_num;

            auto dst_arr = new T[arr_size];
            auto src_size = shape_new.array()[i];

            T* src_arr = nullptr;
            NArr<T> axis;
            if( i == 0)
            {
                axis = axis_arr[1];
            }
            else if(i == 1)
            {
                axis = axis_arr[0];
            }
            else
            {
                axis = axis_arr[i];
            }

            src_arr = static_cast<T*>(axis.begin());
            copyArray(src_arr, dst_arr, src_size, block_size, arr_size);

            NArr<T> na(dst_arr, shape_new);
            delete [] dst_arr;
            if(i == 1)
            {
                mg.insert(mg.begin(), na);
            }
            else
            {
                mg.push_back(na);
            }
        }
        return mg;
    }

    NArr<T> ravel()
    {
        return NArr(this->_arr, _shape.size());
    }

    static NArr<T> c_(const std::vector<NArr>& nas)
    {
        if(nas.size() == 0)
        {
            throw::std::logic_error("array size must be not zero.");
        }

        // check shape
        auto arr_size = 0;
        auto row =  nas[0].shape().row();

        if(row == 0)
        {
            row = nas[0].size();
        }

        for(auto i = 1; i < nas.size(); i++)
        {
            auto row_i = nas[i].shape().row();
            if(row_i == 0)
                row_i = nas[i].size();

            if( row_i != row)
            {
                throw::std::logic_error("c_:colum size must be same.");
            }
            arr_size += nas[i].size();
        }

        T* arr = new T[arr_size*nas.size()];
        int count = 0;

        for(auto r = 0; r < row; r++)
        {
            for(auto i = 0; i < nas.size(); i++)
            {
                auto col = nas[i].shape().column();
                if(col == 0)
                    col = 1;

                for(auto c = 0; c < col; c++)
                {
                    arr[count] = nas[i](r*col + c);
                    count++;
                }
            }
        }

        Shape shape(row, count/row);
        NArr na(arr, shape);
        delete [] arr;
        return na;
    }

    iterator begin()
    {
        return _arr;
    }

    iterator end()
    {
        return begin() + _arr_size;
    }

    const_iterator cbegin() const
    {
        return _arr;
    }

    const_iterator cbegin(unsigned inRow) const
    {
        if (inRow >= _shape._rows)
        {
            throw std::runtime_error("input row is greater than the number of rows in the array.");
        }

        return cbegin() + inRow * _shape._cols;
    }


    const_iterator cend() const noexcept
    {
        return cbegin() + _arr_size;
    }

    const_iterator cend(unsigned inRow) const
    {
        if (inRow >=  _shape._rows)
        {
            throw std::runtime_error("input row is greater than the number of rows in the array.");
        }

        return cbegin(inRow) + _shape._cols;
    }

    /* mathmatic function */
    static NArr<T> tanh(NArr<T> x)
    {
        NArr<T> ret = x;
        auto size = x.size();
        for(auto i = 0; i < size; i++)
        {
            ret(i) = std::tanh(ret(i));
        }
        return ret;
    }

    static NArr<T> exp(const NArr<T> x)
    {
        NArr<T> ret = x;
        for(auto i = 0; i < x.size(); i++)
        {
            ret.begin()[i] = std::exp(ret(i));
        }
        return ret;
    }

    NArr exp()
    {
        NArr<T> dst(_shape);
        dst.fillZero();

        for(int i = 0; i < _shape.size(); i++)
        {
            auto value = _arr[i];
            value = std::exp(value);
            dst.begin()[i] = value;
        }
        return dst;
    }

    NArr power(int n)
    {
        NArr<T> dst(_shape);
        dst.fillZero();

        for(int i = 0; i < _shape.size(); i++)
        {
            auto value = _arr[i];
            value = std::pow(value, n);
            dst.begin()[i] = value;
        }
        return dst;
    }

    NArr log()
    {
        NArr<T> dst(_shape);
        dst.fillZero();

        for(int i = 0; i < _shape.size(); i++)
        {
            auto value = _arr[i];
            value = std::log(value);
            dst.begin()[i] = value;
        }
        return dst;
    }

    NArr sin()
    {
        NArr<T> dst(_shape);
        dst.fillZero();

        for(int i = 0; i < _shape.size(); i++)
        {
            auto value = _arr[i];
            value = std::sin(value);
            dst.begin()[i] = value;
        }
        return dst;
    }

    NArr cos()
    {
        NArr<T> dst(_shape);
        dst.fillZero();

        for(int i = 0; i < _shape.size(); i++)
        {
            auto value = _arr[i];
            value = std::cos(value);
            dst.begin()[i] = value;
        }
        return dst;
    }

private:

    inline T find_max(T* arr, int sid, int skip, int loop, int &mid)
    {
        T max_value = -100000.0;
        for(auto i = 0; i < loop; i++)
        {
            auto id = sid + i*skip;

            auto value = arr[id];
            if( max_value < value )
            {
                max_value = value;
                mid = id - sid;
            }
        }
        return max_value;
    }

    void to_string(int n, std::vector<unsigned> &shape, std::string &text, unsigned &count)
    {
        if(text.length() != 0 && text[text.length()-1]==']')
            text += "\n[";
        else
            text += "[";

        if(shape.size() - 1 == n)
        {
            for(int i = 0; i < shape[n]; i++)
            {
                if(i != shape[n] - 1)
                {
                    text += std::to_string(_arr[count]) + " ";
                }
                else
                {
                    text += std::to_string(_arr[count]);
                }
                count++;
            }
        }
        else
        {
            for(int i = 0; i < shape[n]; i++)
            {
                to_string(n+1, shape, text, count);
            }
        }
        text += "]";
    }

    static void copyArray(T* src, T* dst, unsigned src_size, unsigned block_size, unsigned dst_size)
    {
        auto block_size_pre = src_size*block_size;// = 18;
        auto up_size = dst_size/block_size_pre;// = 2;

        /*
        qDebug() << "src_size" << src_size;
        qDebug() << "block_size" << block_size;
        qDebug() << "block_size_pre" << block_size_pre;
        qDebug() << "up_size" << up_size;
        */

        for(auto l = 0; l < up_size; l++)
        {
            for(auto k = 0; k < src_size; k++)
            {
                for(auto j = 0; j < block_size; j++)
                {
                    auto id = j + k*block_size + block_size_pre*l;
                    auto value = src[k];
                    //qDebug() << id << ":" << value;
                    dst[id] = src[k];
                }
            }
        }
    }


};

template <typename T> long NArr<T>::_use_memory = 0;
}

#endif // NARR_H
