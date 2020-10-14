QT -= gui

CONFIG += console
CONFIG -= app_bundle


CONFIG += c++17

DEFINES += QT_DEPRECATED_WARNINGS
INCLUDEPATH += ../src

SOURCES += \
    ../src/core/function.cpp \
    ../src/core/variable_core.cpp \
    ../src/core/variable_manager.cpp \
    main.cpp

HEADERS += \
    ../src/core/config.h \
    ../src/core/dataloader.h \
    ../src/core/mnist_reader.h \
    ../src/core/datasets.h \
    ../src/core/function.h \
    ../src/core/function_manager.h \
    ../src/core/function_operator.h \
    ../src/core/narr.h \
    ../src/core/narr_auxiliary.h \
    ../src/core/narr_operation.h \
    ../src/core/optimizer.h \
    ../src/core/variable.h \
    ../src/core/variable_core.h \
    ../src/core/variable_manager.h \
    ../src/core/variable_operation.h \
    ../src/core/layer.h \
    ../src/pdl_core.h \
    ../src/step/step01.h \
    ../src/step/step02.h \
    ../src/step/step03.h \
    ../src/step/step04.h \
    ../src/step/step05.h \
    ../src/step/step06.h \
    #../src/step/step10.h \
    #../src/step/step12.h \
    #../src/step/step13.h \
    #../src/step/step14.h \
    #../src/step/step16.h \
    #../src/step/step20.h \
    #../src/step/step22.h \
    #../src/step/step23.h \
    ../src/step/step28.h \
    ../src/step/step29.h \
    ../src/step/step33.h \
    ../src/step/step34.h \
    ../src/step/step41.h \
    #../src/step/step42.h \
    #../src/step/step43.h \
    #../src/step/step44.h \
    ../src/step/step46.h \
    ../src/step/step47.h \
    ../src/step/step48.h \
    ../src/step/step51.h \
    ../src/util/io.h \
    ../src/util/npy.h \
    ../src/util/svg_writer.h \
    ../src/test/test_narr.h \
    ../src/variable_core_operation.h

