#include <iostream>
#include <string>
#include "graph.h"
#include "search.h"

// Print a styled header
void printHeader(const std::string& title) {
    std::cout << "\n" << std::string(70, '=') << std::endl;
    std::cout << "  " << title << std::endl;
    std::cout << std::string(70, '=') << std::endl;
}

// Print a separator line
void printSeparator() {
    std::cout << std::string(70, '-') << std::endl;
}

// Helper to print a path with arrows
void printPath(const std::vector<std::string>& path) {
    std::cout << "\n  Route: ";
    for (int i = 0; i < (int)path.size(); i++) {
        if (i > 0) std::cout << " --> ";
        std::cout << path[i];
    }
    std::cout << "\n" << std::endl;
}

int main(int argc, char* argv[]) {
    printHeader("GLOBAL FLIGHT ROUTE SEARCH");
    
    if (argc < 2) {
        std::cerr << "\n  ERROR: Missing question number\n" << std::endl;
        std::cerr << "  Usage: routeSearch <question#> [args...]\n" << std::endl;
        return 1;
    }

    // Load the flight graph
    Graph g;
    g.loadFromFile("flight.txt");

    int question = std::stoi(argv[1]);

    if (question == 1) {
        // routeSearch 1 <cityA> <cityB> <maxConnections>
        if (argc < 5) {
            std::cerr << "\n  ERROR: Missing required arguments\n" << std::endl;
            std::cerr << "  Usage: routeSearch 1 <city_A> <city_B> <max_connections>\n" << std::endl;
            return 1;
        }
        std::string cityA = argv[2];
        std::string cityB = argv[3];
        int maxConn = std::stoi(argv[4]);

        std::cout << "\n  QUESTION 1: Shortest Path Search" << std::endl;
        std::cout << "  " << std::string(50, '-') << std::endl;
        std::cout << "  From: " << cityA << std::endl;
        std::cout << "  To:   " << cityB << std::endl;
        std::cout << "  Max Connections: " << maxConn << std::endl;
        printSeparator();

        std::vector<std::string> path = question1(g, cityA, cityB, maxConn);
        if (path.empty()) {
            std::cout << "\n   No route found with fewer than " << maxConn << " connections.\n" << std::endl;
        } else {
            std::cout << "\n   Route Found!" << std::endl;
            printPath(path);
            std::cout << "  Total Hops: " << path.size() - 1 << std::endl;
            std::cout << std::string(70, '=') << "\n" << std::endl;
        }
    } else if (question == 3) {
        // Question 3 - Serene Plummer
        // Find a route that visits all reachable cities and returns to start with minimal connections
        if (argc < 3) {
            std::cerr << "\n  ERROR: Missing required arguments\n" << std::endl;
            std::cerr << "  Usage: routeSearch 3 <city_A>\n" << std::endl;
            return 1;
        }

        std::string startCity = argv[2];
        
        std::cout << "\n  QUESTION 3: Hamiltonian Cycle (Serene Plummer)" << std::endl;
        std::cout << "  " << std::string(50, '-') << std::endl;
        std::cout << "  Starting City: " << startCity << std::endl;
        std::cout << "  Objective: Visit all reachable cities and return" << std::endl;
        printSeparator();

        // Show reachable cities
        std::vector<std::string> reachable = getReachableCities(g, startCity);
        if (reachable.empty()) {
            std::cout << "\n  ERROR: Starting city not found in graph.\n" << std::endl;
            return 1;
        }

        std::cout << "\n  Reachable Cities (" << reachable.size() << " total):" << std::endl;
        std::cout << "  " << std::string(50, '-') << std::endl;
        for (size_t i = 0; i < reachable.size(); i++) {
            std::cout << "    " << (i + 1) << ". " << reachable[i] << std::endl;
        }
        printSeparator();
        
        std::vector<std::string> route = question3(g, startCity);

        if (route.empty()) {
            std::cout << "\n   No valid route found." << std::endl;
            std::cout << "  Could not visit all reachable cities and return to start.\n" << std::endl;
        } else {
            std::cout << "\n   Route Found!" << std::endl;
            printPath(route);
            std::cout << "  Total Hops: " << route.size() - 1 << std::endl;
            std::cout << std::string(70, '=') << "\n" << std::endl;
        }
    } else {
        std::cerr << "\n  ERROR: Unsupported question number: " << question << std::endl;
        std::cerr << "  Supported: 1 (shortest path), 3 (hamiltonian cycle)\n" << std::endl;
        return 1;
    }

    return 0;
}