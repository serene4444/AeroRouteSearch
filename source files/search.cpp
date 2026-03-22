#include "search.h"
#include <queue>
#include <unordered_map>
#include <iostream>

// BFS finds the shortest path (fewest connections) between two cities.
// We also check that the path length is under maxConnections.
std::vector<std::string> question1(const Graph& g,
                                   const std::string& cityA,
                                   const std::string& cityB,
                                   int maxConnections) {

    // Each queue entry stores: current city + path taken so far
    std::queue<std::pair<std::string, std::vector<std::string>>> bfsQueue;
    std::unordered_map<std::string, bool> visited;

    // Start BFS from cityA
    bfsQueue.push({cityA, {cityA}});
    visited[cityA] = true;

    while (!bfsQueue.empty()) {
        auto [currentCity, currentPath] = bfsQueue.front();
        bfsQueue.pop();

        // Number of connections = path length - 1
        int connections = (int)currentPath.size() - 1;

        // If we've already exceeded the limit, skip this branch
        if (connections >= maxConnections) continue;

        // Check all direct flights from currentCity
        for (const std::string& neighbor : g.getNeighbors(currentCity)) {
            if (visited[neighbor]) continue;

            std::vector<std::string> newPath = currentPath;
            newPath.push_back(neighbor);

            // Found the destination!
            if (neighbor == cityB) {
                return newPath;
            }

            visited[neighbor] = true;
            bfsQueue.push({neighbor, newPath});
        }
    }

    // No path found within the connection limit
    return {};
}