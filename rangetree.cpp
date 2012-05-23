#include "rangetree.h"


using namespace std;

#include <algorithm>
#include <cmath>
#include <functional>
#include <cfloat>

#include <iostream>

typedef vector<UTMNode>::iterator VecIt;


RangeTree::RangeTree(const vector<UTMNode> &nodes)
{
    vector<UTMNode> points = nodes;
    m_root = build(points.begin(), points.end(), 0);
}


RangeTree::TreeNode *RangeTree::build(VecIt begin, VecIt end, size_t depth)
{
    assert(begin != end);
    assert(begin <= end);

    size_t size = end - begin;

    // just one element
    if (size == 1)
        return new TreeNode(*begin);

    NodeType type = (depth & 1) ? HORIZONTAL : VERTICAL;

    VecIt median = begin + ceil(0.5 * size) - 1;

    // DEBUG
    assert(median >= begin);
    assert(median < end);

    double medianValue;

    // for odd, splitting by x
    if (type == VERTICAL)
    {
        nth_element(begin, median, end,
                    [](const UTMNode &p1, const UTMNode &p2) { return p1.x < p2.x; });

        medianValue = median->x;
        median = partition(begin, end,
                           [medianValue](const UTMNode &p) { return p.x < medianValue;});
    }
    else
    {
        nth_element(begin, median, end,
                    [](const UTMNode &p1, const UTMNode &p2) { return p1.y < p2.y; });

        medianValue = median->y;
        median = partition(begin, end,
                           [medianValue](const UTMNode &p) { return p.y < medianValue;});
    }

    TreeNode *left = build(begin, median+1, depth+1);
    TreeNode *right = build(median+1, end, depth+1);
    return new TreeNode(left, right, type, medianValue);
}


list<UTMNode> RangeTree::searchTree(TreeNode *root, const Range &query, const Range &range)
{
    if (root->type == LEAF)
    {
        if (query.contains(root->point))
            return {root->point};
        return {};
    }


    bool vertical = root->type == VERTICAL;
    double line = root->line();
    TreeNode *left = root->left;
    TreeNode *right = root->right;
    list<UTMNode> result;

    Range leftRange = vertical ? range.left(line) : range.below(line); // if current node is vertical split, left child is what's to the left of line
    Range rightRange = vertical ? range.right(line) : range.above(line); // if current node is a horizontal vertical split, left child is what's to below the line

    // left child completely inside
    if (leftRange.inside(query))
        result.splice(result.end(), points(left));
    else if (query.intersects(leftRange))
        result.splice(result.end(), searchTree(left, query, leftRange));

    if (rightRange.inside(query))
        result.splice(result.end(), points(right));
    else if (query.intersects(rightRange))
        result.splice(result.end(), searchTree(right, query, rightRange));

    return result;
}


std::list<UTMNode> RangeTree::points(TreeNode *root)
{
    if (root->type == LEAF)
        return {root->point};
    auto left = points(root->left);
    left.splice(left.end(), points(root->right));
    return left;
}



list<UTMNode> RangeTree::search(const UTMNode &q, double w, double h)
{
    assert(w > 0 && h > 0);
    return searchTree(m_root,
                      Range(q.x, q.x + w, q.y, q.y + h),
                      Range(DBL_MIN, DBL_MAX, DBL_MIN, DBL_MAX));
}



