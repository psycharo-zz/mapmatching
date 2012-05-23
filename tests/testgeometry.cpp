#include <vector>
#include <set>
#include <QDebug>


#include "geometry.h"
#include "testgeometry.h"

using namespace mmatch;

void TestGeometry::distanceWGS84()
{
    // max error 10cm
    double maxError = 0.2;
    double expectedDist;

    expectedDist = 160.67261499605;
    WGS84Node a(47.2966133, -122.2486660);
    WGS84Node b(47.2980286, -122.2482361);
    QVERIFY(fabs(mmatch::distance(a, b)-expectedDist) <= maxError);
    qDebug() << mmatch::distance(a, b) << expectedDist;

    expectedDist = 18.3867374807513;
    a = WGS84Node(47.2964237, -122.2442696);
    b = WGS84Node(47.2964234, -122.2440265);
    QVERIFY(fabs(mmatch::distance(a, b)-expectedDist) <= maxError);
    qDebug() << mmatch::distance(a, b) << expectedDist;

    expectedDist = 56.315912395171;
    a = WGS84Node(47.2402925, -122.5312462);
    b = WGS84Node(47.2407990, -122.5312566);
    QVERIFY(fabs(mmatch::distance(a, b)-expectedDist) <= maxError);
    qDebug() << mmatch::distance(a, b) << expectedDist;

}



void TestGeometry::distanceUTM()
{
    // max error 10cm
    double maxError = 0.2;
    double expectedDist;

    expectedDist = 160.67261499605;
    UTMNode a = mmatch::toUTM(47.2966133, -122.2486660);
    UTMNode b = mmatch::toUTM(47.2980286, -122.2482361);
    QVERIFY(fabs(mmatch::distance(a, b)-expectedDist) <= maxError);
    qDebug() << mmatch::distance(a, b) << expectedDist;

    expectedDist = 18.3867374807513;
    a = mmatch::toUTM(47.2964237, -122.2442696);
    b = mmatch::toUTM(47.2964234, -122.2440265);
    QVERIFY(fabs(mmatch::distance(a, b)-expectedDist) <= maxError);
    qDebug() << mmatch::distance(a, b) << expectedDist;

    expectedDist = 56.315912395171;
    a = mmatch::toUTM(47.2402925, -122.5312462);
    b = mmatch::toUTM(47.2407990, -122.5312566);
    QVERIFY(fabs(mmatch::distance(a, b)-expectedDist) <= maxError);
    qDebug() << mmatch::distance(a, b) << expectedDist;


}


