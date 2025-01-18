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

// Function to generate a Graphviz-compatible .dot file for a graph
void generateGraphvizFile(const vector<Edge>& edges, const string& filename, bool isMST = false) {
    ofstream outFile(filename);
    if (!outFile) {
        cerr << "Error opening file for writing: " << filename << "\n";
        return;
    }

    outFile << "graph " << (isMST ? "MST" : "InputGraph") << " {\n";
    for (const auto& edge : edges) {
        outFile << "    " << edge.u << " -- " << edge.v << " [label=\"" << edge.weight << "\"];\n";
    }
    outFile << "}\n";
    outFile.close();
    cout << (isMST ? "MST" : "Input graph") << " file generated: " << filename << "\n";
}

int main() {
    string inputFileName;
    cout << "Enter the input file name: ";
    cin >> inputFileName;

    ifstream inputFile(inputFileName);
    if (!inputFile) {
        cerr << "Error: Unable to open input file.\n";
        return 1;
    }

    int n, m; // Number of nodes (devices or locations) and edges (connections)
    inputFile >> n >> m;

    vector<Edge> edges(m);
    for (int i = 0; i < m; i++) {
        inputFile >> edges[i].u >> edges[i].v >> edges[i].weight;
    }
    inputFile.close();

    // Generate the input graph visualization
    string inputGraphFile = "input_graph.dot";
    generateGraphvizFile(edges, inputGraphFile);

    // Generate the MST using Kruskal's algorithm
    auto [mstWeight, mstEdges] = kruskalMST(n, edges);

    // Display the Minimum Spanning Tree details
    cout << "\nNetwork Design - Minimum Spanning Tree (MST):\n";
    cout << "Connections in the MST:\n";
    for (const auto& edge : mstEdges) {
        cout << "Device " << edge.u << " connected to Device " << edge.v << " with cost " << edge.weight << "\n";
    }
    cout << "Total cost of connecting the network: " << mstWeight << "\n";

    // Generate the MST graph visualization
    string mstGraphFile = "mst_graph.dot";
    generateGraphvizFile(mstEdges, mstGraphFile, true);

    // Render the graphs as images using Graphviz
    cout << "Rendering the input graph...\n";
    system(("dot -Tpng " + inputGraphFile + " -o input_graph.png").c_str()); // Generate input graph image
    cout << "Rendering the MST graph...\n";
    system(("dot -Tpng " + mstGraphFile + " -o mst_graph.png").c_str());    // Generate MST graph image

    // Open the images (adjust for your OS)
    system("input_graph.png");
    system("mst_graph.png");

    return 0;
}