#ifndef GEOMETRY_H
#define GEOMETRY_H


#include <cmath>



/*


#include <CGAL/basic.h>
#include <CGAL/Cartesian.h>
#include <CGAL/squared_distance_2.h>

typedef CGAL::Point_2<CGAL::Cartesian<double> > Point2D;
typedef CGAL::Segment_2<CGAL::Cartesian<double> > Segment;


*/



namespace mmatch {


class Node
{
public:
    double lat;
    double lon;

    Node()
    {}

    Node(double _lat, double _long):
        lat(_lat),
        lon(_long)
    {}


};



// Earth radius
static const double EARTH_RADIUS = 6378e+3;

inline double toRad(double degrees)
{
    return M_PI / 180 * degrees;
}



// get distance between two points in lat/lon format (in degrees)
inline double distance(double lat1, double lon1,
                       double lat2, double lon2)
{
    double dLat = toRad(lat2 - lat1);
    double dLon = toRad(lon2 - lon1);

    lat1 = toRad(lat1);
    lat2 = toRad(lat2);

    dLat = sin(0.5 * dLat);
    dLon = sin(0.5 * dLon);

    double a = dLat * dLat + dLon * dLon * cos(lat1) * cos(lat2);
    double c = 2 * atan2(sqrt(a), sqrt(1 - a));
    return EARTH_RADIUS * c;
}



inline double distance(const Node &p1, const Node &p2)
{
    return distance(p1.lat, p1.lon, p2.lat, p2.lon);
}

}




#endif // GEOMETRY_H
