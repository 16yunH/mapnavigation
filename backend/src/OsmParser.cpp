#include "OsmParser.h"
#include "D:/3.code/CLionProjects/mapnavigation/backend/lib/tinyxml2.h"
#include <iostream>

#define ll long long

using namespace tinyxml2;
using namespace std;

OsmParser::OsmParser() = default;

OsmParser::~OsmParser() = default;

bool OsmParser::parse(const string& filename) {
    XMLDocument doc;

    // Load the XML file
    if (doc.LoadFile(filename.c_str()) != XML_SUCCESS) {
        cerr << "Error loading OSM file: " << filename << endl;
        return false;
    }

    // Get the root element <osm>
    XMLElement* osmElement = doc.RootElement();
    if (osmElement == nullptr) {
        cerr << "Error: No root element <osm> found in the file." << endl;
        return false;
    }

    // Parse 
    for(XMLElement* element = osmElement->FirstChildElement(); element != nullptr; element = element->NextSiblingElement()) {
        string elementName = element->Name();
        if (elementName == "node") {
            parseNode(element);
        } else if (elementName == "way") {
            parseWay(element);
        } else if (elementName == "relation") {
            parseRelation(element);
        }
    }

    return true;
}

const unordered_map<ll, OsmNode>& OsmParser::getNodes() const {
    return nodes_;
}

const unordered_map<ll, OsmWay>& OsmParser::getWays() const {
    return ways_;
}

const unordered_map<ll, OsmRelation>& OsmParser::getRelations() const {
    return relations_;
}

void OsmParser::parseNode(XMLElement* nodeElement) {
    // Parse the attributes of the node
    ll id = nodeElement->Int64Attribute("id");
    double lat = nodeElement->DoubleAttribute("lat");
    double lon = nodeElement->DoubleAttribute("lon");

    // Create a new OsmNode and add it to the nodes map
    OsmNode node{id, lat, lon};
    praseTags(nodeElement, node.tags);
    nodes_[id] = node;
}

void OsmParser::parseWay(XMLElement* wayElement) {
    // Parse the attributes of the way
    ll id = wayElement->Int64Attribute("id");

    // Initialize an OsmWay object
    OsmWay way;
    way.id = id;

    // Parse the <nd> elements inside the <way> element (these represent the nodes in the way)
    for(XMLElement* ndElement = wayElement->FirstChildElement("nd"); ndElement != nullptr; ndElement = ndElement->NextSiblingElement("nd")) {
        ll nodeId = ndElement->Int64Attribute("ref");
        way.nodeIds.push_back(nodeId);
    }

    praseTags(wayElement, way.tags);
    ways_[id] = way;
}

void OsmParser::parseRelation(XMLElement* relationElement) {
    // Parse the attributes of the relation
    ll id = relationElement->Int64Attribute("id");

    // Initialize an OsmRelation object
    OsmRelation relation;
    relation.id = id;

    // Parse the <member> elements inside the <relation> element
    for(XMLElement* memberElement = relationElement->FirstChildElement("member"); memberElement != nullptr; memberElement = memberElement->NextSiblingElement("member")) {
        OsmRelation::Member member;

        const char* type = memberElement->Attribute("type");
        const char* role = memberElement->Attribute("role");
        member.ref = memberElement->Int64Attribute("ref");
        
        if (type) member.type = type;
        else member.type = "unknown";

        if (role) member.role = role;
        else member.role = "unknown";

        relation.members.push_back(member);
    }

    praseTags(relationElement, relation.tags);
    relations_[id] = relation;
}

void OsmParser::praseTags(XMLElement* element, unordered_map<string, string>& tags) {
    // Parse the <tag> elements inside the given element
    for(XMLElement* tagElement = element->FirstChildElement("tag"); tagElement != nullptr; tagElement = tagElement->NextSiblingElement("tag")) {
        const char* key = tagElement->Attribute("k");
        const char* value = tagElement->Attribute("v");

        if (key && value) {
            tags[key] = value;
        }
    }
}