#ifndef GEOMETRY_H
#define GEOMETRY_H


#include <cmath>


namespace mmatch {




class WGS84Node
{
public:
    double lat;
    double lon;

    WGS84Node() {}

    WGS84Node(double _lat, double _long):
        lat(_lat),
        lon(_long)
    {}
};


class UTMNode
{
public:
    double x;
    double y;

    UTMNode() {}

    UTMNode(double _x, double _y):
        x(_x),
        y(_y)
    {}


};



// Earth radius
static const double EARTH_RADIUS = 6371400;

// WGS84 ellipsoid params (c) JCoord
static const double WGS84_MAJ = 6378137.000;
static const double WGS84_MIN = 6356752.3141;
static const double WGS84_ECC = ((WGS84_MAJ * WGS84_MAJ) - (WGS84_MIN * WGS84_MIN)) / (WGS84_MAJ * WGS84_MAJ);




// convert degrees to radians
inline double toRad(double degrees)
{
    return M_PI / 180 * degrees;
}


// convert radians to degrees
inline double toDeg(double rads)
{
    return 180 / M_PI * rads;
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


// get distance between two points in lat/lon format (in degrees)
inline double distance(const WGS84Node &p1, const WGS84Node &p2)
{
    return distance(p1.lat, p1.lon, p2.lat, p2.lon);
}


// get bearing (azimuth) given starting and ending point
inline double bearing(double lat1, double lon1,
                      double lat2, double lon2)
{
    double dLon = toRad(lon2 - lon1);

    lat1 = toRad(lat1);
    lat2 = toRad(lat2);

    double y = sin(dLon) * cos(lat2);
    double x = cos(lat1) * sin(lat2) -
               sin(lat1) * cos(lat2) * cos(dLon);
    return atan2(y, x);
}

// get bearing (azimuth) given starting and ending point
inline double bearing(const WGS84Node &p1, const WGS84Node &p2)
{
    return distance(p1.lat, p1.lon, p2.lat, p2.lon);
}


// get the destination point given the bearing and the distance
inline void destination(double lat1, double lon1, double b, double dist, double &lat2, double &lon2)
{
    lat2 = asin(sin(lat1) * cos(dist / EARTH_RADIUS) +
                cos(lat1) * sin(dist / EARTH_RADIUS) * cos(b));

    lon2 = lon1 + atan2(sin(b) * sin(dist / EARTH_RADIUS) * cos(lat1),
                        cos(dist / EARTH_RADIUS) - sin(lat1) * sin(lat2));
}







// convert latitude and longitude to UTM
// (c) JCoord library
inline void toUTM(double lat, double lon, double &east, double &north)
{
    double UTM_F0 = 0.9996;
    double a = WGS84_MAJ;
    double eSquared = WGS84_ECC;

    double latRad = lat * (M_PI / 180.0);
    double lonRad = lon * (M_PI / 180.0);

    int longitudeZone = (int) floor((lon + 180.0) / 6.0) + 1;

    // Special zone for Norway
    if (lat >= 56.0 && lat < 64.0 && lon >= 3.0 && lon < 12.0)
        longitudeZone = 32;


    // Special zones for Svalbard
    if (lat >= 72.0 && lat < 84.0)
    {
        if (lon >= 0.0 && lon < 9.0)
            longitudeZone = 31;
        else if (lon >= 9.0 && lon < 21.0)
            longitudeZone = 33;
        else if (lon >= 21.0 && lon < 33.0)
            longitudeZone = 35;
        else if (lon >= 33.0 && lon < 42.0)
            longitudeZone = 37;
    }

    double longitudeOrigin = (longitudeZone - 1) * 6 - 180 + 3;
    double longitudeOriginRad = longitudeOrigin * (M_PI / 180.0);

    double ePrimeSquared = (eSquared) / (1 - eSquared);

    double n = a / sqrt(1 - eSquared * sin(latRad) * sin(latRad));
    double t = tan(latRad) * tan(latRad);
    double c = ePrimeSquared * cos(latRad) * cos(latRad);
    double A = cos(latRad) * (lonRad - longitudeOriginRad);

    double M =
        a
            * ((1 - eSquared / 4 - 3 * eSquared * eSquared / 64 - 5 * eSquared
                * eSquared * eSquared / 256)
                * latRad
                - (3 * eSquared / 8 + 3 * eSquared * eSquared / 32 + 45
                    * eSquared * eSquared * eSquared / 1024)
                * sin(2 * latRad)
                + (15 * eSquared * eSquared / 256 + 45 * eSquared * eSquared
                    * eSquared / 1024) * sin(4 * latRad) - (35
                * eSquared * eSquared * eSquared / 3072)
                * sin(6 * latRad));

    east =
        (UTM_F0
            * n
            * (A + (1 - t + c) * pow(A, 3.0) / 6 + (5 - 18 * t + t * t
                + 72 * c - 58 * ePrimeSquared)
                * pow(A, 5.0) / 120) + 500000.0);

    north =
        (UTM_F0 * (M + n
            * tan(latRad)
            * (A * A / 2 + (5 - t + (9 * c) + (4 * c * c)) * pow(A, 4.0)
                / 24 + (61 - (58 * t) + (t * t) + (600 * c) - (330 * ePrimeSquared))
                * pow(A, 6.0) / 720)));

    // Adjust for the southern hemisphere
    if (lat < 0)
        north += 10000000.0;

}


// convert latitude and longitude to UTM
// (c) JCoord library
inline UTMNode toUTM(double lat, double lon)
{
    UTMNode result;
    toUTM(lat, lon, result.x, result.y);
    return result;
}

// convert latitude and longitude to UTM
// (c) JCoord library
inline UTMNode toUTM(const WGS84Node &p)
{
    return toUTM(p.lat, p.lon);
}



// distance between two UTM points
inline double distance(const UTMNode &a, const UTMNode &b)
{
    return sqrt((b.x - a.x) * (b.x - a.x) + (b.y - a.y) * (b.y - a.y));
}




} // namespace mmatch




#endif // GEOMETRY_H
