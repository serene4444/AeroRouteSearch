/***********************************************************
  Synopsis: read from city.name and flight.txt
            store the graph in a adjacency matrix
***********************************************************/

#include <iostream>
#include <vector>
#include <ctime> 
#include <cstdlib>
#include <string>
#include <fstream>
#include <map>
#include <functional>

using namespace std;

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

    //find city name by id helper 
    auto cityName = [&](int id) -> string{
        for (auto it = city.begin(); it != city.end(); it++){
            if (it->second == id) return it->first;
        return "Unknown";
        }
    };

    cout << endl << "Q3 Route from City " << cityName(startID) << ": " << endl;
    if (!found){
        cout << "No route visits all reachable cities and returns to start." << endl;
        return;
    }

    for (size_t i = 0; i < route.size(); i++){
        cout << route[i];
        if (i + 1 < route.size()) cout << " -> ";
    }
    cout << endl;
    cout << "Smallest number of connections (hops): " << route.size() - 1 << endl;
}




struct strCmp {
    bool operator()( const string s1, const string s2 ) const {
      return s1 <s2;
    }
  };


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

    // Q3 (Serene Plummer): start from City A (ID 0).
    SolveQ3(graph, 0, n);
}
