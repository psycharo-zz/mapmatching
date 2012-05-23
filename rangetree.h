#ifndef RANGETREE_H
#define RANGETREE_H

#include <cstddef>
#include <vector>
#include <algorithm>
#include <list>
#include <cassert>

#include "graph.h"


using namespace mmatch;


namespace mmatch {


class RangeTree
{
private:

    enum NodeType
    {
        LEAF,
        VERTICAL,
        HORIZONTAL
    };

    class TreeNode
    {
    public:
        TreeNode(const UTMNode &p):
            left(NULL),
            right(NULL),
            point(p),
            type(LEAF)
        {}

        TreeNode(TreeNode *_left, TreeNode *_right, NodeType _type, double line):
            left(_left),
            right(_right),
            type(_type)
        {
            assert(type != LEAF);
            (type == VERTICAL) ? point.x = line : point.y = line;
        }


        inline double line() const { return (type == VERTICAL) ? point.x : point.y; }

        TreeNode *left;
        TreeNode *right;

        // TODO: two types of nodes?
        UTMNode point;

        NodeType type;
    };

    TreeNode *m_root;

    // recursive method for constructing the tree
    RangeTree::TreeNode *build(std::vector<UTMNode>::iterator start, std::vector<UTMNode>::iterator end, size_t depth);

    class Range
    {
    public:

        Range(double _leftX, double _rightX,
              double _bottomY, double _topY):
            leftX(_leftX), rightX(_rightX),
            bottomY(_bottomY), topY(_topY)
        {}

        bool contains(double x, double y) const
        {
            return x >= leftX && x <= rightX && y >= bottomY && y <= topY;
        }

        bool contains(const UTMNode &q) const
        {
            return q.x >= leftX && q.x <= rightX && q.y >= bottomY && q.y <= topY;
        }

        bool inside(const Range &r) const
        {
            return leftX > r.leftX && rightX < r.rightX &&
                   bottomY > r.bottomY && topY < r.topY;
        }

        bool intersects(const Range &r) const
        {
            return contains(leftX, bottomY) || contains(leftX, topY) ||
                   contains(rightX, bottomY) || contains(rightX, topY);
        }


        Range below(double y) const { return Range(leftX, rightX, bottomY, std::min(topY, y)); }
        Range above(double y) const { return Range(leftX, rightX, std::max(bottomY, y), topY); }
        Range left(double x) const { return Range(leftX, std::min(rightX, x), bottomY, topY); }
        Range right(double x) const { return Range(std::max(leftX, x), rightX, bottomY, topY); }


        double leftX, rightX;
        double topY, bottomY;
    };


    // recursive method for search the tree
    std::list<UTMNode> searchTree(TreeNode *root, const Range &query, const Range &range);

    // get all points from the tree
    std::list<UTMNode> points(TreeNode *root);

public:
    RangeTree(const std::vector<UTMNode> &points);

    std::list<UTMNode> search(const UTMNode &q, double w, double h);

//    Point nn();



};


} // namespace mmatch

#endif // RANGETREE_H
