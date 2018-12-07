#ifndef BFS_H
#define BFS_H

#include <iostream>
#include <vector>
#include <queue>
#include <QString>
#include <QDebug>

using namespace std;

// data structure to store graph edges
struct BFSedge {
    int src, dest;
    double dist;
};

// class to represent a graph object
class Temp
{
public:
    double sum;
    QStringList BFS;

    // Graph Constructor
    Temp()
    {
        sum = 0;
    }
};

// Adjacency Matrix
vector< vector<double> > FormAdjMatrix(vector<BFSedge> const &BFSedges, int names)
{
    // We could use an array for the adjMatrix if we knew the size, but it's safer to use a vector.
    vector< vector<double> > adjMatrix;

    // Initialize the adjMatrix so that all vertices can visit themselves.
    // (Basically, make an identity matrix.)
    const int n = names;

    for(int i = 0; i < n; i++)
    {
        // Initialize the row.
        vector<double> row;
        adjMatrix.push_back(row);

        // Set the row to have all 0's. (Except the vertex ing itself.)
        for(int j = 0; j < n; j++)
        {
            int value = 0;

            if(i == j)
            { value = 1; }

            adjMatrix[i].push_back(value);
        }
    }

    // Our matrix is set up, we just need to set up the edges (vertex connections).

    for (auto &edge: BFSedges)
    {
        adjMatrix[edge.src][edge.dest] = edge.dist;
    }

    // Our adjacency matrix is complete.
    return adjMatrix;
}

// Given an Adjacency Matrix, do a BFS on vertex "start"
void BFS(Temp &temp, vector< vector<double> > adjMatrix, int start, QStringList names)
{
    int n = names.size();
    // Create a "visited" array (true or false) to keep track of if we visited a vertex.
    bool visited[n];
    for (int i = 0; i < n; i++)
        visited[i] = false;

    // Create a queue for the nodes we visit.
    queue<int> q;

    // Add the starting vertex to the queue and mark it as visited.
    q.push(start);
    visited[start] = true;

    do
    {
        vector<int> store;
        vector<double> store2;
        // While the queue is not empty..
        while(q.empty() == false)
        {
            int vertex = q.front();
            q.pop();

            // Doing +1 in the cout because our graph is 1-based indexing, but our code is 0-based.
            temp.BFS << names[vertex];

            // Loop through all of it's friends.
            for(int i = 0; i < adjMatrix[vertex].size(); i++)
            {
                // The neighbor is the column number, and the edge is the value in the matrix.
                int neighbor = i;
                double edge = adjMatrix[vertex][i];

                // If the edge is "0" it means this guy isn't a neighbor. Move on.
                if(edge == 0.0) continue;

                // If the friend hasn't been visited yet, add it to the queue and mark it as visited
                if(visited[neighbor] == false)
                {
                    visited[neighbor] = true;
                    store.push_back(neighbor);
                    store2.push_back(edge);
                    temp.sum += edge;
                }
            }
        }
        // Sorting, then populating q
        int temp, vecsize = store.size();
        for (int j = 0; j < vecsize - 1; ++j) {
            double min = store2.at(j);
            temp = j;
            for (int i = j+1; i < vecsize; ++i) {
                if (min > store2.at(i)) {
                    min = store2.at(i);
                    temp = i;
                }
            }
            swap(store2.at(j), store2.at(temp));
            swap(store.at(j), store.at(temp));
        }

        for(vector<int>::iterator it = store.begin(); it != store.end(); ++it)
            q.push(*it);
    }
    while(q.empty() == false);
}

#endif // BFS_H
