#include <iostream>
#include <vector>
#include <queue>
#include <omp.h>

using namespace std;

// Graph class representing the adjacency list
class Graph {
    int V;  // Number of vertices
    vector<vector<int>> adj;  // Adjacency list

public:
    Graph(int V) : V(V), adj(V) {}

    // Add an edge to the graph
    void addEdge(int v, int w) {
        adj[v].push_back(w);
    }

    // Parallel Depth-First Search
    void parallelDFS(int startVertex) {
        vector<bool> visited(V, false);
        parallelDFSUtil(startVertex, visited);
    }

    // Parallel DFS utility function
    void parallelDFSUtil(int v, vector<bool>& visited) {
        visited[v] = true;
        cout << v << " ";

        #pragma omp parallel for
        for (int i = 0; i < adj[v].size(); ++i) {
            int n = adj[v][i];
            if (!visited[n])
                parallelDFSUtil(n, visited);
        }
    }

};

int main() {
    // Example input (commented out)
    /*
    Enter the number of vertices: 7
    Enter the number of edges: 6
    Enter the edges (vertex pairs):
    0 1
    0 2
    1 3
    1 4
    2 5
    2 6
    Enter the start vertex for DFS and BFS: 0
    */

    // Uncomment the following lines to use the example input
    /*
    int numVertices = 7, numEdges = 6;
    Graph g(numVertices);
    g.addEdge(0, 1);
    g.addEdge(0, 2);
    g.addEdge(1, 3);
    g.addEdge(1, 4);
    g.addEdge(2, 5);
    g.addEdge(2, 6);
    int startVertex = 0;
    */

    // Or input your own values:
    int numVertices, numEdges;
    cout << "Enter the number of vertices: ";
    cin >> numVertices;
    cout << "Enter the number of edges: ";
    cin >> numEdges;

    // Create a graph
    Graph g(numVertices);

    // Input edges
    cout << "Enter the edges (vertex pairs):" << endl;
    for (int i = 0; i < numEdges; ++i) {
        int v, w;
        cin >> v >> w;
        g.addEdge(v, w);
    }

    // Input start vertex
    int startVertex;
    cout << "Enter the start vertex for DFS: ";
    cin >> startVertex;

    // Perform DFS and BFS
    cout << "Depth-First Search (DFS): ";
    g.parallelDFS(startVertex);
    cout << endl;

    return 0;
}


