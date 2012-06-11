#ifndef GRAPH_H
#define GRAPH_H


#include <vector>
#include <string>

#include "geometry.h"
#include "util.h"


using namespace mmatch;


namespace mmatch {


/**
 * @brief The Edge class
 */
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

    std::vector<UTMNode> geometry;

    //! fill geometry data from the given string
    void fillGeometry(const std::string &s);

    //! fill geometry, metric data
    void fillUTMGeometry(const std::string &s);

    // bounding box???
};



/**
 * @brief The RoadGraph class incapsulates the road structure
 */
class RoadGraph
{
private:
    std::vector<UTMNode> m_nodes;
    std::vector<std::vector<const Edge*> > m_edges;
    std::vector<const Edge*> m_edgeIndex;

public:
    RoadGraph() {}

    virtual ~RoadGraph();

    // TODO: binary save/load!

    //! load all the data from files
    void load(const char *fileNodes, const char *fileEdges, const char *fileGeometry);

    //! load all the data from files, metric coords
    void loadUTM(const char *fileNodes, const char *fileEdges, const char *fileGeometry);


    //! load everything from provided data
    void fromData(const std::vector<UTMNode> &, const std::vector<Edge> &);

    //! load all from binary file
    void loadBinary(const char *outputFile);

    //! save everything as binary file
    void saveBinary(const char *dst);


    // data accessors
    inline const std::vector<UTMNode> &nodes() const { return m_nodes; }
    inline const std::vector<std::vector<const Edge*> > &edges() const { return m_edges; }
    inline const std::vector<const Edge*> &index() const { return m_edgeIndex; }
    inline const UTMNode &getNode(int32_t eid, int32_t gid) const { return m_edgeIndex[eid]->geometry[gid]; }


    //! get all outgoing edges for a given node id
    inline const std::vector<const Edge*> &outgoing(int nodeId) const { return m_edges[nodeId]; }
    //! get an edge by id
    inline const Edge *edge(int id) const { return m_edgeIndex[id]; }
};



} // namespace mmatch



#endif // GRAPH_H
