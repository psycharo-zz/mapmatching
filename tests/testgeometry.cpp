
#include <vector>
#include <set>
#include <QDebug>




#include "geometry.h"
#include "testgeometry.h"

using namespace mmatch;

void TestGeometry::distance()
{
    // max error 10cm
    double maxError = 0.1;

    double expectedDist = 130;
    Node a(47.2964240, -122.2445086);
    Node b(47.2964243, -122.2462307);

    QVERIFY(fabs(mmatch::distance(a, b)-expectedDist) <= maxError);


}


