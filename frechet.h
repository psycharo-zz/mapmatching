#ifndef FRECHET_H
#define FRECHET_H

#include <cstdlib>
#include <vector>
#include <map>
#include <cassert>
#include <geometry.h>


#include <input.h>
#include <spatial.h>




using namespace std;


namespace mmatch
{
class Interval;

typedef vector<Interval> Chain;

typedef Chain::iterator IntervalIt;



struct IntervalPos
{
    // the position on the route (k) (number of the cell)
    size_t cell;
    // the parameter on the current edge [0,1]
    float param;

    IntervalPos(size_t _pos, float param)
    {}

    bool operator<(const IntervalPos &other) const
    {
        return (cell == other.cell) ? (param > other.param) : (cell > other.cell);
    }

};

struct Interval
{
    // the id to which this interval corresponds
    // this is the index of the chain in chain vector
    size_t node;

    IntervalPos left;
    IntervalPos right;


    // the left pointers
    map<size_t, IntervalPos> l_ptrs;
    // the right pointers
    map<size_t, IntervalPos> r_ptrs;

    bool operator <(const Interval &other) const
    {
        // TODO: CHECK ME: ORDER
        return left < other.left;
    }
};


// get a position on a polygonal curve for given parameters
inline UTMNode param(const std::vector<UTMNode> &curve, size_t i, double t)
{
//    assert(t >= 0 && t <= 1 && i < curve.size());
    return curve[i] * (1 - t) + curve[i+1] * t;
}



inline std::ostream &operator<<(std::ostream &os, const IntervalPos &i)
{
    os << "(" << i.cell << "," << i.param << ")";
}

inline std::ostream &operator<<(std::ostream &os, const Interval &i)
{
    os << i.left << " : " << i.right;
}




//! frechet-based algorithm
Output match_frechet(const RoadGraph &graph, ISpatialIndex *index, const Input &input);



} // namespace mmatch


#endif // FRECHET_H
