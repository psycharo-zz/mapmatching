#ifndef TESTGEOMETRY_H
#define TESTGEOMETRY_H

#include "autotest.h"


class TestGeometry : public QObject
{
Q_OBJECT
private slots:

    //
    void distanceWGS84();

    //
    void distanceUTM();

    //
    void distanceCircle();

};

DECLARE_TEST(TestGeometry)


#endif // TESTGEOMETRY_H
