#include <iostream>
#include <vector>
#include <algorithm>
#include <fstream> 
#include <cstdlib> 

using namespace std;

// Edge structure for network connections
struct Edge {
    int u, v;       // Nodes connected by this edge
    int weight;     // Cost of connection 
    bool operator<(const Edge& other) const {
        return weight < other.weight; // Sort edges by weight for greedy approach
    }
};

// Disjoint Set Union (DSU) for cycle detection
struct DSU {
    vector<int> parent, rank;

    DSU(int n) {
        parent.resize(n);
        rank.resize(n, 0);
        for (int i = 0; i < n; i++) parent[i] = i;
    }

    int find(int u) {
        if (u != parent[u])
            parent[u] = find(parent[u]); // Path compression
        return parent[u];
    }

    bool unionSets(int u, int v) {
        u = find(u);
        v = find(v);
        if (u == v) return false; // Cycle detected
        if (rank[u] < rank[v])
            swap(u, v);
        parent[v] = u;
        if (rank[u] == rank[v])
            rank[u]++;
        return true;
    }
};

// Kruskal's MST algorithm for network design
pair<int, vector<Edge>> kruskalMST(int n, vector<Edge>& edges) {
    sort(edges.begin(), edges.end()); // Sort edges by cost (greedy step)
    DSU dsu(n);

    int mstWeight = 0; // Total cost of the MST
    vector<Edge> mstEdges;

    for (const auto& edge : edges) {
        if (dsu.unionSets(edge.u, edge.v)) { // Connect the nodes if no cycle is formed
            mstWeight += edge.weight;
            mstEdges.push_back(edge);
        }
    }
    return {mstWeight, mstEdges};
}