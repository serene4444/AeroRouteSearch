#include "graph.h"
#include <fstream>
#include <sstream>
#include <iostream>

// Reads flight.txt and builds the adjacency list.
// Expected format: each line is "CityA CityB CityC ..."
// where CityA can fly directly to CityB, CityC, etc.
void Graph::loadFromFile(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Error: could not open " << filename << std::endl;
        return;
    }

    std::string line;
    while (std::getline(file, line)) {
        std::istringstream ss(line);
        std::string fromCity, toCity;

        // First token is the source city
        ss >> fromCity;
        if (fromCity.empty()) continue;

        // Remaining tokens are destination cities
        while (ss >> toCity) {
            adjacencyList[fromCity].push_back(toCity);
        }
    }
    file.close();
}

std::vector<std::string> Graph::getNeighbors(const std::string& city) const {
    auto it = adjacencyList.find(city);
    if (it != adjacencyList.end()) {
        return it->second;
    }
    return {}; // empty if city not found
}

bool Graph::hasCity(const std::string& city) const {
    return adjacencyList.count(city) > 0;
}

void Graph::printGraph() const {
    for (auto& entry : adjacencyList) {
        std::cout << entry.first << " -> ";
        for (auto& dest : entry.second) {
            std::cout << dest << " ";
        }
        std::cout << std::endl;
    }
}