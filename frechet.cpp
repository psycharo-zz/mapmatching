#include "frechet.h"

#include <util.h>

using namespace mmatch;
using namespace std;

#include <unordered_map>



void filter(vector<double> &results)
{
    if (results.size() == 0)
        return;

    double t1 = results[0];
    double t2 = results[1];

    if (t1 < 0)
    {
        if (t2 < 0)
            results.clear(); // cout << "skipping:" << t1 << ":" << t2 << endl;
        else if (t2 >= 0 && t2 <= 1)
            results[0] = 0;   //          cout << "t1 inside:" << 0 << ":" << t2 << ":" << param(route[rid], route[rid+1], t2) << endl;
        else if (t2 > 1)
        {
            results[0] = 0;
            results[1] = 1; // cout << "completely inside:" << t1 << ":" << t2 << endl;
        }
    }
    else if (t1 >= 0 && t1 <= 1)
    {
        // t2 < 0 - not possible
        assert(t2 >= 0);
        if (t2 >= 0 && t2 <= 1)
            return; // cout << "as they are:" << t1 << ":" << t2 << endl;
        else if (t2 > 1)
            results[1] = 1; // cout << "t2 inside:" << t1 << ":" << 1 << param(route[rid], routde[rid+1], t2) << endl;
    }
    else if (t1 > 1)
    {
        assert(t2 > 1);
        results.clear(); //         cout << "skipping:" << t1 << ":" << t2 << endl;
    }
}



Output mmatch::match_frechet(const RoadGraph &graph, ISpatialIndex *index, const Input &input)
{
    Output result(input);


    // DEBUG
    double error = 15;

    // NOTE: THIS ARE GRAPH NODES, while we're using geometrical nodes,
    // that are indexed by SHAPE_ID(edge_id, geom_id)
    auto graph_nodes = graph.nodes();
    auto route = input.nodes();

    // NOTE: THIS IS GRAPH EDGES, while we're working with geometrical edges
    auto graph_edges = graph.index();

    // all the intervals
    typedef pair<double, double> dblpair_t;
    typedef map<geom_id, vector<dblpair_t> > Diagram;


//    Diagram hor_diagram;

//    // computing one-dimensional diagram for all the geometrical nodes
//    // TODO: NODES ARE ACTUALLY COMPOUND
//    // initialisation

//    //
//    vector<dblpair_t> widths(route.size()-1);

//    for (const Edge *edge : graph_edges)
//    {
//        auto geom = edge->geometry;
//        for (int32_t gid = 0; gid < geom.size(); ++gid)
//        {
//            // this is the read node id
//            geom_id curr_id = edge->geometry_id(gid);

//            // for all the route edges, saving c & d
//            for (int32_t rid = 0; rid < route.size()-1; ++rid)
//            {
//                auto result = line_circle_distance(route[rid], route[rid+1], geom[gid], error);
//                filter(result);
//                widths[rid] = (result.size() > 0) ? dblpair_t(result[0], result[1]) : dblpair_t(1, 0);
//            }
//            hor_diagram[curr_id] = widths;
//        }
//    }


    // for each (i,j) \in E, we have k =(route.size()) a, b (vertical spikes)
    map<geom_id, Diagram> vert_diagram;



    // now computing edge - to - route diagram points
    // TODO: unnecessary operations

    vector<dblpair_t> heights(route.size());



    size_t iteration = 0;
    for (const Edge *edge : graph_edges)
    {
        ++iteration;
        if (iteration % 100000 == 0)
            cout << iteration << endl;

        // TODO: elaborate on this!!!
        size_t count = 0;
        for (const UTMNode &r : route)
            if (distance(r, graph_nodes[edge->from])-error > edge->length)
                ++count;

        if (count == route.size())
            continue;

        size_t geom_size = edge->geometry.size();
        for (int32_t gid = 0; gid < geom_size; ++gid)
        {
            geom_id curr_id = edge->geometry_id(gid);
            // now getting all the adjacent edges
            for (geom_id adj : graph.adjacent(curr_id))
            {

                size_t count = 0;
                for (int32_t rid = 0; rid < route.size(); ++rid)
                {
                    auto result = line_circle_distance(graph.node(curr_id), graph.node(adj), route[rid], error);
                    filter(result);
                    if (result.size() > 0)
                    {
                        heights[rid] = dblpair_t(result[0], result[1]);
                        count += 1;
                    }
                    else
                        heights[rid] = dblpair_t(1, 0);
                }
                if (count > 0)
                    vert_diagram[curr_id][adj] = heights;
            }
        }
    }

    cout << vert_diagram.size() << endl;

    // the priority queue Q
    priority_queue<Interval> queue;
    // putting all white intervals starting from the leftmost point

    while (!queue.empty())
    {
        Interval interval = queue.top();
        queue.pop();

        cout << interval << endl;


        if (true) // actually found the solution
        {

        }
    }




    return result;
}
