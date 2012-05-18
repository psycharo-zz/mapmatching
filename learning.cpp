#include "learning.h"

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
