#include <QCoreApplication>
#include "core/function.h"
#include "core/variable_core.h"
#include "core/narr.h"

#include "step/step04.h"
#include "step/step06.h"
#include "step/step12.h"
#include "step/step29.h"
#include "step/step33.h"
#include "step/step34.h"

#include "step/step47.h"
#include "step/step48.h"
#include "step/step51.h"

#include "util/io.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    /* Higher order differential */
    //step33();

    /* MNIST */
    std::string mnist_dir("D:\\AppData\\Qt\\Mnist");
    step51(mnist_dir);

    return a.exec();
}
