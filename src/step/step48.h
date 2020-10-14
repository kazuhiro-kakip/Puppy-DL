#ifndef STEP48_H
#define STEP48_H

#include "pdl_core.h"
#include "util/svg_writer.h"

void step48()
{
    using namespace  pdl;
    auto max_epoch = 300;
    auto batch_size = 30;
    auto lr = 1.0;

    auto data_size = 300;
    auto max_iter = data_size / batch_size;

    double *data_x;
    double *data_t;

    getSpiral(data_x,data_t);

    auto model = MLP<double>("MLP", {10,3});
    auto optimizer = new Optimizer<double>(&model, lr);

    NArr<double> nx(data_x, data_size, 2);
    NArr<double> nt(data_t, data_size, 1);

    unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();

    std::vector<int> index(data_size);
    for(int i = 0; i < data_size; i++)
        index[i] = i;

    std::cout << "step48 start loop" << '\n';

    for(int epoch = 0; epoch < max_epoch; epoch++)
    {
        shuffle(index.begin(), index.end(), std::default_random_engine(seed));

        for(int i = 0;i < max_iter; i++)
        {
            std::vector<double> batch_index;

            for(int j = 0; j < batch_size; j++)
            {
                auto id = index[i*batch_size + j];
                batch_index.push_back(id);
            }

            auto batch_x = nx[{batch_index}];
            auto batch_t = nt[{batch_index}];

            auto x = variable<double>("x", batch_x);
            auto t = variable<double>("t", batch_t);

            auto y = model(x);

            auto loss = softmax_cross_entropy("sce", y, t);
            model.cleargrads();
            loss.backward();
            optimizer->update();

            std::cout << "loss: " << loss.dataText().c_str() << '\n';
        }
    }

    auto h = 0.05;
    auto x_min = nx[{{}, {0}}].min() - 0.1;
    auto x_max = nx[{{}, {0}}].max() + 0.1;

    auto y_min = nx[{{}, {1}}].min() - 0.1;
    auto y_max = nx[{{}, {1}}].max() + 0.1;

    auto x_area = NArr<double>::arange(x_min, x_max, h);
    auto y_area = NArr<double>::arange(y_min, y_max, h);
    auto grid = NArr<double>::meshgrid({x_area, y_area});
    NArr<double> x_points = grid[0].ravel();
    NArr<double> y_points = grid[1].ravel();
    std::vector<NArr<double>> points{x_points,y_points};
    auto X = NArr<double>::c_(points);
    auto v_grid = variable<double>("grid", X);
    auto predict = model(v_grid);

    svg::Color col[3];
    col[0] = svg::Color(255,100,100);
    col[1] = svg::Color(100,255,100);
    col[2] = svg::Color(100,100,255);

    auto predict_data = predict.data();
    auto predict_cls = predict_data.argmax(1);

    svg::SvgWriter svg(1000, 800);
    auto shape_size = 4;

    for(auto i = 0; i < predict_cls.size(); i++)
    {
        int shape = predict_cls(i);
        auto x = X(2*i + 0);
        auto y = -X(2*i + 1);
        //QPointF pt(x,y);

        switch (shape) {
            case 0:
                svg.addRect(x,y, shape_size, shape_size,  col[shape]);
            break;

            case 1:
                svg.addCircle(x,y, shape_size * 0.5,  col[shape]);
            break;

            case 2:
                svg.addTriangle(x,y, shape_size,  col[shape]);
            break;
        }
    }

    col[0] = svg::Color(255,10,10);
    col[1] = svg::Color(10,255,10);
    col[2] = svg::Color(10,10,255);
    shape_size = 8;

    for(auto i = 0; i < nt.size(); i++)
    {
        int shape = nt(i);
        auto x = nx(2*i + 0);
        auto y = -nx(2*i + 1);

        switch (shape) {
            case 0:
                svg.addRect(x,y, shape_size, shape_size,  col[shape]);
            break;

            case 1:
                svg.addCircle(x,y, shape_size * 0.5,  col[shape]);
            break;

            case 2:
                svg.addTriangle(x,y, shape_size,  col[shape]);
            break;
        }
    }

    std::string file_name("step46.html");
    svg.writeFile(file_name);

    std::string command;

#ifdef linux
    command = "xdg-open ";

#elif _WIN32
    command = "start " ;
#else
    command = "open "; //mac
#endif

    command += file_name;
    system(command.c_str());

    clearVariable<double>();
}

#endif // STEP48_H
