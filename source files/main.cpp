#include <iostream>
#include <string>
#include "graph.h"
#include "search.h"

// Helper to print a path like: Tokyo to Seoul to London
void printPath(const std::vector<std::string>& path) {
    for (int i = 0; i < (int)path.size(); i++) {
        if (i > 0) std::cout << " to ";
        std::cout << path[i];
    }
    std::cout << std::endl;
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cerr << "Usage: routeSearch <question#> [args...]" << std::endl;
        return 1;
    }

    // Load the flight graph
    Graph g;
    g.loadFromFile("flight.txt");

    int question = std::stoi(argv[1]);

    if (question == 1) {
        // routeSearch 1 <cityA> <cityB> <maxConnections>
        if (argc < 5) {
            std::cerr << "Usage: routeSearch 1 <city_A> <city_B> <num_connection>" << std::endl;
            return 1;
        }
        std::string cityA = argv[2];
        std::string cityB = argv[3];
        int maxConn = std::stoi(argv[4]);

        std::vector<std::string> path = question1(g, cityA, cityB, maxConn);
        if (path.empty()) {
            std::cout << "No route found with less than " << maxConn << " connections." << std::endl;
        } else {
            printPath(path);
            std::cout << "Total connections: " << path.size() - 1 << std::endl;
        }
    }
    // Add else if blocks for questions 2, 3, 4 here.
    // Question 3 owner: Serene Plummer.
    vector<string> route = question3(graph, startCity);
    if (route.empty()){
        cout << "No route found." << endl;
    } else {
        for (size_t i = 0; i < route.size(); i++){
            cout << route[i];
            if (i + 1 < route.size()) cout << " -> ";
        }
        cout << "\nSmallest number of connections (hops): " << route.size() - 1 << endl;
    }
    

    return 0;
}