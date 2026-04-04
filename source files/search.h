#ifndef SEARCH_H
#define SEARCH_H

#include "graph.h"
#include <string>
#include <vector>

struct Question4Result {
    bool found = false;
    std::string meetingCity;
    std::vector<std::string> routeA;
    std::vector<std::string> routeB;
    std::vector<std::string> routeC;
    int totalConnections = 0;
};

// Q1: Find the shortest path from A to B with fewer than x connections.
// Returns the path as a vector of city names, or empty if none found.
std::vector<std::string> question1(const Graph& g,
                                   const std::string& cityA,
                                   const std::string& cityB,
                                   int maxConnections);

// Q2: Shortest path from A to D through B and C.
std::vector<std::string> question2(const Graph& g,
                                   const std::string& cityA,
                                   const std::string& cityB,
                                   const std::string& cityC,
                                   const std::string& cityD);

// Q3 (Serene Plummer) - Find all cities reachable from a starting city
std::vector<std::string> getReachableCities(const Graph& g, const std::string& cityA);

// Q3 (Serene Plummer) - Find Hamiltonian cycle visiting all reachable cities
std::vector<std::string> question3(const Graph& g, const std::string& cityA);

//Q4 (Serene Plummer) Find meeting city minimizing total connections for 3 people.
Question4Result question4(const Graph& g,
                         const std::string& cityA,
                         const std::string& cityB,
                         const std::string& cityC);

#endif