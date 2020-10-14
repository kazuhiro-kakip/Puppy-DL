#ifndef DATALOADER_H
#define DATALOADER_H

#include "datasets.h"
#include "util/io.h"
#include "config.h"

namespace pdl {

class DataLoader{
private:
    DataSets *_data_sets;
    unsigned _batch_size{0};
    unsigned _max_iterator{0};
    unsigned _iterator{0};
    bool _b_shuffle{false};
    std::vector<int> _index;
public:

    DataLoader(DataSets* dt, int batch_size, bool b_shuffle = true)
    {
        _data_sets = dt;
        _batch_size = batch_size;
        _b_shuffle = b_shuffle;
        _max_iterator = _data_sets->len() / batch_size;
        std::cout << "max iterator:" << _max_iterator << std::endl;
        reset();
    }

    void reset()
    {
        _iterator = 0;

        auto size = _data_sets->len();
        _index.resize(size);
        for(auto i = 0; i < size; i++)
        {
            _index[i] = i;
        }

        if (_b_shuffle)
        {
            unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
            shuffle(_index.begin(), _index.end(), std::default_random_engine(seed));

            /*
            qDebug() << "shuffle index";
            QString text;
            for(int i = 0; i < _index.size(); i++)
            {
                text += QString::number(_index[i]) + ",";
                if(i%10==0)
                {
                    qDebug() << text;
                    text = "";
                }
            }
            qDebug() << text;
            */
        }
    }

    void resetIterator()
    {
        _iterator = 0;
    }

    bool next(NArr<double>& x, NArr<double>& t)
    {
        if(_iterator == _max_iterator)
        {
            return false;
        }

        auto sid = _iterator * _batch_size;

        std::vector<std::vector<double>> id2;


        std::vector<int> id1;
        for(auto i = 0; i < _batch_size; i++)
        {
            id1.push_back(_index[sid + i]);
        }

        if(g_b_debug)
        {
            id1 = loadIntArray("D:\\Temp\\numpy\\dataloaders\\batch_index_" + std::to_string(_iterator) + ".npy");
        }
        /*
        if(_iterator == 0)
        {
            id1 = getDebugIndex();
        }*/

        x = _data_sets->getData(id1);
        t = _data_sets->getLabel(id1);

        if(g_b_debug)
        {
            std::string dir = std::string(NPY_FILE_DIR) + "dataloaders\\";
            auto fname = dir + "batch_index_" + std::to_string(_iterator);
            saveIntArray(fname, id1);

            fname = dir + "t_" + std::to_string(_iterator);
            saveFloatArray(fname, t);

            fname = dir + "/x_" + std::to_string(_iterator);
            saveFloatArray(fname, x);
        }

        _iterator++;

        return true;
    }

    std::vector<int> getDebugIndex()
    {
        std::vector<int> idx = {23529,21671,43871,35823,53024,11247,46907,23961,25805,16967,7924,39683,36264,14874,55212,54087,40125,50485,18092,56804,23585,18589,59921,31761,8742,7773,57944,59345,38893,20811,15475,7028,16812,10736,19878,14646,58561,58989,38466,10652,1993,10423,21379,30557,53009,25022,2955,9918,46925,12939,12124,10986,3325,4254,41606,50617,36876,55960,33452,44270,25548,11187,29626,9120,39795,515,20250,59409,2921,43506,27258,28377,55393,20295,16716,8150,53510,10607,16162,8838,25799,13208,24892,13792,7862,3455,49467,58072,746,18018,26379,3137,37985,36653,10436,11078,56029,54606,16957,44159};
        return idx;
    }
};
}

#endif // DATALOADER_H
