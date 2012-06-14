#include "frechet.h"

#include <util.h>

using namespace mmatch;
using namespace std;

#include <unordered_map>
#include <unordered_set>



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



//Output mmatch::match_frechet(const RoadGraph &graph, ISpatialIndex *index, const Input &input)
//{
//    Output result(input);


//    size_t iteration = 0;
//    cout << "--------------------------" << endl;
//    cout << "input size before filtering: " << input.nodes().size() << endl;
//    auto route = filter_input(input);
//    cout << "input size after filtering: " << route.size() << endl;
//    cout << "--------------------------" << endl;

//    cout << "starting horizontal diagram builder" << endl;

//    // DEBUG
//    double error = 10;

//    // NOTE: THIS IS GRAPH EDGES, while we're working with geometrical edges
//    auto graph_edges = graph.index();
//    auto graph_nodes = graph.nodes();


//    Diagram hor_diagram;
//    // computing one-dimensional diagram for all the geometrical nodes
//    size_t iterations = 0;
//    //
//    vector<dblpair_t> widths(route.size()-1);

//    for (const Edge *edge : graph_edges)
//    {
//        ++iterations;
//        if (iterations % 100000 == 0)
//            cout << iterations << endl;

//        // checking whether the
//        double length = edge->length;
//        UTMNode ep = graph_nodes[edge->from];
//        bool need_check = false;
//        for (const UTMNode &r : route)
//            if (distance(r, ep) - error < length)
//                need_check = true;
//        if (!need_check)
//            continue;

//        auto geom = edge->geometry;
//        size_t geom_size = geom.size();
//        for (int32_t gid = 0; gid < geom_size; ++gid)
//        {
//            // this is the read node id
//            geom_id curr_id = edge->geometry_id(gid);

//            size_t count = 0;
//            // for all the route edges, saving c & d
//            for (int32_t rid = 0; rid < route.size()-1; ++rid)
//            {
//                auto result = line_circle_distance(route[rid], route[rid+1], geom[gid], error);
//                filter(result);
//                if (result.size() > 0)
//                {
//                    widths[rid] = dblpair_t(result[0], result[1]);
//                    ++count;
//                }
//                else
//                    widths[rid] = dblpair_t(1, 0);
//            }

//            if (count > 0)
//                hor_diagram[curr_id] = widths;
//        }
//    }
//    cout << "horizontal diagram size:" << hor_diagram.size() << endl;
//    cout << "------------------------" << endl;


//    cout << "starting vertical diagram builder" << endl;
//    // for each (i,j) \in E, we have k =(route.size()) a, b (vertical spikes)
//    map<geom_id, Diagram> vert_diagram;
//    vector<dblpair_t> heights(route.size());

//    iteration = 0;
//    for (const Edge *edge : graph_edges)
//    {
//        ++iteration;
//        if (iteration % 100000 == 0)
//            cout << iteration << endl;

////         checking whether the
//        UTMNode ep = graph_nodes[edge->from];
//        double length = edge->length;
//        bool check_needed = false;
//        for (const UTMNode &r : route)
//            if (distance(r, ep) - error < length)
//                check_needed = true;
//        if (!check_needed)
//            continue;


//        size_t geom_size = edge->geometry.size();
//        for (int32_t gid = 0; gid < geom_size; ++gid)
//        {
//            geom_id curr_id = edge->geometry_id(gid);
//            // now getting all the adjacent edges
//            for (geom_id adj : graph.adjacent(curr_id))
//            {

//                size_t count = 0;
//                for (int32_t rid = 0; rid < route.size(); ++rid)
//                {
//                    auto result = line_circle_distance(graph.coord(curr_id), graph.coord(adj), route[rid], error);
//                    filter(result);
//                    if (result.size() > 0)
//                    {
//                        heights[rid] = dblpair_t(result[0], result[1]);
//                        count += 1;
//                    }
//                    else
//                        heights[rid] = dblpair_t(1, 0);
//                }
//                if (count > 0)
//                    vert_diagram[curr_id][adj] = heights;
//            }
//        }
//    }

//    cout << "vertical diagram size:" << vert_diagram.size() << endl;
//    cout << "----------------------" << endl;

//    // the priority queue Q
//    priority_queue<Interval> queue;

//    // putting all degenerate intervals starting from the leftmost point

//    while (!queue.empty())
//    {
//        Interval interval = queue.top();
//        queue.pop();

//        cout << interval << endl;


//        if (true) // actually found the solution
//        {

//        }
//    }

//    return result;
//}







struct diagram_id
{
    enum Type
    {
        HORIZONTAL,
        VERTICAL,
        SOURCE,
        TARGET
    };


    // the id of the path segment
    geom_id node;
    // the id of the route segment
    int32_t route;
    // false -> horizontal, true -> vertical
    Type type;
    // the node weight (distance from the route)
    double weight;

    diagram_id(geom_id _node, int32_t _route, Type _tp, double _weight):
        node(_node),route(_route),type(_tp),weight(_weight)
    {}

    diagram_id(geom_id _node, int32_t _route, Type _tp):
        node(_node),route(_route),type(_tp),weight(-1)
    {}


    bool operator ==(const diagram_id &other) const
    {
        return (node == other.node) && (route == other.route) && (type == other.type);
    }

    bool operator !=(const diagram_id &other) const
    {
        return !(*this == other);
    }


    bool operator <(const diagram_id &other) const
    {
        return weight < other.weight;
    }
};



ostream &operator<<(ostream &os, const diagram_id &did)
{
    string name;
    if (did.type == diagram_id::HORIZONTAL)
        name = "HOR";
    else if (did.type == diagram_id::VERTICAL)
        name = "VERT";
    else if (did.type == diagram_id::SOURCE)
        name = "SRC";

    os << did.node << " route(" << did.route << ") type(" << name << ") w(" << did.weight << ")";
    return os;
}

namespace std
{

template <>
class hash<diagram_id>
{
public :
    size_t operator()(const diagram_id &x) const
    {
        return hash<geom_id>()(x.node) ^ hash<int32_t>()(x.route);
    }
};

}




Output mmatch::match_frechet(const RoadGraph &graph, ISpatialIndex *tree, const Input &input)
{
    Output out(input);

    // mapping diagram_id to its weight
    unordered_map<diagram_id, double> dist;

    multiset<diagram_id> queue;
    unordered_map<diagram_id, multiset<diagram_id>::iterator> index;
    unordered_map<diagram_id, diagram_id> optimal;


    auto update_value = [&dist, &queue, &index] (diagram_id id)
    {
        if (id.weight < MAX_ERROR_GLOBAL)
        {
            // if such a value already exists
            if (index.count(id))
            {
                auto it = index[id];
                queue.erase(it);
            }
            index[id] = queue.insert(id);
            dist[id] = id.weight;
        }
    };

    auto relax_value = [&dist, &queue, &index, &update_value, &optimal] (diagram_id id, diagram_id curr)
    {
        if (dist.count(id))
        {
            // if the value is in there, we should only update the value
            // is more optimal that the one stored (i.e. dist[id] < id.weight)
            if (dist[id] < id.weight)
            {
                update_value(id);
                optimal.insert({id, curr});
            }
        }
        else
        {
            update_value(id);
            optimal.insert({id, curr});
        }
    };


    auto pop_value = [&queue, &index] () -> diagram_id
    {
        assert(!queue.empty());

        diagram_id id = *queue.begin();
        size_t ret = index.erase(id);
        assert(ret != 0);
        queue.erase(queue.begin());
        return id;
    };

    auto route = input.nodes();

    assert(route.size() >= 2);

    MapPoint query(route[0]);
    MapNeighborVisitor visitor;
    // initialisation step, searching for several nearest points
    tree->nearestNeighborQuery(NN_NUMBER_GLOBAL, query, visitor);
    for (id_type src : visitor.neighbors)
    {
        geom_id id = graph.edge(EDGE_ID(src))->geometry_id(GEOM_ID(src));
        double weight = distance(graph.coord(id), route[0], route[1]);
        update_value(diagram_id(id, 0, diagram_id::HORIZONTAL, weight));
    }

    diagram_id curr = pop_value();
    while (!queue.empty())
    {
        if (curr.route == route.size()-1)
            break;

        UTMNode curr_coord = graph.coord(curr.node);

        // CHECK IF THIS CAN INFLUENCE ANYTHING
        double curr_dist = dist[curr];

        if (curr.type == diagram_id::HORIZONTAL)
        {
            for (geom_id adj : graph.adjacent(curr.node))
            {
                UTMNode adj_coord = graph.coord(adj);
                double weight = -1;
                diagram_id did(curr.node, 0, diagram_id::VERTICAL, -1);

                // LEFT
                weight = max(distance(route[curr.route], curr_coord, adj_coord), curr_dist);
                did = diagram_id(curr.node, curr.route, diagram_id::VERTICAL, weight);
                relax_value(did, curr);
                // RIGHT
                weight = max(distance(route[curr.route+1], curr_coord, adj_coord), curr_dist);
                did = diagram_id(curr.node, curr.route+1, diagram_id::VERTICAL, weight);
                relax_value(did, curr);
                // TOP
                weight = max(distance(adj_coord, route[curr.route], route[curr.route+1]), curr_dist);
                did = diagram_id(adj, curr.route, diagram_id::HORIZONTAL, weight);
                relax_value(did, curr);
            }


        }
        else if (curr.type == diagram_id::VERTICAL)
        {
            for (geom_id adj : graph.adjacent(curr.node))
            {
                UTMNode adj_coord = graph.coord(adj);
                double weight = -1;
                diagram_id did(curr.node, 0, diagram_id::VERTICAL, -1);

                if (curr.route > 0)
                {
                    // all the leftmost
                    // LEFT
                    weight = max(distance(route[curr.route-1], curr_coord, adj_coord), curr_dist);
                    did = diagram_id(curr.node, curr.route-1, diagram_id::VERTICAL, weight);
                    relax_value(did, curr);
                    // BOTTOM
                    weight = max(distance(curr_coord, route[curr.route-1], route[curr.route]), curr_dist);
                    did = diagram_id(curr.node, curr.route-1, diagram_id::HORIZONTAL, weight);
                    relax_value(did, curr);
                    // TOP
                    weight = max(distance(adj_coord, route[curr.route-1], route[curr.route]), curr_dist);
                    did = diagram_id(adj, curr.route-1, diagram_id::HORIZONTAL, weight);
                    relax_value(did, curr);
                }

                if (curr.route < route.size()-1)
                {
                    // all the rightmost

                    // RIGHT
                    weight = max(distance(route[curr.route+1], curr_coord, adj_coord), curr_dist);
                    did = diagram_id(curr.node, curr.route+1, diagram_id::VERTICAL, weight);
                    relax_value(did, curr);
                    // BOTTOM
                    weight = max(distance(curr_coord, route[curr.route], route[curr.route+1]), curr_dist);
                    did = diagram_id(curr.node, curr.route+1, diagram_id::HORIZONTAL, weight);
                    relax_value(did, curr);
                    // TOP
                    weight = max(distance(adj_coord, route[curr.route], route[curr.route+1]), curr_dist);
                    did = diagram_id(adj, curr.route+1, diagram_id::HORIZONTAL, weight);
                    relax_value(did, curr);

                }
            }

        }

        curr = pop_value();
    }

    vector<geom_id> matched;
    for (size_t i = 0; i < route.size(); ++i)
    {
        matched.push_back(curr.node);
        curr = optimal.find(curr)->second;
    }
    reverse(matched.begin(), matched.end());

    size_t prev = 0;

    size_t count = 0;
    for (size_t i = 1; i < matched.size(); ++i)
    {
        if (matched[i].is_internal() && !matched[prev].is_internal())
        {
            for (size_t j = prev; j <= i; ++j)
                out.setEstimation(j, matched[i].eid, 1.0);
            prev = i;
        }
        else if (matched[i].is_internal() && matched[prev].is_internal())
        {
            out.setEstimation(i, matched[i].eid, 1.0);
            prev = i;
        }
        else if (!matched[i].is_internal() && matched[prev].is_internal())
        {
            out.setEstimation(i, matched[prev].eid, 1.0);
            prev = i;
        }
        else if (matched[i] != matched[prev])
        {
            // TODO:?
            ++count;
        }
    }

    cout << double(count) / route.size() << endl;

    return out;
}




