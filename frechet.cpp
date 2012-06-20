#include "frechet.h"

#include <util.h>

using namespace mmatch;
using namespace std;

#include <unordered_map>
#include <unordered_set>



// all the intervals
typedef pair<double, double> dblpair_t;
typedef map<geom_id, vector<dblpair_t> > Diagram;



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

    diagram_id():
        node(-1,-1),route(-1),type(TARGET),weight(0)
    {}

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



Output mmatch::match_frechet(const RoadGraph &graph, ISpatialIndex *tree, const Input &input, const double max_error2)
{
    Output out(input);

    // mapping diagram_id to its weight
    unordered_map<diagram_id, double> dist;

    multiset<diagram_id> queue;
    unordered_map<diagram_id, multiset<diagram_id>::iterator> index(BUCKETS);
    unordered_map<diagram_id, diagram_id> optimal(BUCKETS);

    auto update_value = [&dist, &queue, &index, &max_error2] (const diagram_id &id)
    {
        if (id.weight < max_error2)
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

    auto set_value = [&dist, &queue, &index] (const diagram_id &id)
    {
        auto it = index[id];
        queue.erase(it);
        index[id] = queue.insert(id);
        dist[id] = id.weight;
    };

    auto relax_value = [&dist, &queue, &index, &update_value, &set_value, &optimal] (const diagram_id &id, const diagram_id &curr)
    {
        auto it = dist.find(id);
        if (it != dist.end())
        {
            // if the value is in there, we should only update the value
            // is more optimal that the one stored (i.e. dist[id] < id.weight)
            if (it->second < id.weight)
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
        diagram_id id = *queue.begin();
        size_t ret = index.erase(id);
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
        double weight = distance2(graph.coord(id), route[0], route[1]);
        update_value(diagram_id(id, 0, diagram_id::HORIZONTAL, weight));
    }


    size_t route_max = 0;
    diagram_id curr;
    while (!queue.empty())
    {
        curr = pop_value();

        if (curr.route > route_max)
        {
            cout << curr.route << endl;
            route_max = curr.route;
        }

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
                weight = max(distance2(route[curr.route], curr_coord, adj_coord), curr_dist);
                did = diagram_id(curr.node, curr.route, diagram_id::VERTICAL, weight);
                relax_value(did, curr);
                // RIGHT
                weight = max(distance2(route[curr.route+1], curr_coord, adj_coord), curr_dist);
                did = diagram_id(curr.node, curr.route+1, diagram_id::VERTICAL, weight);
                relax_value(did, curr);
                // TOP
                weight = max(distance2(adj_coord, route[curr.route], route[curr.route+1]), curr_dist);
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
                    weight = max(distance2(route[curr.route-1], curr_coord, adj_coord), curr_dist);
                    did = diagram_id(curr.node, curr.route-1, diagram_id::VERTICAL, weight);
                    relax_value(did, curr);
                    // BOTTOM
                    weight = max(distance2(curr_coord, route[curr.route-1], route[curr.route]), curr_dist);
                    did = diagram_id(curr.node, curr.route-1, diagram_id::HORIZONTAL, weight);
                    relax_value(did, curr);
                    // TOP
                    weight = max(distance2(adj_coord, route[curr.route-1], route[curr.route]), curr_dist);
                    did = diagram_id(adj, curr.route-1, diagram_id::HORIZONTAL, weight);
                    relax_value(did, curr);

                }

                if (curr.route < route.size()-1)
                {
                    // all the rightmost

                    // RIGHT
                    weight = max(distance2(route[curr.route+1], curr_coord, adj_coord), curr_dist);
                    did = diagram_id(curr.node, curr.route+1, diagram_id::VERTICAL, weight);
                    relax_value(did, curr);
                    // BOTTOM
                    weight = max(distance2(curr_coord, route[curr.route], route[curr.route+1]), curr_dist);
                    did = diagram_id(curr.node, curr.route+1, diagram_id::HORIZONTAL, weight);
                    relax_value(did, curr);
                    // TOP
                    weight = max(distance2(adj_coord, route[curr.route], route[curr.route+1]), curr_dist);
                    did = diagram_id(adj, curr.route+1, diagram_id::HORIZONTAL, weight);
                    relax_value(did, curr);
                }
            }

        }

        curr = pop_value();
    }

    if (optimal.size() == 0 || curr.route != route.size()-1)
    {
        Output output;
        output.setError(sqrt(curr.weight));
//        cout << "max route:" << route_max << " " << output.maxError() << endl;
        return output;
    }

    // first match the f
    int32_t prev_match = 0;
    diagram_id next = curr;
      // reconstructing the path
    while (curr.route != 0)
    {
        if (curr.node.is_internal())
        {
            out.setEstimation(curr.route, curr.node.eid, 1.0);
            prev_match = curr.node.eid;
        }
        else if (next.node.is_internal())
        {
            out.setEstimation(curr.route, next.node.eid, 1.0);
            prev_match = next.node.eid;
        }
        else if (next.node != curr.node) // one by one different ids
        {
            for (const Edge *e : graph.outgoing(curr.node.gid))
            {
                if (e->to == next.node.gid)
                {
                    out.setEstimation(curr.route, e->id, 1.0);
                    prev_match = e->id;
                }
            }
        }
        else // one by one same ids
        {
            // just matching the last match
            out.setEstimation(curr.route, prev_match, 1.0);
        }
        curr = optimal[curr];
        next = curr;
    }
    out.setEstimation(curr.route, prev_match, 1.0);
    out.setError(sqrt(curr.weight));
    return out;
}


Output mmatch::match_frechet_smart(const RoadGraph &graph, ISpatialIndex *index, const Input &input)
{
    const size_t NUM_PARTS = 10;
    vector<Input> inputs = input.split(NUM_PARTS);

    vector<Output> result;
    for (const Input &i : inputs)
    {
        cout << "size:" << i.size() << endl;
        Output out = match_frechet(graph, index, i, MAX_CONSIDERED_AREA);
        if (out.size() != i.size())
        {
            // TODO:
            cout << "NOT FOUND" << endl;
        }
        result.push_back(out);
    }


    return Output(result);
}



