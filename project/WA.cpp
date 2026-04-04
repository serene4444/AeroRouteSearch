/***********************************************************
  Synopsis: read from city.name and flight.txt
            store the graph in a adjacency matrix
***********************************************************/

#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <map>
#include <functional>
#include <queue>
#include <algorithm>

using namespace std;

struct strCmp {
    bool operator()(const string s1, const string s2) const {
        return s1 < s2;
    }
};

class Graph{
     public:
          Graph(int d);
          ~Graph(){};

          void print();
          void addEdge(int node1, int node2){ adj[node1][node2] = 1; };
        bool hasEdge(int node1, int node2) const { return adj[node1][node2] == 1; }
        int size() const { return n; }
     private: 
          int n;
          int adj[140][140]; // You may want to use adjacent list implementation for sparse graph instead
};

Graph::Graph(int d){
    for (int i=0; i < d; i++){
        for (int j=0; j < d; j++){
            adj[i][j] = 0;
        }
    }
    n = d;
};

void Graph::print(){
    for (int i=0; i < n; i++){
        cout << i << " -> ";
        for (int j=0; j < n; j++){
            if (adj[i][j]){
                cout << j << " ";
            }
        }
        cout << endl;
    }

};

static string cityNameById(int id, const map<const string, int, strCmp>& city){
    for (auto it = city.begin(); it != city.end(); it++){
        if (it->second == id) return it->first;
    }
    return "Unknown";
}

static string formatPathById(const vector<int>& route, const map<const string, int, strCmp>& city) {
    string output;
    for (size_t i = 0; i < route.size(); i++) {
        if (i > 0) output += " to ";
        output += cityNameById(route[i], city);
    }
    return output;
}

static vector<int> shortestPathById(const Graph& graph, int startID, int endID, int n) {
    if (startID < 0 || startID >= n || endID < 0 || endID >= n) {
        return {};
    }

    vector<bool> visited(n, false);
    vector<int> parent(n, -1);
    queue<int> q;

    visited[startID] = true;
    q.push(startID);

    while (!q.empty()) {
        int current = q.front();
        q.pop();

        if (current == endID) {
            break;
        }

        for (int next = 0; next < n; next++) {
            if (graph.hasEdge(current, next) && !visited[next]) {
                visited[next] = true;
                parent[next] = current;
                q.push(next);
            }
        }
    }

    if (!visited[endID]) {
        return {};
    }

    vector<int> path;
    for (int cur = endID; cur != -1; cur = parent[cur]) {
        path.push_back(cur);
    }
    reverse(path.begin(), path.end());
    return path;
}

/***********************************************************
   Q3 - Serene Plummer
   1. Start at City A
   2. Visit every city reachable from City A (directed flight edges)
   3. Return to City A at the end
   4. Do it with as few connections (hops) as possible
***********************************************************/

void SolveQ3(Graph& graph, int startID, int n, map<const string, int, strCmp>& city){
    if (startID < 0 || startID >= n || n > graph.size()){
        cout << "Q3 error: invalid start city or graph size." << endl;
        return;
    }

    vector<bool> reachable(n, false);
    function<void(int)> markReachable = [&](int current){
        reachable[current] = true;
        for (int next = 0; next < n; next++){
            if (graph.hasEdge(current, next) && !reachable[next]){
                markReachable(next);
            }
        }
    };
    markReachable(startID);

    int numReachable = 0;
    for (int i = 0; i < n; i++){
        if (reachable[i]) numReachable++;
    }

    vector<bool> visited(n, false);
    vector<int> route;

    function<bool(int, int)> dfs = [&](int current, int visitedCount){
        route.push_back(current);
        visited[current] = true;

        // Base case: visited all reachable cities and can return to start.
        if (visitedCount == numReachable){
            if (graph.hasEdge(current, startID)){
                route.push_back(startID);
                return true;
            }
            visited[current] = false;
            route.pop_back();
            return false;
        }

        for (int next = 0; next < n; next++){
            if (reachable[next] && graph.hasEdge(current, next) && !visited[next]){
                if (dfs(next, visitedCount + 1)){
                    return true;
                }
            }
        }

        visited[current] = false;
        route.pop_back();
        return false;
    };

    bool found = dfs(startID, 1);

    cout << endl << "Q3 Route from City " << cityNameById(startID, city) << ": " << endl;
    if (!found){
        cout << "No route visits all reachable cities and returns to start." << endl;
        return;
    }

    for (size_t i = 0; i < route.size(); i++){
        cout << cityNameById(route[i], city);
        if (i + 1 < route.size()) cout << " -> ";
    }
    cout << endl;
    cout << "Smallest number of connections (hops): " << route.size() - 1 << endl;
}


/***********************************************************
   Q4 - Serene Plummer
   Find a city different from A/B/C that minimizes
   the total number of connections from A, B, and C.
***********************************************************/
void SolveQ4(Graph& graph,
             int cityA,
             int cityB,
             int cityC,
             int n,
             map<const string, int, strCmp>& city) {
    if (cityA < 0 || cityB < 0 || cityC < 0 ||
        cityA >= n || cityB >= n || cityC >= n) {
        cout << "There is no such a city." << endl;
        return;
    }

    if (cityA == cityB || cityA == cityC || cityB == cityC) {
        cout << "There is no such a city." << endl;
        return;
    }

    bool found = false;
    int bestMeeting = -1;
    int bestTotal = 0;
    vector<int> bestPathA;
    vector<int> bestPathB;
    vector<int> bestPathC;

    for (int meet = 0; meet < n; meet++) {
        if (meet == cityA || meet == cityB || meet == cityC) {
            continue;
        }

        vector<int> pathA = shortestPathById(graph, cityA, meet, n);
        vector<int> pathB = shortestPathById(graph, cityB, meet, n);
        vector<int> pathC = shortestPathById(graph, cityC, meet, n);

        if (pathA.empty() || pathB.empty() || pathC.empty()) {
            continue;
        }

        int total = static_cast<int>((pathA.size() - 1) + (pathB.size() - 1) + (pathC.size() - 1));

        if (!found || total < bestTotal ||
            (total == bestTotal && cityNameById(meet, city) < cityNameById(bestMeeting, city))) {
            found = true;
            bestMeeting = meet;
            bestTotal = total;
            bestPathA = pathA;
            bestPathB = pathB;
            bestPathC = pathC;
        }
    }

    if (!found) {
        cout << "There is no such a city." << endl;
        return;
    }

    cout << "You three should meet at " << cityNameById(bestMeeting, city) << endl;
    cout << "Route for first person: " << formatPathById(bestPathA, city)
         << " (" << bestPathA.size() - 1 << " connections)" << endl;
    cout << "Route for second person: " << formatPathById(bestPathB, city)
         << " (" << bestPathB.size() - 1 << " connections)" << endl;
    cout << "Route for third person: " << formatPathById(bestPathC, city)
         << " (" << bestPathC.size() - 1 << " connections)" << endl;
    cout << "Total number of connection: " << bestTotal << endl;
}



/*to map a city name with an unique integer */
map<const string, int, strCmp> city;

int main(int argc, char *argv[]){
   int i,j, node1, node2,n;
   string line;


   cout << "Please enter the number of cities in your graph: " << endl;
   cout << "---------------------------------------------------" << endl;
   cin >> n;

   Graph graph(n);
 
   char lineChar[256];

   ifstream cityFile;
   cityFile.open("city.name", ios::in);

   for (i = 0; i < n; i++){
      cityFile.getline(lineChar,256);
      line = lineChar;
      city[line] = i;
      cout << line << " is numbered as city " << city[line]<< endl;
   }

   cityFile.close();

  /* initialize adjacent matrix*/

   ifstream flightFile;
   flightFile.open("flight.txt", ios::in);

   while (flightFile.getline(lineChar, 256)){
      /* if line constains From: */
      line = lineChar;

      if (line.find("From:", 0) == 0 ){
          line.erase(0,7);
          node1 = city[line];
      } else {
          line.erase(0,7);
          node2 = city[line];
          graph.addEdge(node1,node2);
      }
   }
   flightFile.close();

   /* print the graph */
   cout << endl << "The graph generated can be represented by the following adjacent matrix : " << endl;
   cout << "-----------------------------------------------------------------------------------" << endl;
   graph.print();

    if (argc < 2) {
        cout << "Usage: ./WA <question#> [args...]" << endl;
        cout << "  Q3: ./WA 3 <city_A>" << endl;
        cout << "  Q4: ./WA 4 <city_A> <city_B> <city_C>" << endl;
        return 1;
    }

    int question = atoi(argv[1]);
    if (question == 3) {
        if (argc < 3) {
            cout << "Usage: ./WA 3 <city_A>" << endl;
            return 1;
        }

        string cityA = argv[2];
        auto itA = city.find(cityA);
        if (itA == city.end()) {
            cout << "No route: start city not found." << endl;
            return 0;
        }
        SolveQ3(graph, itA->second, n, city);
    } else if (question == 4) {
        if (argc < 5) {
            cout << "Usage: ./WA 4 <city_A> <city_B> <city_C>" << endl;
            return 1;
        }

        string cityA = argv[2];
        string cityB = argv[3];
        string cityC = argv[4];

        auto itA = city.find(cityA);
        auto itB = city.find(cityB);
        auto itC = city.find(cityC);
        if (itA == city.end() || itB == city.end() || itC == city.end()) {
            cout << "There is no such a city." << endl;
            return 0;
        }

        SolveQ4(graph, itA->second, itB->second, itC->second, n, city);
    } else {
        cout << "Unsupported question number. Use 3 or 4." << endl;
        return 1;
    }

    return 0;
}
