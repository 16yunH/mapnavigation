#ifndef PATHFINDER_H
#define PATHFINDER_H

#include <unordered_map>
#include <unordered_set>
#include <vector>
#include <queue>
#include <functional>
#include <cmath>
#include "OsmParser.h"
#define ll long long

using namespace std;

class PathFinder
{
public:
    PathFinder(const OsmParser &parser);
    vector<ll> findShortestPath(ll startId, ll goalId);

private:
    struct NodeInfo
    {
        ll nodeId;
        double gScore;
        double fScore;
        ll cameFrom;
        bool operator>(const NodeInfo &other) const
        {
            return fScore > other.fScore;
        }
    };

    double heuristic(const OsmNode &a, const OsmNode &b);
    void buildGraph();
    const OsmParser &parser_;
    unordered_map<ll, vector<pair<ll, double>>> adjacencyList_;
};

#endif // PATHFINDER_H
