#include "learning.h"


#include "spatial.h"

#include <cfloat>


// DEBUG
#include <iostream>
using namespace std;


using namespace mmatch;


double angleMeasure(const UTMNode &p1, const UTMNode &p2, const vector<UTMNode> &geometry)
{
    UTMNode v1 = p2 - p1;

    double minMeasure = 1.0;
    for (auto it = geometry.begin()+1; it != geometry.end(); ++it)
    {
        UTMNode v2 = *it - *(it-1);
        // cosine
        double currMeasure = v1 * v2 / (v1.length() * v2.length());

        if (currMeasure < minMeasure)
            minMeasure = currMeasure;
    }
    return minMeasure;
}


double distanceMeasure(const UTMNode &p, const UTMNode &cndt1, const UTMNode &cndt2)
{
    return distance(p, cndt1, cndt2);
}


double normalizedMeasure(double d)
{
    double result = (MAX_ERROR - d);
    return result / MAX_ERROR;
}


double distanceMin(const UTMNode &p, const vector<UTMNode> &geometry)
{
    double minMeasure = DBL_MAX;
    for (auto it = geometry.begin()+1; it != geometry.end(); ++it)
    {
        double currMeasure = mmatch::distance(p, *(it-1), *it);
        if (currMeasure < minMeasure)
            minMeasure = currMeasure;
    }
    return minMeasure;
}

double distanceMeasureAvg(const UTMNode &p, const vector<UTMNode> &geometry)
{
    double avgMeasure = 0.0;
    for (auto it = geometry.begin()+1; it != geometry.end(); ++it)
        avgMeasure += mmatch::distance(p, *(it-1), *it);
    return avgMeasure / (geometry.size()-1);
}



Output mmatch::match(const RoadGraph &graph, ISpatialIndex *tree, const Input &input)
{
    if (!tree->isIndexValid())
        throw Exception("spatial index in invalid state");

    if (input.nodes().size() == 0)
        throw Exception("empty input");

    Output out(input);

    UTMNode first = input.nodes().at(0);

    MapPoint query(first);
    MapNeighborVisitor visitor;

    // initialisation step, searching for several nearest points
    tree->nearestNeighborQuery(NN_NUMBER, query, visitor);

    // we need at least one candidate
    assert(visitor.neighbors.size() >= 1);

    double minDist = DBL_MAX;

    // candidate edge id and geom id
    int32_t ceid, cgid;

    // the last matched edge
    const Edge *lastMatchedEdge = NULL;

    // searching for the closest neighbor
    for (id_type id: visitor.neighbors)
    {
        int32_t eid = EDGE_ID(id);
        int32_t gid = GEOM_ID(id);

        const Edge *edge = graph.edge(eid);

        int32_t gida = (gid == edge->geometry.size()-1) ? gid-1 : gid;
        int32_t gidb = (gid == edge->geometry.size()-1) ? gid : gid+1;

        UTMNode a = edge->geometry.at(gida);
        UTMNode b = edge->geometry.at(gidb);

        const double currDist = mmatch::distance(first, a, b);

        if (currDist <= minDist)
        {
            ceid = eid;
            cgid = gid;
            lastMatchedEdge = edge;
            minDist = currDist;
        }
    }

    assert(lastMatchedEdge);

    // first one matched
    out.setEstimation(0, ceid, 1.0);

    // now matching all the subsequent points
    for (size_t i = 1; i < input.nodes().size(); ++i)
    {
        int to = lastMatchedEdge->to;

        double minMeasure = distanceMin(input[i], lastMatchedEdge->geometry);

        for (const Edge *edge : graph.outgoing(to))
        {
            double currMeasure = distanceMin(input[i], edge->geometry);

            if (currMeasure <= minMeasure)
            {
                minMeasure = currMeasure;
                lastMatchedEdge = edge;
                ceid = edge->id;
            }
        }

        // DEBUG
        cout << i << " " << ceid << " " << minMeasure << endl;

        out.setEstimation(i, ceid, 1.0);
    }
    return out;
}






/*
#include "geometry.h"

double maxDistance(const RoadGraph &graph, const Input &input, const Output &output)
{
    double result = 0.0;

    for (int i = 0; i < input.nodes().size(); i++)
    {
        Node node = input.nodes()[i];
        Point2D p(node.latitude, node.longitude);

        Output::Estimate estmt = output.estimates()[i];
        const Edge *edge = graph.edge(estmt.edge);

        Node from(graph.nodes()[edge->from]);
        Node to(graph.nodes()[edge->to]);

        Segment s(Point2D(from.latitude, from.longitude),
                  Point2D(to.latitude, to.longitude));

        double dist = CGAL::squared_distance(p, s);

        if (dist > result)
            result = dist;
    }
    return result;
}



double avgDistance(const RoadGraph &graph, const Input &input, const Output &output)
{
    double result = 0.0;

    for (int i = 0; i < input.nodes().size(); i++)
    {
        Node node = input.nodes()[i];
        Point2D p(node.latitude, node.longitude);

        Output::Estimate estmt = output.estimates()[i];
        const Edge *edge = graph.edge(estmt.edge);

        Node from(graph.nodes()[edge->from]);
        Node to(graph.nodes()[edge->to]);

        Segment s(Point2D(from.latitude, from.longitude),
                  Point2D(to.latitude, to.longitude));

        result += CGAL::squared_distance(p, s);

    }
    return result / input.nodes().size();
}


*/
