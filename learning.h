#ifndef LEARNING_H
#define LEARNING_H

#include "graph.h"
#include "input.h"

// maxError
//! compute maximum distance from the graph to gps coordinate
double maxDistance(const RoadGraph &graph, const Input &input, const Output &output);

//! compute average distance
double avgDistance(const RoadGraph &graph, const Input &input, const Output &output);


#endif // LEARNING_H
