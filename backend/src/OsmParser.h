#ifndef OSM_PARSER_H
#define OSM_PARSER_H

#include <string>
#include <unordered_map>
#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>
#include "D:/3.code/CLionProjects/mapnavigation/backend/lib/tinyxml2.h"

#define ll long long

using namespace tinyxml2;
using namespace std;

// Define a structure to hold node information
struct OsmNode {
    ll id;      // Node ID
    double lat;        // Latitude
    double lon;        // Longitude
    unordered_map<string, string> tags; // Key-value pairs of tags associated with the node
};

// Define a structure to hold way information
struct OsmWay {
    ll id;                  // Way ID
    vector<long long> nodeIds; // References to nodes that make up the way
    unordered_map<string, string> tags; // Key-value pairs of tags associated with the way
};

// Define a structure to hold relation information
struct OsmRelation {
    ll id;                  // Relation ID

    // References to nodes, ways, and relations that make up the relation
    struct Member {
        string type; // "node", "way", or "relation"
        ll ref;     // ID of the node, way, or relation
        string role; // Role of the member in the relation
    };

    vector<Member> members; // List of members in the relation
    unordered_map<string, string> tags; // Key-value pairs of tags associated with the relation
};

// Class for parsing OSM XML data
class OsmParser {
public:
    OsmParser();
    ~OsmParser();

    // Method to parse an OSM XML file
    bool parse(const string& filename);

    // Method to get the nodes
    const unordered_map<ll, OsmNode>& getNodes() const;

    // Method to get the ways
    const unordered_map<ll, OsmWay>& getWays() const;

    // Method to get the relations
    const unordered_map<ll, OsmRelation>& getRelations() const;

private:
    // Helper methods for parsing nodes, ways, relations, and tags
    void parseNode(XMLElement* nodeElement);
    void parseWay(XMLElement* wayElement);
    void parseRelation(XMLElement* relationElement);
    void praseTags(XMLElement* element, unordered_map<string, string>& tags);

    // Data structures to store nodes, ways, and relations
    unordered_map<ll, OsmNode> nodes_;
    unordered_map<ll, OsmWay> ways_;
    unordered_map<ll, OsmRelation> relations_;
};

#endif // OSM_PARSER_H
