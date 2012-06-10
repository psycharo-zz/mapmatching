#ifndef ASSIGNEDNODE_H
#define ASSIGNEDNODE_H

#include<geometry.h>
#include<graph.h>
#include <functional>

namespace mmatch {

    class CandidateEdge
    {
    public:
        CandidateEdge(const Edge* edge, int id, float dist)
        {
            this->edge = edge;
            this->mappedInputNodeId = id;
            this->distToInputNode = dist;
        }

        void setMappedInputNodeId(int id)
        {
            mappedInputNodeId = id;
        }

        void setDistToInputNode(float dist)
        {
            this->distToInputNode = dist;
        }

        float getDistToInputNode()
        {
            return this->distToInputNode;
        }

        void setEdge(Edge* e)
        {
            this->edge = e;
        }

        const Edge* getEdge()
        {
            return this->edge;
        }

        int getMappedInputNodeId() const
        {   return mappedInputNodeId; }

//        bool operator < (CandidateEdge &rhs)
//        {
//            return this->getDistToInputNode() < rhs.getDistToInputNode();
//        }

//        bool operator() (CandidateEdge& edge1, CandidateEdge& edge2)
//        {   return edge1.getDistToInputNode() < edge2.getDistToInputNode(); }
    private:
        int mappedInputNodeId;
        float distToInputNode;
        const Edge* edge;
    };

    struct CompareCandidateEdge : public std::binary_function<CandidateEdge, CandidateEdge, bool>
    {
        bool operator()(CandidateEdge& lhs, CandidateEdge& rhs) const
        {
            return lhs.getDistToInputNode() < rhs.getDistToInputNode();
        }
    };
}

#endif // ASSIGNEDNODE_H
