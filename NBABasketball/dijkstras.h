#ifndef DIJKSTRAS_H
#define DIJKSTRAS_H

#include<bits/stdc++.h>
#include<vector>
#include <QString>
#include <QDebug>

using namespace std;

# define INF 0x3f3f3f3f

// iPairD ==> Integer Pair
typedef pair<int, double> iPairD;

// To add an edge
void addEdge(vector <pair<int, double> > adj[], int u,
                                     int v, double wt)
{
    adj[u].push_back(make_pair(v, wt));
}

// class to represent the team name of the shortest distance to the vertex
class Shortest
{
public:
    QString small;
};

// Prints shortest paths from src to all other vertices
double shortestPath(Shortest &smallest, vector<pair<int, double> > adj[], int V, int src, QStringList names, QStringList teams)
{
    // Create a priority queue to store vertices that
    // are being preprocessed. This is weird syntax in C++.
    priority_queue< iPairD, vector <iPairD> , greater<iPairD> > pq;

    // Create a vector for distances and initialize all
    // distances as infinite (INF)
    vector<double> dist(V, INF);

    // Insert source itself in priority queue and initialize
    // its distance as 0.
    pq.push(make_pair(0, src));
    dist[src] = 0;

//    vector< vector<int> > paths;
//    paths.resize(V);

    /* Looping till priority queue becomes empty (or all
    distances are not finalized) */
    while (!pq.empty())
    {
        // The first vertex in pair is the minimum distance
        // vertex, extract it from priority queue.
        // vertex label is stored in second of pair (it
        // has to be done this way to keep the vertices
        // sorted distance (distance must be first item
        // in pair)
        int u = pq.top().second;
        pq.pop();

        // Get all adjacent of u.
        for (auto x : adj[u])
        {
            // Get vertex label and weight of current adjacent
            // of u.
            int v = x.first;
            double weight = x.second;

            // If there is shorted path to v through u.
            if (dist[v] > dist[u] + weight)
            {
                // Updating distance of v
                dist[v] = dist[u] + weight;
                pq.push(make_pair(dist[v], v));
//                for (int i = 0; i < paths[u].size(); i++)
//                {
//                    //paths[v].clear();
//                    paths[v].push_back(paths[u][i]);
//                }
//                paths[v].push_back(v);
            }
        }
    }

    double final = 0.0;
    for (int i = 0; i < teams.size(); i++)
    {
        if (dist[names.indexOf(teams.at(i))] < final || final == 0.0)
        {
            final = dist[names.indexOf(teams.at(i))];
            smallest.small = teams.at(i);
        }
    }

    return final;
}

#endif // DIJKSTRAS_H
