QMAKE_CXXFLAGS += -std=c++11
LIBS += -lpthread \
    -lboost_thread \
    -lboost_system


TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += \
    main.cpp

HEADERS += \
    threadpool.h \
    threadpool.hpp

OTHER_FILES += \
    task.txt

