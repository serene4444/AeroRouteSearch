#include "search.h"
#include <queue>
#include <unordered_map>
#include <unordered_set>
#include <iostream>
#include <functional>
#include <algorithm>

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
                // number of connections in the new path
                int newConnections = (int)newPath.size() - 1;
                // Respect the "fewer than maxConnections" requirement
                if (newConnections < maxConnections) {
                    return newPath;
                } else {
                    // this path reaches the destination but does not satisfy
                    // the strict "fewer than" constraint, so skip it
                    continue;
                }
            }

            visited[neighbor] = true;
            bfsQueue.push({neighbor, newPath});
        }
    }

    // No path found within the connection limit
    return {};
}

// Helper function - Serene Plummer Q3: Find all cities reachable from a starting city
std::vector<std::string> getReachableCities(const Graph& g, const std::string& cityA) {
    if (!g.hasCity(cityA)) {
        return {};
    }

    std::unordered_set<std::string> reachable;
    std::queue<std::string> q;
    q.push(cityA);
    reachable.insert(cityA);

    while (!q.empty()) {
        std::string current = q.front();
        q.pop();

        for (const std::string& next : g.getNeighbors(current)) {
            if (reachable.insert(next).second) {
                q.push(next);
            }
        }
    }

    std::vector<std::string> result(reachable.begin(), reachable.end());
    std::sort(result.begin(), result.end());
    return result;
}

std::vector<std::string> question3(const Graph& g, const std::string& cityA) {
    if (!g.hasCity(cityA)) {
        return {};
    }

    std::unordered_set<std::string> reachable;
    std::queue<std::string> q;
    q.push(cityA);
    reachable.insert(cityA);

    while (!q.empty()) {
        std::string current = q.front();
        q.pop();

        for (const std::string& next : g.getNeighbors(current)) {
            if (reachable.insert(next).second) {
                q.push(next);
            }
        }
    }



    // Serene Plummer - Question 3: Find a route that visits all reachable cities and returns to start, with as few connections as possible. 
    std::unordered_set<std::string> visited;
    std::vector<std::string> path;
    std::vector<std::string> bestPath;

    std::function<void(const std::string&)> dfs = [&](const std::string& current) {
        if (!bestPath.empty() && path.size() >= bestPath.size()) {
            return;
        }

        if (visited.size() == reachable.size()) {
            for (const std::string& next : g.getNeighbors(current)) {
                if (next == cityA) {
                    std::vector<std::string> candidate = path;
                    candidate.push_back(cityA);
                    if (bestPath.empty() || candidate.size() < bestPath.size()) {
                        bestPath = candidate;
                    }
                    break;
                }
            }
            return;
        }

        for (const std::string& next : g.getNeighbors(current)) {
            if (reachable.count(next) && !visited.count(next)) {
                visited.insert(next);
                path.push_back(next);
                dfs(next);
                path.pop_back();
                visited.erase(next);
            }
        }
    };

    visited.insert(cityA);
    path.push_back(cityA);
    dfs(cityA);

    return bestPath;
}