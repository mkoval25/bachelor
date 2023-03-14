
// classes: Graph, meshGraph
// weight generatee
// use typedefs: Edge, Edgelist etc.

// TODO mean, stddev -> float 
// TODO meshGraph -> GridGraph etc.
// TODO use addEdgeRandom() instd. of addEdgeRandom() +
// TODO move coordsInit to constructor?

// TODO put ringNum() into radialGraph class +
// TODO printGraph doesn't need V param

// Use GitHub? 

#include <iostream>
#include <vector>
#include <random>
#include <queue>
#include <chrono>

using namespace std;

// определили синоним типа - СписокСмежности
typedef  *vector<pair<int, int> > AdjList 

void dijkstra(int source, AdjList adj) {
        vector<int> dist(V, INT_MAX);
        vector<bool> visited(V, false);
        priority_queue<pair<int, int>, vector<pair<int, int>>, greater<pair<int, int>>> pq;

        auto start = std::chrono::high_resolution_clock::now();

        dist[source] = 0;
        pq.push(make_pair(0, source));

        while (!pq.empty()) {
            int u = pq.top().second;
            pq.pop();

            if (visited[u]) {
                continue;
            }

            visited[u] = true;

            for (auto& neighbor : adj[u]) {
                int v = neighbor.first;
                int weight = neighbor.second;

                if (dist[u] + weight < dist[v]) {
                    dist[v] = dist[u] + weight;
                    pq.push(make_pair(dist[v], v));
                }
            }
        }

        auto end = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);

        std::cout << "Dijkstra algorithm execution time: " << duration.count() << " microseconds." << std::endl;
}


int main() { 
    return 0;
}