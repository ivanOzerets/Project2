#ifndef DFS_H
#define DFS_H

#include <iostream>
#include <vector>
#include <QString>
#include <QDebug>

using namespace std;

// data structure to store graph edges
struct DFSedge {
    int src, dest;
    double dist;
};

// class to represent a graph object
class Graph
{
public:
    // construct a vector of vectors to represent an adjacency list
    vector<vector<int>> adjList;
    double distList[97][97];
    double sum;
    QStringList DFS;

    // Graph Constructor
    Graph(vector<DFSedge> const &DFSedges, int N)
    {
        sum = 0;
        // resize the vector to N elements of type vector<int>
        adjList.resize(N);

        // add edges to the undirected graph
        for (auto &edge: DFSedges)
        {
            adjList[edge.src].push_back(edge.dest);
            distList[edge.src][edge.dest] = (edge.dist);
        }
    }
};

// Function to perform DFS Traversal
void DFS(Graph &graph, int v, vector<bool> &discovered, QStringList names)
{
    // mark current node as discovered
    discovered[v] = true;

    // print current node
    graph.DFS << names[v];

    // do for every edge (v -> u)
    for (int u : graph.adjList[v])
    {
        // u is not discovered
        if (!discovered[u])
        {
            graph.sum += graph.distList[v][u];

            DFS(graph, u, discovered, names);
        }
    }
}

#endif // DFS_H
