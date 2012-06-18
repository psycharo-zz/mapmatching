#ifndef LEARNING_H
#define LEARNING_H

#include "graph.h"
#include "input.h"
#include "spatial.h"



namespace mmatch {


static const double ANGLE_WEIGHT = 0.5;
static const double DIST_WEIGHT = 0.5;

// maximum error in distance
static const double MAX_ERROR_LOCAL = 2000;
static const double MIN_ERROR_LOCAL = 50;



// maxError
//! compute maximum distance from the graph to gps coordinate
double maxDistance(const RoadGraph &graph, const Input &input, const Output &output);

//! compute average distance
double avgDistance(const RoadGraph &graph, const Input &input, const Output &output);


//! incremental algorithm
Output match(const RoadGraph &graph, ISpatialIndex *index, const Input &input);

//! backtracing algo
Output backtracing_match(const RoadGraph &graph, ISpatialIndex *tree, const Input &input, const double max_error = MAX_ERROR_LOCAL);

//! backtracking + exp search
Output backtracing_match_smart(const RoadGraph &graph, ISpatialIndex *tree, const Input &input);

} // namespace mmatch



#endif // LEARNING_H
