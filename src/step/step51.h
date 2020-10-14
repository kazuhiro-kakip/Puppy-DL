#ifndef STEP51_H
#define STEP51_H

#include <iostream>
#include "pdl_core.h"
#include "util/svg_writer.h"

void step51(std::string &mnist_dir)
{
    using namespace pdl;

    auto max_epoch = 5;
    auto batch_size = 100;
    unsigned hidden_size = 1000;
    auto lr = 0.01;


    Mnist *train_sets = new Mnist(mnist_dir, true);
    Mnist *test_sets = new Mnist(mnist_dir, false);

    DataLoader train_loader(train_sets, batch_size);
    DataLoader test_loader(train_sets, batch_size);

    std::vector<unsigned> inout{hidden_size, 10};
    auto model = MLP<double>("MLP", inout);
    auto optimizer = new Optimizer<double>(&model, lr);

    long count = 0;
    for(auto epoch = 0; epoch < max_epoch; epoch++)
    {
        auto sum_loss = 0.0;

        while(true)
        {
            NArr<double> nx, nt;

            if(!train_loader.next(nx, nt))
            {
                break;
            }

            auto x = variable<double>("x", nx);
            auto t = variable<double>("t", nt);
            auto y = model(x);

            if(g_b_debug)
            {
                std::string dir = std::string(NPY_FILE_DIR) + "step51\\";

                auto fname = dir + "x_" + std::to_string(count+1);
                saveFloatArray(fname, nx);

                fname = dir + "t_" + std::to_string(count+1);
                saveFloatArray(fname, nt);

                fname = dir + "y_" + std::to_string(count+1);
                saveFloatArray(fname, y.data());
            }

            auto loss = softmax_cross_entropy("sce", y, t);
            model.cleargrads();
            loss.backward();
            optimizer->update();

            sum_loss += *loss.data().begin() * batch_size;

            std::cout << "no." << count << " loss:" << loss.dataText().c_str() << std::endl;
            count++;
        }
        train_loader.resetIterator();
        count = 0;

		std::cout << "epoch:" << epoch + 1;
        std::cout << "train loss:" << (float)(sum_loss / train_sets->_number_of_data) << std::endl;
    }
    delete train_sets;
    delete test_sets;
}

#endif // STEP51_H
