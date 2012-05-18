#ifndef GRAPH_H
#define GRAPH_H


#include <vector>
#include <string>

#include "geometry.h"
#include "util.h"


using namespace mmatch;


static const char DELIM = '^';


class Point
{
public:

    Point():
        x(0),y(0)
    {}

    Point(double _x, double _y):
        x(_x),y(_y)
    {}
    double x;
    double y;
};




// TODO: int32?

class Edge
{
public:

    static const size_t MAX_NAME_LENGTH = 64;

    int id;
    int from;
    int to;
    int cost;
    double length;

    // TODO: char name[256]?
    char name[MAX_NAME_LENGTH];
    char type[MAX_NAME_LENGTH];

    std::vector<Node> geometry;

    //! fill geometry data from the given string
    void fillGeometry(const std::string &s);

    // bounding box???
};




class RoadGraph
{
private:
    std::vector<Node> m_nodes;
    std::vector<std::vector<const Edge*> > m_edges;
    std::vector<const Edge*> m_edgeIndex;

public:
    RoadGraph() {}

    virtual ~RoadGraph();

    // TODO: binary save/load!

    //! load all the data from files
    void load(const char *fileNodes, const char *fileEdges, const char *fileGeometry);

    //! load all from binary file
    void load(const char *outputFile);

    //! save everything as binary file
    void save(const char *dst);


    // data accessors
    inline const std::vector<Node> &nodes() const { return m_nodes; }
    inline const std::vector<std::vector<const Edge*> > &edges() const { return m_edges; }
    inline const std::vector<const Edge*> &index() const { return m_edgeIndex; }

    //! get all outgoing edges for a given node id
    inline const std::vector<const Edge*> &outgoing(int nodeId) const { return m_edges[nodeId]; }
    //! get an edge by id
    inline const Edge *edge(int id) const { return m_edgeIndex[id]; }
};







#endif // GRAPH_H
