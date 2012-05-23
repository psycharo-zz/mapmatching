#include "learning.h"


#include "spatial.h"

#include <cfloat>


// DEBUG
#include <iostream>
using namespace std;


using namespace mmatch;


double angleMeasure(const UTMNode &p1, const UTMNode &p2, const UTMNode &c1, const UTMNode &c2)
{
    UTMNode v1 = p2 - p1;
    UTMNode v2 = c2 - c1;
    // cosine
    return v1 * v2 / (v1.length() * v2.length());
}

double distanceMeasure(const UTMNode &p, const UTMNode &cndta, const UTMNode &cndtb)
{
    return distance(p, cndta, cndtb);
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
        int from = lastMatchedEdge->from;
        int to = lastMatchedEdge->to;

        const UTMNode &p1 = graph.nodes().at(from);
        const UTMNode &p2 = graph.nodes().at(to);

        double minMeasure = DIST_WEIGHT * distanceMeasure(input[i], p1, p2); //                            ANGLE_WEIGHT * angleMeasure(input[i-1], input[i], p1, p2);

        const Edge *matchedEdge = lastMatchedEdge;

        for (const Edge *edge : graph.outgoing(to))
        {
            // TODO: check with all the subedges of the edge
            const UTMNode &c1 = graph.nodes().at(to);
            const UTMNode &c2 = graph.nodes().at(edge->to);

            double currMeasure = DIST_WEIGHT * distanceMeasure(input[i], c1, c2); // + ANGLE_WEIGHT * angleMeasure(input[i-1], input[i], c1, c2);

            if (currMeasure < minMeasure)
            {
                minMeasure = currMeasure;
                matchedEdge = edge;
                ceid = edge->id;
            }

        }

        lastMatchedEdge = matchedEdge;

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
