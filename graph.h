#ifndef GRAPH_H
#define GRAPH_H

#include <string>
#include <unordered_map>
#include <vector>

// Represents the flight network as an adjacency list.
// Key = city name, Value = list of cities you can fly to directly.
class Graph {
public:
    // Loads routes from a file (one "from city" and destinations per line)
    void loadFromFile(const std::string& filename);

    // Returns the adjacency list (all neighbors of a city)
    std::vector<std::string> getNeighbors(const std::string& city) const;

    // Returns true if the city exists in the graph
    bool hasCity(const std::string& city) const;

    // Prints all cities and their routes (useful for debugging)
    void printGraph() const;

private:
    // adjacencyList["Tokyo"] = {"Seoul", "Hong Kong", ...}
    std::unordered_map<std::string, std::vector<std::string>> adjacencyList;
};

#endif