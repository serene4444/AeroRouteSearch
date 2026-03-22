#ifndef SEARCH_H
#define SEARCH_H

#include "graph.h"
#include <string>
#include <vector>

// Q1: Find the shortest path from A to B with fewer than x connections.
// Returns the path as a vector of city names, or empty if none found.
std::vector<std::string> question1(const Graph& g,
                                   const std::string& cityA,
                                   const std::string& cityB,
                                   int maxConnections);

// Q2-Q4 stubs — you'll implement these next
std::vector<std::string> question2(const Graph& g,
                                   const std::string& cityA,
                                   const std::string& cityB,
                                   const std::string& cityC,
                                   const std::string& cityD);

// Q3 and Q4 declarations go here...

#endif