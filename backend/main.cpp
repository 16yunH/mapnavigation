#include "src/PathFinder.h"
#include "src/OsmParser.h"
#include <iostream>
#include <vector>

/*

using namespace tinyxml2;
using namespace std;


int main() {
    return 0;
}
*/


//example code for Praser
/*

int main() {
    OsmParser parser("D:/3.code/CLionProjects/MapNavigation/data/mapofgx.osm");

    if (parser.parse()) {
        const std::vector<Node>& nodes = parser.getNodes();
        const std::vector<Way>& ways = parser.getWays();

        std::cout << "Nodes: " << nodes.size() << std::endl;
        std::cout << "Ways: " << ways.size() << std::endl;

        // 输出部分解析的节点和路径信息
        for (const auto& node : nodes) {
            std::cout << "Node ID: " << node.id << ", Lat: " << node.lat << ", Lon: " << node.lon << std::endl;
        }

        for (const auto& way : ways) {
            std::cout << "Way ID: " << way.id << ", Nodes: " << way.nodeIds.size() << std::endl;
        }
    }

    return 0;
}
*/


/*
int main() {
    // Load the OSM data
    OsmParser osmParser;
    if (!osmParser.parse("D:/3.code/CLionProjects/mapnavigation/backend/data/mapofgx.osm")) {
        std::cerr << "Failed to parse OSM data!" << std::endl;
        return 1;
    }

    // Create the PathFinder and set the parser
    PathFinder pathFinder;
    pathFinder.setOsmParser(&osmParser);

    // Define the start and goal IDs (these should be valid node IDs from your OSM data)
    long long startId = 655345426;  // Example start node
    long long goalId = 655345472;   // Example goal node

    // Find the shortest path using Bidirectional A*
    std::vector<long long> path = pathFinder.findShortestPath(startId, goalId);

    // Print the resulting path
    if (!path.empty()) {
        std::cout << "Shortest path: ";
        for (long long id : path) {
            std::cout << id << " ";
        }
        std::cout << std::endl;
    } else {
        std::cout << "No path found." << std::endl;
    }

    return 0;
}

*/

int main() {
    OsmParser parser;
    if (!parser.parse("D:/3.code/CLionProjects/mapnavigation/backend/data/mapofgx.osm")) {
        std::cerr << "Failed to parse osm file." << std::endl;
        return -1;
    }

    PathFinder pathFinder(parser);

    // 设置起点和终点的节点 ID
    long long startNodeId = 573197126; // 请替换为实际的节点 ID
    long long goalNodeId = 573197160;  // 请替换为实际的节点 ID

    // 寻找最短路径
    std::vector<long long> shortestPath = pathFinder.findShortestPath(startNodeId, goalNodeId);

    if (shortestPath.empty()) {
        std::cout << "No path found." << std::endl;
    } else {
        std::cout << "The path:" << std::endl;
        for (long long nodeId : shortestPath) {
            std::cout << nodeId << std::endl;
        }
    }

    return 0;
}