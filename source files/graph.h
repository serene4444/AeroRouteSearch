#ifndef GRAPH_H
#define GRAPH_H

#include <string>
#include <unordered_map>
#include <vector>

// Represents the flight network as adjacency list.
// Key = city name, Value = list of cities you can fly to directly.
class Graph {
public:
    // Loads routes from flight.txt format
    void loadFromFile(const std::string& filename);

    // Return adjacency list (neighbors of city)
    std::vector<std::string> getNeighbors(const std::string& city) const;

    // Returns true if the city exists in the graph
    bool hasCity(const std::string& city) const;

    // Prints all the cities and routes
    void printGraph() const;

    // Q1 helper: find shortest path from start city to end city
    std::vector<std::string> findShortestPath(const std::string& startCity, const std::string& endCity) const;

    // Q1: check if cityA can reach cityB within x connections
    void question1(const std::string& cityA, const std::string& cityB, int x) const;

    // Q2: shortest route from cityA to cityD through cityB and cityC
    void question2(const std::string& cityA, const std::string& cityB, const std::string& cityC, const std::string& cityD) const;

private:
    // adjacencyList
    std::unordered_map<std::string, std::vector<std::string>> adjacencyList;
};

#endif