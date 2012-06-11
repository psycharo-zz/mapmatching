#ifndef SPATIAL_H
#define SPATIAL_H

#include "geometry.h"

#include <spatialindex/SpatialIndex.h>
using namespace SpatialIndex;


namespace mmatch
{

// parameters of spatial index
const int32_t CAPACITY = 100;
const RTree::RTreeVariant TREE_VARIANT = RTree::RV_RSTAR;
const float FILL_FACTOR = 0.7;
const int32_t PAGE_SIZE = 1024;

// the default number of nearest neighbors to search for
const int32_t NN_NUMBER = 5;

// macroses for extracting / setting identifiers
#define SHAPE_ID(edgeID, geomID) ((u_int64_t(edgeID) << 32) | geomID)
#define EDGE_ID(id) (id >> 32)
#define GEOM_ID(id) (id & 0xffffffff)


// initial error (frechet) in meters
const double INITIAL_ERROR = 100;






// simple wrapper to use in spatialindex
class MapPoint : public Point
{
public:
    MapPoint(const UTMNode &p):
        Point((const double *)&p, 2)
    {}
};



// results saver
class MapNeighborVisitor : public IVisitor
{
public:
    std::vector<id_type> neighbors;

    MapNeighborVisitor() {}

    void visitNode(const INode& n) {}

    void visitData(const IData& d)
    {
        neighbors.push_back(d.getIdentifier());
    }

    void visitData(std::vector<const IData*>& v) {}
};

} // namespace mmatch


#endif // SPATIAL_H
