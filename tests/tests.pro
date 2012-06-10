CONFIG  += qtestlib
CONFIG -= app_bundle

INCLUDEPATH += ../


OBJECTS_DIR = .obj

MOC_DIR = .moc


SOURCES += main.cpp \
           test*.cpp \
            ../graph.cpp \
            ../input.cpp \
            ../learning.cpp \
            ../rangetree.cpp \
            ../frechet.cpp

HEADERS += ./test*.h \
           autotest.h \
           testmatching.h \
                ../frechet.h \
                ../graph.h \
                ../input.h \
                ../learning.h \
                ../geometry.h \
                ../rangetree.h \
                ../util.h \
                ../spatial.h

DEPENDPATH += ./*.h


LIBS += -lspatialindex

QMAKE_CXXFLAGS += -std=c++0x



