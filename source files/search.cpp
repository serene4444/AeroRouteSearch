#include "search.h"
#include <queue>
#include <unordered_map>
#include <unordered_set>
#include <iostream>
#include <functional>
#include <algorithm>
#include <limits>

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

    std::vector<std::string> reachable = getReachableCities(g, cityA);
    if (reachable.empty()) {
        return {};
    }

    if (reachable.size() == 1) {
        return {cityA, cityA};
    }

    std::unordered_map<std::string, int> indexOf;
    for (int i = 0; i < static_cast<int>(reachable.size()); ++i) {
        indexOf[reachable[i]] = i;
    }

    auto startIt = indexOf.find(cityA);
    if (startIt == indexOf.end()) {
        return {};
    }

    const int startIndex = startIt->second;
    const int cityCount = static_cast<int>(reachable.size());
    const int maskCount = 1 << cityCount;
    const int inf = std::numeric_limits<int>::max() / 4;

    std::vector<std::vector<int>> dist(cityCount, std::vector<int>(cityCount, inf));
    std::vector<std::vector<std::vector<std::string>>> shortestPaths(
        cityCount, std::vector<std::vector<std::string>>(cityCount)
    );

    for (int i = 0; i < cityCount; ++i) {
        dist[i][i] = 0;
        shortestPaths[i][i] = {reachable[i]};

        for (int j = 0; j < cityCount; ++j) {
            if (i == j) {
                continue;
            }

            std::vector<std::string> path = g.findShortestPath(reachable[i], reachable[j]);
            if (!path.empty()) {
                dist[i][j] = static_cast<int>(path.size()) - 1;
                shortestPaths[i][j] = path;
            }
        }
    }

    std::vector<std::vector<int>> dp(maskCount, std::vector<int>(cityCount, inf));
    std::vector<std::vector<int>> parentMask(maskCount, std::vector<int>(cityCount, -1));
    std::vector<std::vector<int>> parentCity(maskCount, std::vector<int>(cityCount, -1));

    const int startMask = 1 << startIndex;
    dp[startMask][startIndex] = 0;

    for (int mask = 0; mask < maskCount; ++mask) {
        if ((mask & startMask) == 0) {
            continue;
        }

        for (int current = 0; current < cityCount; ++current) {
            if (dp[mask][current] >= inf) {
                continue;
            }

            for (int next = 0; next < cityCount; ++next) {
                if (mask & (1 << next)) {
                    continue;
                }

                if (dist[current][next] >= inf) {
                    continue;
                }

                int nextMask = mask | (1 << next);
                int candidateCost = dp[mask][current] + dist[current][next];

                if (candidateCost < dp[nextMask][next]) {
                    dp[nextMask][next] = candidateCost;
                    parentMask[nextMask][next] = mask;
                    parentCity[nextMask][next] = current;
                }
            }
        }
    }

    int fullMask = maskCount - 1;
    int bestCost = inf;
    int bestEnd = -1;

    for (int end = 0; end < cityCount; ++end) {
        if (dp[fullMask][end] >= inf || dist[end][startIndex] >= inf) {
            continue;
        }

        int totalCost = dp[fullMask][end] + dist[end][startIndex];
        if (totalCost < bestCost) {
            bestCost = totalCost;
            bestEnd = end;
        }
    }

    if (bestEnd == -1) {
        return {};
    }

    std::vector<int> visitOrder;
    int mask = fullMask;
    int current = bestEnd;

    while (current != -1) {
        visitOrder.push_back(current);
        int previousMask = parentMask[mask][current];
        int previousCity = parentCity[mask][current];
        mask = previousMask;
        current = previousCity;
    }

    std::reverse(visitOrder.begin(), visitOrder.end());
    visitOrder.push_back(startIndex);

    std::vector<std::string> route;
    for (size_t i = 0; i + 1 < visitOrder.size(); ++i) {
        const std::vector<std::string>& segment = shortestPaths[visitOrder[i]][visitOrder[i + 1]];
        if (segment.empty()) {
            return {};
        }

        if (route.empty()) {
            route = segment;
        } else {
            route.insert(route.end(), segment.begin() + 1, segment.end());
        }
    }

    return route;
}

Question4Result question4(const Graph& g,
                         const std::string& cityA,
                         const std::string& cityB,
                         const std::string& cityC) {
    Question4Result best;

    if (!g.hasCity(cityA) || !g.hasCity(cityB) || !g.hasCity(cityC)) {
        return best;
    }

    if (cityA == cityB || cityA == cityC || cityB == cityC) {
        return best;
    }

    std::vector<std::string> allCities = g.getAllCities();
    bool hasCandidate = false;
    int bestConnections = 0;

    for (const std::string& candidate : allCities) {
        if (candidate == cityA || candidate == cityB || candidate == cityC) {
            continue;
        }

        std::vector<std::string> pathA = g.findShortestPath(cityA, candidate);
        std::vector<std::string> pathB = g.findShortestPath(cityB, candidate);
        std::vector<std::string> pathC = g.findShortestPath(cityC, candidate);

        if (pathA.empty() || pathB.empty() || pathC.empty()) {
            continue;
        }

        int total = static_cast<int>(pathA.size() - 1 + pathB.size() - 1 + pathC.size() - 1);

        if (!hasCandidate || total < bestConnections ||
            (total == bestConnections && candidate < best.meetingCity)) {
            hasCandidate = true;
            bestConnections = total;

            best.found = true;
            best.meetingCity = candidate;
            best.routeA = pathA;
            best.routeB = pathB;
            best.routeC = pathC;
            best.totalConnections = total;
        }
    }

    return best;
}