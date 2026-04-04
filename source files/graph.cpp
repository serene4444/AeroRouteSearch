#include "graph.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <queue>
#include <algorithm>

static std::string trimSpaces(const std::string& text) {
    size_t start = text.find_first_not_of(" \t");
    if (start == std::string::npos) {
        return "";
    }

    size_t end = text.find_last_not_of(" \t");
    return text.substr(start, end - start + 1);
}

// Reads flight.txt file & builds adjacency list.
void Graph::loadFromFile(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Error: could not open " << filename << std::endl;
        return;
    }

    adjacencyList.clear();

    std::string line;
    std::string fromCity;

    while (std::getline(file, line)) {
        if (line.empty()) {
            continue;
        }

        // If line starts with "From:" then store the current source city
        if (line.find("From:") == 0) {
            fromCity = trimSpaces(line.substr(5));

            if (adjacencyList.find(fromCity) == adjacencyList.end()) {
                adjacencyList[fromCity] = std::vector<std::string>();
            }
        }
        // First destination line starts with "To  :"
        else if (line.find("To") == 0) {
            std::size_t colonPos = line.find(':');
            if (colonPos != std::string::npos) {
                std::string toCity = trimSpaces(line.substr(colonPos + 1));

                if (!fromCity.empty() && !toCity.empty()) {
                    adjacencyList[fromCity].push_back(toCity);

                    if (adjacencyList.find(toCity) == adjacencyList.end()) {
                        adjacencyList[toCity] = std::vector<std::string>();
                    }
                }
            }
        }
        // Remaining indented lines are also destination cities
        else {
            std::string toCity = trimSpaces(line);

            if (!fromCity.empty() && !toCity.empty()) {
                adjacencyList[fromCity].push_back(toCity);

                if (adjacencyList.find(toCity) == adjacencyList.end()) {
                    adjacencyList[toCity] = std::vector<std::string>();
                }
            }
        }
    }

    file.close();
}

std::vector<std::string> Graph::getNeighbors(const std::string& city) const {
    auto it = adjacencyList.find(city);
    if (it != adjacencyList.end()) {
        return it->second;
    }
    return {};
}

// Question 2 helper: get all cities in the graph

std::vector<std::string> Graph::getAllCities() const {
    std::vector<std::string> cities;
    cities.reserve(adjacencyList.size());

    for (const auto& entry : adjacencyList) {
        cities.push_back(entry.first);
    }

    std::sort(cities.begin(), cities.end());
    return cities;
}

bool Graph::hasCity(const std::string& city) const {
    return adjacencyList.count(city) > 0;
}

void Graph::printGraph() const {
    for (auto& entry : adjacencyList) {
        std::cout << entry.first << " -> ";
        for (auto& dest : entry.second) {
            std::cout << dest << " | ";
        }
        std::cout << std::endl;
    }
}

/***********************************************************
   Q1 - Sharvani Venkatesan
   1. Start at City A
   2. Use BFS to search for City B
   3. BFS gives the route with the smallest number of hops
   4. Check if that route uses less than or equal to x connections
   5. Print the route or say there is no such route
***********************************************************/
std::vector<std::string> Graph::findShortestPath(const std::string& startCity, const std::string& endCity) const {
    std::vector<std::string> path;

    if (!hasCity(startCity) || !hasCity(endCity)) {
        return path;
    }

    std::queue<std::string> q;
    std::unordered_map<std::string, bool> visited;
    std::unordered_map<std::string, std::string> parent;

    q.push(startCity);
    visited[startCity] = true;

    while (!q.empty()) {
        std::string currentCity = q.front();
        q.pop();

        if (currentCity == endCity) {
            break;
        }

        auto it = adjacencyList.find(currentCity);
        if (it != adjacencyList.end()) {
            for (const std::string& neighbor : it->second) {
                if (!visited[neighbor]) {
                    visited[neighbor] = true;
                    parent[neighbor] = currentCity;
                    q.push(neighbor);
                }
            }
        }
    }

    if (!visited[endCity]) {
        return {};
    }

    std::string currentCity = endCity;
    while (currentCity != startCity) {
        path.push_back(currentCity);
        currentCity = parent[currentCity];
    }

    path.push_back(startCity);
    std::reverse(path.begin(), path.end());

    return path;
}

void Graph::question1(const std::string& cityA, const std::string& cityB, int x) const {
    std::vector<std::string> path = findShortestPath(cityA, cityB);

    if (path.empty()) {
        std::cout << "There is no such route." << std::endl;
        return;
    }

    int connections = path.size() - 1;

    if (connections <= x) {
        std::cout << "Smallest route: ";
        for (size_t i = 0; i < path.size(); i++) {
            std::cout << path[i];
            if (i != path.size() - 1) {
                std::cout << " -> ";
            }
        }
        std::cout << std::endl;
        std::cout << "Connections: " << connections << std::endl;
    } else {
        std::cout << "There is no such route." << std::endl;
    }
}

/***********************************************************
   Q2 - Sharvani Venkatesan
   1. Start at City A
   2. Must go through City B and City C
   3. The order of B and C does not matter
   4. Check both possible orders:
         A -> B -> C -> D
         A -> C -> B -> D
   5. Use BFS on each part, combine the routes,
      and print the one with the fewest connections
***********************************************************/
void Graph::question2(const std::string& cityA, const std::string& cityB, const std::string& cityC, const std::string& cityD) const {
    std::vector<std::string> pathAB = findShortestPath(cityA, cityB);
    std::vector<std::string> pathBC = findShortestPath(cityB, cityC);
    std::vector<std::string> pathCD = findShortestPath(cityC, cityD);

    std::vector<std::string> pathAC = findShortestPath(cityA, cityC);
    std::vector<std::string> pathCB = findShortestPath(cityC, cityB);
    std::vector<std::string> pathBD = findShortestPath(cityB, cityD);

    std::vector<std::string> route1;
    std::vector<std::string> route2;

    // Build route A -> B -> C -> D
    if (!pathAB.empty() && !pathBC.empty() && !pathCD.empty()) {
        route1 = pathAB;

        for (size_t i = 1; i < pathBC.size(); i++) {
            route1.push_back(pathBC[i]);
        }

        for (size_t i = 1; i < pathCD.size(); i++) {
            route1.push_back(pathCD[i]);
        }
    }

    // Build route A -> C -> B -> D
    if (!pathAC.empty() && !pathCB.empty() && !pathBD.empty()) {
        route2 = pathAC;

        for (size_t i = 1; i < pathCB.size(); i++) {
            route2.push_back(pathCB[i]);
        }

        for (size_t i = 1; i < pathBD.size(); i++) {
            route2.push_back(pathBD[i]);
        }
    }

    if (route1.empty() && route2.empty()) {
        std::cout << "There is no such route." << std::endl;
        return;
    }

    std::vector<std::string> bestRoute;

    if (route1.empty()) {
        bestRoute = route2;
    } else if (route2.empty()) {
        bestRoute = route1;
    } else if (route1.size() <= route2.size()) {
        bestRoute = route1;
    } else {
        bestRoute = route2;
    }

    std::cout << "Smallest route: ";
    for (size_t i = 0; i < bestRoute.size(); i++) {
        std::cout << bestRoute[i];
        if (i != bestRoute.size() - 1) {
            std::cout << " -> ";
        }
    }
    std::cout << std::endl;
    std::cout << "Connections: " << bestRoute.size() - 1 << std::endl;
}