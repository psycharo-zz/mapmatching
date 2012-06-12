#include "frechet.h"

#include <util.h>

using namespace mmatch;
using namespace std;

#include <unordered_map>



// all the intervals
typedef pair<double, double> dblpair_t;
typedef map<geom_id, vector<dblpair_t> > Diagram;





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


// removing almost identical
vector<UTMNode> filter_input(const Input &input)
{
    vector<UTMNode> route = {input.nodes().front()};
    for (size_t i = 1; i < input.nodes().size(); i++)
    {
        if (distance(input.nodes().at(i), route.back()) >= EPS)
            route.push_back(input.nodes().at(i));
    }
    return route;
}



Output mmatch::match_frechet(const RoadGraph &graph, ISpatialIndex *index, const Input &input)
{
    Output result(input);


    size_t iteration = 0;
    cout << "--------------------------" << endl;
    cout << "input size before filtering: " << input.nodes().size() << endl;
    auto route = filter_input(input);
    cout << "input size after filtering: " << route.size() << endl;
    cout << "--------------------------" << endl;

    cout << "starting horizontal diagram builder" << endl;

    // DEBUG
    double error = 10;

    // NOTE: THIS IS GRAPH EDGES, while we're working with geometrical edges
    auto graph_edges = graph.index();
    auto graph_nodes = graph.nodes();


    Diagram hor_diagram;
    // computing one-dimensional diagram for all the geometrical nodes
    size_t iterations = 0;
    //
    vector<dblpair_t> widths(route.size()-1);

    for (const Edge *edge : graph_edges)
    {
        ++iterations;
        if (iterations % 100000 == 0)
            cout << iterations << endl;

        // checking whether the
        double length = edge->length;
        UTMNode ep = graph_nodes[edge->from];
        bool need_check = false;
        for (const UTMNode &r : route)
            if (distance(r, ep) - error < length)
                need_check = true;
        if (!need_check)
            continue;

        auto geom = edge->geometry;
        size_t geom_size = geom.size();
        for (int32_t gid = 0; gid < geom_size; ++gid)
        {
            // this is the read node id
            geom_id curr_id = edge->geometry_id(gid);

            size_t count = 0;
            // for all the route edges, saving c & d
            for (int32_t rid = 0; rid < route.size()-1; ++rid)
            {
                auto result = line_circle_distance(route[rid], route[rid+1], geom[gid], error);
                filter(result);
                if (result.size() > 0)
                {
                    widths[rid] = dblpair_t(result[0], result[1]);
                    ++count;
                }
                else
                    widths[rid] = dblpair_t(1, 0);
            }

            if (count > 0)
                hor_diagram[curr_id] = widths;
        }
    }
    cout << "horizontal diagram size:" << hor_diagram.size() << endl;
    cout << "------------------------" << endl;


    cout << "starting vertical diagram builder" << endl;
    // for each (i,j) \in E, we have k =(route.size()) a, b (vertical spikes)
    map<geom_id, Diagram> vert_diagram;
    vector<dblpair_t> heights(route.size());

    iteration = 0;
    for (const Edge *edge : graph_edges)
    {
        ++iteration;
        if (iteration % 100000 == 0)
            cout << iteration << endl;

//         checking whether the
        UTMNode ep = graph_nodes[edge->from];
        double length = edge->length;
        bool check_needed = false;
        for (const UTMNode &r : route)
            if (distance(r, ep) - error < length)
                check_needed = true;
        if (!check_needed)
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

    cout << "vertical diagram size:" << vert_diagram.size() << endl;
    cout << "----------------------" << endl;

    // the priority queue Q
    priority_queue<Interval> queue;

    // putting all degenerate intervals starting from the leftmost point

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

