QMAKE_CXXFLAGS += -std=c++11
LIBS += -lpthread \
    -lboost_thread

TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += \
    main.cpp \
    priority_queue.cpp

HEADERS += \
    priority_queue.h

OTHER_FILES += \
    ../task.txt
