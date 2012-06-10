TEMPLATE = app
CONFIG += console


CONFIG  += qtestlib

OBJECTS_DIR = .obj

SOURCES += main.cpp \
    graph.cpp \
    input.cpp \
    learning.cpp \
    rangetree.cpp \
    frechet.cpp


INCLUDEPATH += /usr/lib/spatialindex

HEADERS += \
    graph.h \
    input.h \
    learning.h \
    geometry.h \
    rangetree.h \
    util.h \
    spatial.h \
    frechet.h


LIBS += -lspatialindex

QMAKE_CXXFLAGS += -std=c++0x

# CGAL
#LIBS += -lCGAL
#QMAKE_CXXFLAGS += -frounding-math


