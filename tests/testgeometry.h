#ifndef TESTGEOMETRY_H
#define TESTGEOMETRY_H

#include "autotest.h"


class TestGeometry : public QObject
{
Q_OBJECT
private slots:

    //
    void distance();

};

DECLARE_TEST(TestGeometry)


#endif // TESTGEOMETRY_H
