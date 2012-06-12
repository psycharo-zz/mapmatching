#include "learning.h"

#include "spatial.h"

#include <cfloat>
#include <queue>
#include <stack>
#include <CandidaEdge.h>
#include <unordered_set>


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
        out.getMaxError() = minMeasure > out.getMaxError() ? minMeasure : out.getMaxError();
//        cout << i << " " << ceid << " " << minMeasure << endl;

        out.setEstimation(i, ceid, 1.0);
    }
    return out;
}




struct GeometryEdge
{
    geom_id from;
    geom_id to;

    double weight;

    GeometryEdge(geom_id _from, geom_id _to, double _weight):
        from(_from), to(_to), weight(_weight)
    {}

    bool operator < (const GeometryEdge &other) const
    {
        return weight < other.weight;
    }




};



struct MatchPair
{
    //! the edge to match
    GeometryEdge edge;

    //! point being matched
    size_t point;

    MatchPair(const GeometryEdge &_edge, size_t _point):
        edge(_edge),point(_point)
    {}

    bool operator==(const MatchPair &other) const
    {
        return (point == other.point) && (edge.from == other.edge.from) && (edge.to == other.edge.to);
    }
};


namespace std
{
template <>
class hash<MatchPair>
        {
        public :
        size_t operator()(const MatchPair &x) const
        {
            return hash<int>()(x.edge.from.eid) ^ hash<int>()(x.edge.to.gid) ^ hash<int>()(x.point);
        }
    };
}


multiset<GeometryEdge> get_candidates(const RoadGraph &graph, const GeometryEdge &curr, const UTMNode &route_point)
{
    multiset<GeometryEdge> candidates;
    UTMNode from = graph.coord(curr.to);
    for (geom_id adj : graph.adjacent(curr.to))
    {
        UTMNode to = graph.coord(adj);
        GeometryEdge adj_edge(curr.to, adj, distance(route_point, from, to));
        candidates.insert(adj_edge);
    }
    return candidates;
}


Output mmatch::backtracing_match(const RoadGraph &graph, ISpatialIndex *tree, const Input &input, const double max_error)
{
    Output out(input);

    vector<UTMNode> route = input.nodes();

    MapPoint query(route[0]);
    MapNeighborVisitor visitor;

    // initialisation step, searching for several nearest points
    tree->nearestNeighborQuery(NN_NUMBER, query, visitor);

    // candidate edge id and geom id
    geom_id curr_id(-1, -1);

    multiset<GeometryEdge> source_candidates;

    for (id_type id : visitor.neighbors)
    {
        int32_t gid = GEOM_ID(id);
        curr_id.eid = EDGE_ID(id);
        curr_id.gid = gid;

        const Edge *edge = graph.edge(curr_id.eid);
        size_t geom_size = edge->geometry.size();

        int32_t gida = (gid == geom_size-1) ? gid-1 : gid;
        int32_t gidb = (gid == geom_size-1) ? gid : gid+1;

        geom_id from = edge->geometry_id(gida);
        geom_id to = edge->geometry_id(gidb);

        // FIXME
        double weight = distance(route[0], graph.coord(from), graph.coord(to));
        source_candidates.insert(GeometryEdge(from, to, weight));
    }


    vector<GeometryEdge> matched_route;
    bool found = false;

    for (const GeometryEdge &src : source_candidates)
    {
        if (found)
            break;

        matched_route = {src};

        unordered_set<MatchPair> matched_table;
        stack<MatchPair> pairs;
        pairs.push(MatchPair(src, 0));
        while (!pairs.empty() && !found)
        {
            MatchPair p = pairs.top();
            pairs.pop();
            matched_table.insert(p);

            GeometryEdge curr = p.edge;
            if (curr.weight < max_error)
            {

                // FOUND!!!
                if (p.point == route.size()-1)
                {
                    found = true;
                    matched_route.push_back(curr);
                }
                else
                {
                    // NOT YET :/
                    // removing all up until this point
                    while (matched_route.size() > p.point)
                        matched_route.pop_back();

                    matched_route.push_back(curr);

                    UTMNode route_point = route[p.point+1];

                    multiset<GeometryEdge> candidates = get_candidates(graph, curr, route_point);

                    double weight = distance(route_point, graph.coord(p.edge.from), graph.coord(p.edge.to));

                    for (const GeometryEdge &c : candidates)
                    {
                        MatchPair cp(c, p.point+1);
                        if (!matched_table.count(cp))
                            pairs.push(cp);
                    }
                    pairs.push(MatchPair(GeometryEdge(p.edge.from, p.edge.to, weight), p.point+1));
                }
            }
        }
    }

    assert(found && matched_route.size() == input.size());

    for (size_t i = 0; i < matched_route.size(); ++i)
    {
        GeometryEdge e = matched_route[i];
        if (e.from.is_internal())
            out.setEstimation(i, e.from.eid, 1.0);
        else if (e.to.is_internal())
            out.setEstimation(i, e.to.eid, 1.0);
        else
        {
            vector<const Edge*> all = graph.outgoing(e.from.gid);
            const Edge *edge = *find_if(all.begin(), all.end(),
                                        [&e](const Edge *edge) { return edge->to == e.to.gid; });
            out.setEstimation(i, edge->id, 1.0);
        }
    }
    return out;
}


