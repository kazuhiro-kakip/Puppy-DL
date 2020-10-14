#ifndef IO_H
#define IO_H
#include "npy.h"
#include "pdl_core.h"
#include <stdlib.h>
#include <iostream>

static pdl::NArr<double> loadFloatArray(std::string path)
{
    std::vector<unsigned long> shape_data;
    bool fortran_order;
    std::vector<float> data;

    shape_data.clear();
    data.clear();
    npy::LoadArrayFromNumpy(path, shape_data, fortran_order, data);

    pdl::Shape shape;
    for(auto i = 0; i < shape_data.size(); i++)
        shape.appendDimension(shape_data[i]);

    pdl::NArr<double> na(shape);
    for(auto i = 0;i < data.size(); i++)
    {
        na(i) = data[i];
    }
    return na;


    /*qDebug() << "shape: ";
    for (size_t i = 0; i<shape.size(); i++)
      qDebug()  << shape[i] << ", ";

    qDebug() << "data: ";
    for (size_t i = 0; i<data.size(); i++)
      qDebug() << data[i] << ", ";*/
}

static std::vector<int> loadIntArray(std::string path)
{
    std::vector<unsigned long> shape_data;
    bool fortran_order;
    std::vector<int> data;

    shape_data.clear();
    data.clear();
    npy::LoadArrayFromNumpy(path, shape_data, fortran_order, data);

    //pdl::Shape shape;
    //for(auto i = 0; i < shape_data.size(); i++)
    //   shape.appendDimension(shape_data[i]);

    return data;
}

static void saveFloatArray(std::string &file_path, pdl::NArr<double>& na)
{
    std::vector<float> data;
    for(auto i = 0; i < na.size(); i++)
        data.push_back(na(i));

    auto ndim = na.shape().ndim();

    long unsigned *leshape = new long unsigned[ndim];
    auto shape_arr = na.shape().array();

    for(auto i = 0; i < ndim; i++)
        leshape[i] = shape_arr[i];

    npy::SaveArrayAsNumpy(file_path + ".npy" , false, ndim, leshape, data);

    delete [] leshape;
}

static void saveIntArray(std::string &file_path, std::vector<int>& data)
{
    auto ndim = 1;
    long unsigned *leshape = new long unsigned[1];
    leshape[0] = data.size();

    npy::SaveArrayAsNumpy(file_path, false, ndim, leshape, data);

    delete [] leshape;
}

static void spiltFilePath (const std::string& file_path, std::string& dir, std::string & file_name) {
  std::cout << "Splitting: " << file_path << '\n';
  unsigned found = file_path.find_last_of("/\\");
  dir = file_path.substr(0,found);
  file_name = file_path.substr(found+1);
}


template <typename T>
static void saveDotGraph( pdl::VariableCore<T>*vc, const std::string& file_path)
{
    std::string dot = vc->getDotGraph();
    std::cout << dot << std::endl;

    std::ofstream file;
    file.open (file_path);
    file << dot;
    file.close();

    /*
    std::string dir;
    std::string file_name;
    spiltFilePath(file_path, dir, file_name);
    auto extension = ".png";//  # Extension(e.g. png, pdf)
    auto cmd = "dot " + file_path + " -T " + extension + " -o " + "out.png";
    system(cmd.c_str());*/
}

#endif // IO_H
