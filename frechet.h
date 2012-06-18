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


static const size_t NN_NUMBER_GLOBAL = 10000;
static const double MAX_ERROR_GLOBAL = 1200;


//! frechet-based algorithm
Output match_frechet(const RoadGraph &graph, ISpatialIndex *index, const Input &input, const double max_error = MAX_ERROR_GLOBAL);

//! frechet-based algorithm, based on the output, can re-split the input into several parts
Output match_frechet_smart(const RoadGraph &graph, ISpatialIndex *index, const Input &input);



////! weak frechet-based
//Output match_frechet_weak(const RoadGraph &graph, ISpatialIndex *index, const Input &input);



} // namespace mmatch


#endif // FRECHET_H
