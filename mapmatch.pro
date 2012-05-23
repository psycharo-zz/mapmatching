TEMPLATE = app
CONFIG += console
CONFIG += qt

CONFIG  += qtestlib

SOURCES += main.cpp \
    graph.cpp \
    input.cpp \
#    learning.cpp \
    rangetree.cpp


INCLUDEPATH += /usr/lib/spatialindex

HEADERS += \
    graph.h \
    input.h \
    learning.h \
    geometry.h \
    rangetree.h \
    util.h \
    spatial.h


LIBS += -lspatialindex

QMAKE_CXXFLAGS += -std=c++0x

# CGAL
#LIBS += -lCGAL
#QMAKE_CXXFLAGS += -frounding-math


