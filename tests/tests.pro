CONFIG  += qtestlib
CONFIG -= app_bundle

INCLUDEPATH += ../



SOURCES += main.cpp \
           test*.cpp \
           ../rangetree.cpp

HEADERS += ../rangetree.h\
           ./test*.h \
           autotest.h


QMAKE_CXXFLAGS += -std=c++0x



