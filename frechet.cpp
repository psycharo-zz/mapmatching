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
    double error = 10;

    // NOTE: THIS ARE GRAPH NODES, while we're using geometrical nodes,
    // that are indexed by SHAPE_ID(edge_id, geom_id)
    auto graph_nodes = graph.nodes();
    auto route = input.nodes();

    // NOTE: THIS IS GRAPH EDGES, while we're working with geometrical edges
    auto graph_edges = graph.index();

    // all the intervals
    typedef pair<double, double> dblpair_t;
    map<id_type, vector<dblpair_t> > diagram;

    // computing one-dimensional diagram for all the geometrical nodes
    // TODO: NODES ARE ACTUALLY COMPOUND
    // initialisation
    for (const Edge *edge : graph_edges)
    {
        auto geom = edge->geometry;

        for (int32_t gid = 0; gid < geom.size(); ++gid)
        {
            // this is the read node id
            id_type node_id = SHAPE_ID(edge->id, gid);

            cout << "node id:" << node_id << " " << edge->id << " " << gid << endl;
            vector<dblpair_t> cells(route.size()-1);
            // for all the route edges, saving c & d
            for (int32_t rid = 0; rid < route.size()-1; ++rid)
            {
                auto result = line_circle_distance(route[rid], route[rid+1], geom[gid], error);
                filter(result);
                if (result.size() > 0)
                    cells[rid] = dblpair_t(result[0], result[1]);
                else
                    cells[rid] = dblpair_t(1, 0);
            }

            diagram[node_id] = cells;
        }
    }

    // now computing the same for


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
