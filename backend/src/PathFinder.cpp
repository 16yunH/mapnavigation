#include "PathFinder.h"
#include <unordered_map>
#include <unordered_set>
#include <queue>
#include <cmath>
#include <algorithm>
#include <limits>
#define ll long long

using namespace std;

PathFinder::PathFinder(const OsmParser &parser) : parser_(parser)
{
    buildGraph();
}

void PathFinder::buildGraph()
{
    const auto &ways = parser_.getWays();
    const auto &nodes = parser_.getNodes();

    for (const auto &wayPair : ways)
    {
        const OsmWay &way = wayPair.second;
        const auto &nodeIds = way.nodeIds;

        for (size_t i = 0; i + 1 < nodeIds.size(); ++i)
        {
            ll fromId = nodeIds[i];
            ll toId = nodeIds[i + 1];

            if (nodes.count(fromId) && nodes.count(toId))
            {
                const OsmNode &fromNode = nodes.at(fromId);
                const OsmNode &toNode = nodes.at(toId);
                double distance = heuristic(fromNode, toNode);
                adjacencyList_[fromId].emplace_back(toId, distance);
                adjacencyList_[toId].emplace_back(fromId, distance);
            }
        }
    }
}

double PathFinder::heuristic(const OsmNode &a, const OsmNode &b)
{
    const double R = 6371000; // Earth radius in meters
    double lat1 = a.lat * M_PI / 180.0;
    double lon1 = a.lon * M_PI / 180.0;
    double lat2 = b.lat * M_PI / 180.0;
    double lon2 = b.lon * M_PI / 180.0;

    double dlat = lat2 - lat1;
    double dlon = lon2 - lon1;

    double h = sin(dlat / 2) * sin(dlat / 2) + cos(lat1) * cos(lat2) * sin(dlon / 2) * sin(dlon / 2);
    double distance = 2 * R * asin(sqrt(h));

    return distance;
}

vector<ll> PathFinder::findShortestPath(ll startId, ll goalId)
{
    const auto &nodes = parser_.getNodes();

    if (!nodes.count(startId) || !nodes.count(goalId))
    {
        return {};
    }

    unordered_map<ll, double> gScoreForward, gScoreBackward;
    unordered_map<ll, ll> cameFromForward, cameFromBackward;
    unordered_set<ll> closedSetForward, closedSetBackward;

    auto cmp = [](const NodeInfo &a, const NodeInfo &b)
    { return a.fScore > b.fScore; };

    priority_queue<NodeInfo, vector<NodeInfo>, decltype(cmp)> openSetForward(cmp);
    priority_queue<NodeInfo, vector<NodeInfo>, decltype(cmp)> openSetBackward(cmp);

    gScoreForward[startId] = 0.0;
    gScoreBackward[goalId] = 0.0;

    openSetForward.push({startId, 0.0, heuristic(nodes.at(startId), nodes.at(goalId)), -1});
    openSetBackward.push({goalId, 0.0, heuristic(nodes.at(goalId), nodes.at(startId)), -1});

    ll meetingNode = -1;

    while (!openSetForward.empty() && !openSetBackward.empty())
    {
        if (!openSetForward.empty())
        {
            NodeInfo current = openSetForward.top();
            openSetForward.pop();

            if (closedSetBackward.count(current.nodeId))
            {
                meetingNode = current.nodeId;
                break;
            }

            closedSetForward.insert(current.nodeId);

            for (const auto &neighbor : adjacencyList_[current.nodeId])
            {
                ll neighborId = neighbor.first;
                double edgeWeight = neighbor.second;

                if (closedSetForward.count(neighborId))
                {
                    continue;
                }

                double tentative_gScore = gScoreForward[current.nodeId] + edgeWeight;

                if (!gScoreForward.count(neighborId) || tentative_gScore < gScoreForward[neighborId])
                {
                    cameFromForward[neighborId] = current.nodeId;
                    gScoreForward[neighborId] = tentative_gScore;
                    double fScore = tentative_gScore + heuristic(nodes.at(neighborId), nodes.at(goalId));
                    openSetForward.push({neighborId, tentative_gScore, fScore, current.nodeId});
                }
            }
        }

        if (!openSetBackward.empty())
        {
            NodeInfo current = openSetBackward.top();
            openSetBackward.pop();

            if (closedSetForward.count(current.nodeId))
            {
                meetingNode = current.nodeId;
                break;
            }

            closedSetBackward.insert(current.nodeId);

            for (const auto &neighbor : adjacencyList_[current.nodeId])
            {
                ll neighborId = neighbor.first;
                double edgeWeight = neighbor.second;

                if (closedSetBackward.count(neighborId))
                {
                    continue;
                }

                double tentative_gScore = gScoreBackward[current.nodeId] + edgeWeight;

                if (!gScoreBackward.count(neighborId) || tentative_gScore < gScoreBackward[neighborId])
                {
                    cameFromBackward[neighborId] = current.nodeId;
                    gScoreBackward[neighborId] = tentative_gScore;
                    double fScore = tentative_gScore + heuristic(nodes.at(neighborId), nodes.at(startId));
                    openSetBackward.push({neighborId, tentative_gScore, fScore, current.nodeId});
                }
            }
        }
    }

    if (meetingNode == -1)
    {
        // No path found
        return {};
    }

    vector<ll> pathForward, pathBackward;
    ll current = meetingNode;
    while (current != -1)
    {
        pathForward.push_back(current);
        if (cameFromForward.find(current) != cameFromForward.end())
        {
            current = cameFromForward[current];
        }
        else
        {
            break;
        }
    }
    reverse(pathForward.begin(), pathForward.end());
    current = meetingNode;
    while (current != -1)
    {
        pathBackward.push_back(current);
        if (cameFromBackward.find(current) != cameFromBackward.end())
        {
            current = cameFromBackward[current];
            pathBackward.push_back(current);
        }
        else
        {
            break;
        }
    }

    vector<ll> fullPath = pathForward;
    fullPath.insert(fullPath.end(), pathBackward.begin(), pathBackward.end());
    return fullPath;
}