#ifndef SPATIAL_H
#define SPATIAL_H


#include "geometry.h"

#include <spatialindex/SpatialIndex.h>
using namespace SpatialIndex;


const int32_t CAPACITY = 100;
const RTree::RTreeVariant TREE_VARIANT = RTree::RV_RSTAR;
const float FILL_FACTOR = 0.7;
const int32_t PAGE_SIZE = 1024;

// the default number of nearest neighbors to search for
const int32_t NN_NUMBER = 5;


// macroses for extracting / setting identifiers
#define SHAPE_ID(edgeID, geomID) ((edgeID << 32) | geomID)
#define EDGE_ID(id) (id >> 32)
#define GEOM_ID(id) (id & 0xffffffff)



class MapPoint : public Point
{
public:

    MapPoint(const double *coords):
        Point(coords, 2)
    {}

    double getMinimumDistance(const IShape &s) const
    {
        const MapPoint *mpt = dynamic_cast<const MapPoint*>(&s);
        if (mpt != 0) return getMinimumDistance(*mpt);

        // TODO: MyRegion

        return Point::getMinimumDistance(s);
    }

    double getMinimumDistance(const MapPoint &p) const
    {
        return mmatch::distance(m_pCoords[0], m_pCoords[1],
                                p.m_pCoords[0], p.m_pCoords[1]);
    }

};



class MapRegion : public Region
{
public:

    MapRegion(double *pLow, double *pHigh):
        Region(pLow, pHigh, 2)
    {}


    double getMinimumDistance(const IShape& s) const
    {
        const MapPoint *mpt = dynamic_cast<const MapPoint*>(&s);
        if (mpt != 0) return getMinimumDistance(*mpt);
        return Region::getMinimumDistance(s);
    }


    double getMinimumDistance(const MapPoint &p) const
    {
        assert(m_dimension == 2);

        double ret = 0.0;

        double coords[2] = {p.m_pCoords[0], p.m_pCoords[1]};

        for (int i = 0; i < 2; ++i)
            if (p.m_pCoords[i] < m_pLow[i])
                coords[i] = m_pLow[i];
            else if (p.m_pCoords[i] > m_pHigh[i])
                coords[i] = m_pHigh[i];

        return mmatch::distance(p.m_pCoords[0], p.m_pCoords[1],
                                coords[0], coords[1]);
    }


};



class MapNeighborVisitor : public IVisitor
{
public:
    std::vector<id_type> neighbors;

    MapNeighborVisitor()
    {
    }


    void visitNode(const INode& n) {}

    void visitData(const IData& d)
    {
        neighbors.push_back(d.getIdentifier());
    }

    void visitData(std::vector<const IData*>& v) {}
};



// TODO: switch to this comparator
class MapNearestNeighbor : public INearestNeighborComparator
{
public:
    double getMinimumDistance(const IShape &query, const IShape &data)
    {
        assert(true);
        return 0;
    }

    double getMinimumDistance(const IShape &query, const IData &data)
    {
        assert(true);
        return 0;
    }

};




#endif // SPATIAL_H
