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

std::string formatPathForQuestion3(const std::vector<std::string>& path) {
    std::string output;

    for (size_t i = 0; i < path.size(); ++i) {
        if (i > 0) {
            output += " to ";
        }
        output += path[i];
    }

    return output;
}

std::string formatPathForQuestion4(const std::vector<std::string>& path) {
    std::string output;

    for (size_t i = 0; i < path.size(); ++i) {
        if (i > 0) {
            output += " to ";
        }
        output += path[i];
    }

    return output;
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
        if (argc < 3) {
            std::cerr << "\n  ERROR: Missing required arguments\n" << std::endl;
            std::cerr << "  Usage: routeSearch 3 <city_A>\n" << std::endl;
            return 1;
        }

        std::string startCity = argv[2];

        std::vector<std::string> route = question3(g, startCity);
        if (route.empty()) {
            std::cout << "There is no such a route." << std::endl;
        } else {
            int connections = static_cast<int>(route.size()) - 1;
            if (route.size() == 2 && route.front() == route.back()) {
                connections = 0;
            }

            std::cout << formatPathForQuestion3(route) << std::endl;
            std::cout << "smallest number of connection: " << connections << std::endl;
        }
    } else if (question == 4) {
        // routeSearch 4 <city_A> <city_B> <city_C>
        if (argc < 5) {
            std::cerr << "\n  ERROR: Missing required arguments\n" << std::endl;
            std::cerr << "  Usage: routeSearch 4 <city_A> <city_B> <city_C>\n" << std::endl;
            return 1;
        }

        std::string cityA = argv[2];
        std::string cityB = argv[3];
        std::string cityC = argv[4];

        Question4Result result = question4(g, cityA, cityB, cityC);

        if (!result.found) {
            std::cout << "There is no such a city." << std::endl;
            return 0;
        }

        std::cout << "You three should meet at " << result.meetingCity << std::endl;
        std::cout << "Route for first person: " << formatPathForQuestion4(result.routeA)
                  << " (" << result.routeA.size() - 1 << " connections)" << std::endl;
        std::cout << "Route for second person: " << formatPathForQuestion4(result.routeB)
                  << " (" << result.routeB.size() - 1 << " connections)" << std::endl;
        std::cout << "Route for third person: " << formatPathForQuestion4(result.routeC)
                  << " (" << result.routeC.size() - 1 << " connections)" << std::endl;
        std::cout << "Total number of connection: " << result.totalConnections << std::endl;
    } else {
        std::cerr << "\n  ERROR: Unsupported question number: " << question << std::endl;
        std::cerr << "  Supported: 1 (shortest path), 3 (hamiltonian cycle), 4 (best meeting city)\n" << std::endl;
        return 1;
    }

    return 0;
}