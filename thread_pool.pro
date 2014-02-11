QMAKE_CXXFLAGS += -std=c++11
LIBS += -lpthread

TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += \
    main.cpp \
    threadpool.cpp \
    work_thread.cpp

HEADERS += \
    threadpool.h

