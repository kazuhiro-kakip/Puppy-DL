#ifndef NARR_AUXILIARY_H
#define NARR_AUXILIARY_H

#include <algorithm>
#include <map>
#include <vector>
#include <iostream>

namespace pdl {

enum Axis
{
    NONE = -1,
    ROW,
    COL
};

class Shape
{
public:

    unsigned _rows{ 0 };
	unsigned _cols{ 0 };

    std::vector<unsigned> _arr;
    std::vector<unsigned> _block_arr;

    Shape(){}

    Shape(const Shape& shape)
    {
        _arr = shape.array();
        setColRow();
    }

    explicit Shape(unsigned size) noexcept
    {
        _arr.push_back(size);
        setColRow();
    }

    Shape(unsigned row, unsigned col)
    {
        _arr.push_back(row);
        _arr.push_back(col);
        _cols = col;
        _rows = row;
    }

    Shape(const std::vector<unsigned> &arr)
    {
        std::copy(arr.begin(), arr.end(), back_inserter(_arr));
    }

    Shape(const std::initializer_list<unsigned>& size_arr)
    {
        for (auto& size : size_arr)
        {
            _arr.push_back(size);
        }
    }

    ~Shape(){}

    unsigned size() const
    {
        unsigned mul = 1;
        for(size_t i = 0; i < _arr.size(); i++)
            mul *= _arr[i];

        return mul;
    }

    size_t ndim() const
    {
        return _arr.size();
    }

    bool operator==(const Shape& other)
    {
        if(this->ndim() == other.ndim())
        {
            for(size_t i = 0; i < _arr.size(); i++)
            {
                if (_arr[i] != other._arr[i])
                    return false;
            }
        }
        else
        {
            return false;
        }

        return true;
    }

    bool operator!=(const Shape& other)
    {
        return !(*this == other);
    }


    void setColRow()
    {
        if( 1 <= _arr.size() )
        {
            _cols =  _arr[_arr.size()-1];
        }
        else
        {
            _cols = 0;
        }

        if( 2 <= _arr.size() )
        {
            _rows = _arr[_arr.size()-2];
        }
        else
        {
            _rows = 0;
        }
    }

    unsigned column()
    {
        return _cols;
    }

    unsigned row()
    {
        return _rows;
    }

    void clear()
    {
        _arr.clear();
    }

    void squeeze()
    {
        while(true)
        {
            bool b_deleted = false;
            for(int i = 0; i < _arr.size(); i++)
            {
                if(_arr[i] == 1)
                {
                    _arr.erase(_arr.begin() + i);
                    b_deleted = true;
                    break;
                }
            }

            if(!b_deleted)
                break;
        }
    }

    void squeeze(unsigned axis)
    {
        if(_arr[axis] == 1)
            _arr.erase(_arr.begin() + axis);
        else
            throw::std::runtime_error("squeeze:cannot select an axis to squeeze out which has size not equal to one");
    }

    void squeeze(std::vector<unsigned> axis)
    {
        auto it = axis.rbegin();
        for(; it != axis.rend(); it++)
        {
            squeeze(*it);
        }
    }


    std::string str()
    {
        std::string ret;
        for(int i = 0; i < _arr.size(); i++)
        {
            ret += std::to_string(_arr[i]) + ",";
        }
        return ret;
    }

    std::vector<unsigned> array() const
    {
        return _arr;
    }

    std::vector<unsigned> reverseArray() const
    {
        return std::vector<unsigned> (_arr.rbegin(), _arr.rend());
    }

    int findAxis(unsigned size)
    {
        auto dim = ndim();
        for(unsigned i = 0; i < dim; i++)
            if(size == _arr[i])
                return static_cast<int>(i);

        return -1;
    }

    void extendDimension(int n)
    {
        if(_arr.size() < n)
        {
            auto dif = n - _arr.size();
            for(int i = 0; i < dif; i++)
                _arr.insert(_arr.begin(), 1);
        }
    }

    void setDimensionSize(int n, unsigned v)
    {
        _arr[n] = v;
    }

    void appendDimension(unsigned v)
    {
        _arr.push_back(v);
    }

    void insertDimension(int n, unsigned v)
    {
        _arr.insert(_arr.begin() + n, v);
    }

    void setupBlock()
    {
        _block_arr.clear();
        auto arr_size = size();
        auto mul = 1;

        for(auto i = 0; i < _arr.size(); i++)
        {
            mul *= _arr[i];
            auto block_size = arr_size/mul;
            _block_arr.push_back(block_size);
        }
    }

    void setSizeArray(const std::vector<unsigned>&size_arr)
    {
        _arr.clear();
        for (auto& size : size_arr)
        {
            _arr.push_back(size);
        }
    }

    inline void indexToMulti(unsigned id, int *multi_id)
    {
        unsigned rem = id;
        for(auto i = 0; i < _block_arr.size(); i++)
        {
            multi_id[i] = rem/_block_arr[i];
            rem = rem%_block_arr[i];
        }
    }

    inline unsigned indexTo1D(int* multi_id)
    {
        unsigned id = 0;
        for(auto i = 0; i < _block_arr.size(); i++)
        {
            id += _block_arr[i]*multi_id[i];
        }
        return id;
    }

    static bool enableBroadcast(const Shape &sp1, const Shape &sp2, Shape &sp_new)
    {
        auto size_arr1 = sp1.array();
        auto dim1 = sp1.ndim();

        auto size_arr2 = sp2.array();
        auto dim2 = sp2.ndim();

        if(dim1 < dim2)
        {
            auto dif = dim2 - dim1;
            for(auto i = 0; i < dif; i++)
            {
                size_arr1.insert(size_arr1.begin(),1);
            }
        }
        else if(dim2 < dim1)
        {
            auto dif = dim1 - dim2;
            for(auto i = 0; i < dif; i++)
            {
                size_arr2.insert(size_arr2.begin(),1);
            }
        }

        auto dim = std::max(dim1, dim2);

        for(auto i = 0; i < dim; i++)
        {
            if(size_arr1[i] == size_arr2[i])
            {
                sp_new.appendDimension(size_arr1[i]);
                continue;
            }
            else if(size_arr1[i] == 1)
            {
                sp_new.appendDimension(size_arr2[i]);
                continue;
            }
            else if(size_arr2[i] == 1)
            {
                sp_new.appendDimension(size_arr1[i]);
                continue;
            }
            else
            {
                return false;
            }
        }

        return true;
    }

    static bool broadcast(Shape &shape1, Shape &shape2, Shape &dst)
    {
        auto d1 = shape1.ndim();
        auto d2 = shape2.ndim();

        auto d_max = std::max(d1, d2);
        auto d_min = std::min(d1, d2);

        auto it1 = d1 == d_min ? shape1._arr.rbegin() : shape2._arr.rbegin();
        auto it2 = d2 == d_max ? shape2._arr.rbegin() : shape1._arr.rbegin();

        std::vector<unsigned> size_arr;
        size_arr.resize(d_max);
        auto it_new = size_arr.rbegin();

        for(unsigned i = 0; i < d_min; i++)
        {
            if(*it1 == *it2)
            {
                *it_new = *it1;
            }
            else if(*it1 == 1)
            {
                *it_new = *it2;
            }

            else if(*it2 == 1)
            {
                *it_new = *it1;
            }
            it1++;
            it2++;
            it_new++;
        }

        auto diff = d_max - d_min;
        for(unsigned i = 0; i < diff; i++)
        {
            *it_new = *it2;
        }

        dst._arr = size_arr;
        return true;
    }
};

template <typename T = double>
class BroadcastTree
{
public:
    std::vector<std::vector<unsigned>> _src;
    std::vector<std::vector<unsigned>> _dst;
    std::vector<unsigned> _arr;

    std::map<unsigned, std::vector<unsigned>> _u2d;
    std::map<unsigned, unsigned> _arr_map;

    int _unq_id = 0;

    BroadcastTree(){}

    BroadcastTree(std::vector<unsigned>& src_shape, std::vector<unsigned>& dst_shape)
    {
        if(src_shape.size() == dst_shape.size())
        {
            for(auto i = 0; i < src_shape.size(); i++)
                addSourceLevel(src_shape[i]);

            for(auto i = 0; i < dst_shape.size(); i++)
                addDistinationLevel(dst_shape[i]);

            createArrayMap();
        }
    }

    void addSourceLevel(unsigned size)
    {
        if(_unq_id == 0)
        {
            std::vector<unsigned> lv0;
            for(unsigned i = 0; i < size; i++)
            {
                lv0.push_back(_unq_id);
                _unq_id++;
            }
            _src.push_back(lv0);
        }
        else
        {
            std::vector<unsigned> ids;

            auto up = _src.size()-1;
            for(auto i = 0; i < _src[up].size(); i++)
            {
                int pid = _src[up][i];

                std::vector<unsigned> uids;
                for(unsigned j = 0; j < size; j++)
                {
                    uids.push_back(_unq_id);
                    ids.push_back(_unq_id);
                    _unq_id++;
                }
                _u2d[pid] = uids;
            }

            _src.push_back(ids);
        }
    }

    void addDistinationLevel(unsigned size)
    {
        auto lv = _dst.size();

        if(lv == 0)
        {
            std::vector<unsigned> lv0;
            if(_src[0].size() == size)
            {
                for(auto i = 0; i < size; i++)
                {
                    auto id = _src[0][i];
                    lv0.push_back(id);
                }
            }
            else
            {
                for(auto i = 0; i < size; i++)
                {
                    auto id = _src[0][0];
                    lv0.push_back(id);
                }
            }
            _dst.push_back(lv0);
        }
        else
        {
            std::vector<unsigned> level;
            for(auto j = 0; j < _dst[lv-1].size(); j++)
            {
                auto uid_p = _dst[lv-1][j];
                auto ids = _u2d[uid_p];

                if(size != ids.size())
                {
                    for(unsigned i = 0; i < size; i++)
                    {
                        auto uid_c = ids[0];
                        level.push_back(uid_c);
                    }
                }
                else
                {
                    for(unsigned i = 0; i < size; i++)
                    {
                        auto uid_c = ids[i];
                        level.push_back(uid_c);
                    }
                }

            }
            _dst.push_back(level);
        }
    }

    void createArrayMap()
    {
        size_t  last_lv_id = _src.size()-1;
        for(auto i = 0; i < _src[last_lv_id].size(); i++)
        {
            auto uid = _src[last_lv_id][i];
            _arr_map[uid] = i;
        }

        for(auto i = 0; i < _dst[last_lv_id].size(); i++)
        {
            auto uid = _dst[last_lv_id][i];
            auto arr_id = _arr_map[uid];
            _arr.push_back(arr_id);
        }
    }

    void set(T* src, T* dst)
    {
        for(auto i = 0;i < _arr.size(); i++)
        {
            int id = _arr[i];
            dst[i] = src[id];
        }
    }
};

template <typename T = double>
class FancyIndex
{
public:
    std::vector<std::vector<int>> _id_chain;
    std::vector<std::vector<int>> _ids_mod;
    std::vector<unsigned> _shape_org;
    int _arr_size = 1;
    std::vector<unsigned> _shape_new;

    FancyIndex(std::vector<unsigned> shape, std::vector<std::vector<int>> ids)
    {
        std::vector<int> dim_arr;
        std::vector<bool> colon_arr;
        auto uniq_size = -1;

        _shape_org = shape;

        //dim_arrの作成
        //dim_arr:
        //許容される個数は1,もしくは、共通でshape以下のユニークな数字
        //i番目が「:」の場合は、shape[i]の数が入る
        //shape_newは「:」の場合、元のshapeの要素数が採用され、ユニークな数の場合、最初に出てきた数が採用される

        //colon_arr:配列が「:」ならtrue、それ以外はfalse
        //_ids_mod:次の工程で使用する。「:」の場合は、shapeの要素数だけゼロから詰める

        for(int i = 0; i < shape.size(); i++)
        {
            if( i < ids.size() )
            {
                int dim_size = ids[i].size();
                if(dim_size == 0)
                {
                    dim_arr.push_back(shape[i]);
                    colon_arr.push_back(true);
                    setFullID(_ids_mod, shape[i]);
                }
                else
                {
                    if( uniq_size < 0 )
                    {
                        uniq_size = dim_size;
                    }
                    else if(uniq_size != dim_size)
                    {
                        std::cout << "size error" << std::endl;
                        return;
                    }

                    dim_arr.push_back(dim_size);
                    colon_arr.push_back(false);
                    _ids_mod.push_back(ids[i]);
                }
            }
            else
            {
                dim_arr.push_back(shape[i]);
                colon_arr.push_back(true);

                std::vector<int> elm_id_arr;
                setFullID(_ids_mod, shape[i]);
            }
        }

        bool b_once = true;

        //shape_newを算出する
        //i番目が「:」の場合、その要素数を採用する
        //ユニーク数の場合は、最初に出てきた数字のみを採用し、以後はスキップする

        for(int i = 0; i < shape.size(); i++)
        {
            if(colon_arr[i])
            {
                _shape_new.push_back(dim_arr[i]);
                _arr_size *= dim_arr[i];
            }
            else if(dim_arr[i] == uniq_size && b_once)
            {
                b_once = false;
                _shape_new.push_back(uniq_size);
                _arr_size *= uniq_size;
            }
        }

        _id_chain = std::vector<std::vector<int>> (_arr_size, std::vector<int>(_ids_mod.size()));

        // shape_newから配列のサイズarr_sizeが求める
        // arr_size × 元のshapeのサイズの２次元配列を作成し、
        // 上の要素かidを詰めていく
        // i番目が「:」の場合,id = 0,1,2,3,0,1,2,3...となり
        // 「:」ではない場合、id = 0,0,0,0,1,1,1,1...と詰めていく
        for(int i = 0; i < shape.size(); i++)
        {
            for(int j = 0; j < _arr_size; j++)
            {
                if(colon_arr[i])
                {
                    _id_chain[j][i] = j % _ids_mod[i].size();
                }
                else
                {
                    auto up_count = _arr_size / _ids_mod[i].size();
                    auto elm_id = j / up_count;
                    _id_chain[j][i] = _ids_mod[i][elm_id];
                }
            }
        }
    }

    void setFullID(std::vector<std::vector<int>> &ids_mod, int size)
    {
        std::vector<int> arr;
        for(int i = 0; i < size; i++)
            arr.push_back(i);

        ids_mod.push_back(arr);
    }

    void getArray(T* arr_ref, T*& arr_new, Shape &shape_new)
    {
        std::vector<int> ofs_arr;

        int arr_size_org = 1;
        for(int i = 0; i < _shape_org.size(); i++)
        {
            arr_size_org *= _shape_org[i];
        }

        auto elm_size = arr_size_org;
        for(int i = 0; i < _shape_org.size(); i++)
        {
            elm_size /= _shape_org[i];
            ofs_arr.push_back(elm_size);
        }

        auto arr_size_new = _id_chain.size();
        arr_new = new T[arr_size_new];
        auto count = 0;

        for(auto i = 0; i < _id_chain.size(); i++)
        {
            auto id = 0;
            for(auto j = 0; j < _id_chain[i].size(); j++ )
            {
                id += ofs_arr[j] * _id_chain[i][j];
            }
            arr_new[count] = arr_ref[id];
            count++;

        }

        shape_new.setSizeArray(_shape_new);
       //shape_new._dim_size_arr = _shape_new;
    }
};

}
#endif // NARR_AUXILIARY_H
