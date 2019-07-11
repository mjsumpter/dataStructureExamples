/***********************************************************************************************
 * Graph.cpp (PP5)
 * Author: Matthew Sumpter
 * Description: Implementation file for Graph class that represents a weighted, undirected graph
 *              as an adjacency list, with string labels for the vertices.
 * 
 *              The Graph class provides the shortestPath() function, which uses Dijkstra's Algorithm
 *              to calculate the shortest path between any two vertices.
 * 
 *              See header file for class architecture
 * *********************************************************************************************/

#include "Graph.hpp"

#include <map>
#include <list>
#include <string>
#include <vector>
#include <exception>
#include <utility>
#include <limits>
#include <algorithm>


/***************************************************** Helper Functions *****************************************************/

// returns true if the vertex [label] exists, false otherwise
bool Graph::vertexExists(vertexName label)
{
    return adjList.find(label) != adjList.end();
}

// returns a pointer to adjList item of [label1] describing and edge to [label2]. Returns nullptr if not found
Graph::edge* Graph::findEdge(vertexName label1, vertexName label2)
{
    // if both vertices exist
    if(vertexExists(label1) && vertexExists(label2))
    {   // iterate through all adjacencies of [label1] vertex
        for (auto p = adjList[label1]->begin(); p != adjList[label1]->end(); ++p)
        {   // return pointer to edge if there is a match to [label2] vertex
            if (p->first == label2)
                return &(*p);
        }
    }
    return nullptr;
}

/***************************************************** Public Functions *****************************************************/

// destructor
Graph::~Graph()
{   // delete every list in adjList
    for (auto edges : adjList)
        delete edges.second;
}

// Creates and adds a vertex [label] to the graph. No two vertices can have the same label
void Graph::addVertex(std::string label)
{
    // if vertex already exists, throw error
    if ( vertexExists(label) )
        throw std::invalid_argument("Vertex already exists");

    // allocate a new list of edges
    edgeList* new_edges = new std::list<edge>;

    // assign the new edge list to the new vertex in [adjList] map
    adjList[label] = new_edges;

    ++numVertices;
}

// Removes the vertex with [label] from graph. Also removes all edges associated with that graph
void Graph::removeVertex(std::string label)
{
    // if vertex exists
    if( vertexExists(label) )
    {   // itereate over all vertices
        for (auto p = adjList.begin(); p != adjList.end(); ++p)
        {   // if edge exists with removed vertex, remove it from list
            auto edgeFound = findEdge(p->first, label);
            if (edgeFound != nullptr)
                p->second->remove(*edgeFound);
        }

        delete adjList[label]; // deallocate the vertex list associated with vertex
        adjList.erase(label);  // erase the vertex from the adjList
        --numVertices;
    }
    else
    {
        throw std::invalid_argument("Vertex doesn't exist");
    }
    
}

// Adds an edge of [weight] value to the graph between vertices [label1] and [label2].
// Conditions: Both vertices must exist, there cannot already be an edge between them, and a vertex can't have an edge to itself
void Graph::addEdge(std::string label1, std::string label2, unsigned long weight)
{
    bool vert1Exists = vertexExists(label1);
    bool vert2Exists = vertexExists(label2);
    bool selfEdge = label1 == label2;   
    bool existingEdge = findEdge(label1, label2) != nullptr;

    // if all conditions are satisfied
    if (vert1Exists && vert2Exists && !selfEdge && !existingEdge)
    {
        // add each label with the weight to the adjacency list for
        // each respective vertex
        adjList[label1]->push_back(std::make_pair(label2, weight));
        adjList[label2]->push_back(std::make_pair(label1, weight));
    }
    else
    {
        std::string error_msg = "An error occured";
        if (!vert1Exists)
            error_msg = label1 + " is not a valid vertex";
        else if (!vert2Exists)
            error_msg = label2 + " is not a valid vertex";
        else if (selfEdge)
            error_msg = "Vertex cannot have an edge to itself";
        else if (existingEdge)
            error_msg = "An edge already exists between " + label1 + " and " + label2;

        throw std::invalid_argument(error_msg);
    }
}

// Removes the edge from the graph between the vertex with [label1] and the vertex with [label2].
// Conditions: both vertices must exist, and there must be an edge between them
void Graph::removeEdge(std::string label1, std::string label2)
{
    bool vert1Exists = vertexExists(label1);
    bool vert2Exists = vertexExists(label2);
    bool edgeExists = findEdge(label1, label2) != nullptr;

    if (vert1Exists && vert2Exists && edgeExists)
    {
        auto vert1Tovert2 = findEdge(label1, label2);
        auto vert2Tovert1 = findEdge(label2, label1);

        adjList[label1]->remove(*vert1Tovert2);
        adjList[label2]->remove(*vert2Tovert1);
    }
    else
    {
        std::string error_msg = "An error occured";
        if (!vert1Exists)
            error_msg = label1 + " is not a valid vertex";
        else if (!vert2Exists)
            error_msg = label2 + " is not a valid vertex";
        else if (!edgeExists)
            error_msg = "An edge doesn't exist between " + label1 + " and " + label2;

        throw std::invalid_argument(error_msg);
    }
    
}

// Calculates the shortest path betwen the vertex [startLabel] and the vertex [endLabel] using Dijkstra's Algorithm.
// [path] stores the shortest path between the vertices
// the return value is the sum of the edges between the start and end vertices on the shortest path
unsigned long Graph::shortestPath(std::string startLabel, std::string endLabel, std::vector<std::string> &path)
{
    // INFINITY -> largest unsigned long value
    const unsigned long MAX_DIST = std::numeric_limits<unsigned long>::max();

    unsigned long min_path = MAX_DIST;     // Initialize min_path (return value) to MAX_DIST

    // initialize map of distanceValues. key->vertex label; value -> distance value
    // all vertices initialized to MAX_DIST besides [startLabel] - per Dijkstra's Algorithm
    std::map<vertexName, unsigned long> distanceValues;
    for (auto p = adjList.begin(); p != adjList.end(); ++p)
        distanceValues[p->first] = p->first == startLabel ? 0 : MAX_DIST;

    // initialize map of paths. key -> vertex label; value -> vector to hold the shortest path between the key vertex and [startLabel]
    std::map<vertexName, std::vector<std::string>> paths;
    for (auto p = adjList.begin(); p != adjList.end(); ++p)
    {
        std::string initialPath = p->first == startLabel ? startLabel : "";   // start label gets initialized to itself, as the shortest path to a vertex is the vertex itself
        paths[p->first].push_back(initialPath);
    }
   
   
    while (true)
    {
        // pair for tracking the current minimum distance vertex
        std::pair<vertexName, unsigned long> minimum = std::make_pair("null", MAX_DIST);

        // iterate over every non-visited vertex
        for(auto vertex : distanceValues)
        {   // find the vertex with the smallest distance value
            if (vertex.second < minimum.second)
                minimum = vertex;
        }

        path.push_back(minimum.first);  // minimum vertex distance will be visited next, push onto path vector (which is used to track visited vertices)

        // if the endLabel is the current minimum
        if (minimum.first == endLabel)
        {   // set the minimum path value to the current minimum, leave loop - endLabel vertex has been visited, completing the path 
            min_path = minimum.second;
            break;
        }

        
        distanceValues.erase(minimum.first); // remove visited nodes from [distanceValues]

        // for each vertex adjacent to the current minimum
        for ( auto edge : *adjList[minimum.first] )
        {
            // if current edge leads to already visited vertex, skip it
            if (std::find(path.begin(), path.end(), edge.first) != path.end())
                continue;

            // the distance to [edge] from [minimum] is (1) the distance to reach minimum (+) (2) the weight of the edge between them 
            unsigned long distance = edge.second + minimum.second;
            
            // if the distance from [minimum] is less than [edge]'s current distance:
            if (distance < distanceValues[edge.first])
            {  
                distanceValues[edge.first] = distance;           // update edge distance value to new minimum
                paths[edge.first] = paths[minimum.first];        // the new shortest path to this node is the path to [minimum]
                paths[edge.first].push_back(edge.first);         //         plus the vertex itself
            }
        }

    }

    path = paths[endLabel];  // set path to the vector that represents the shortest path to [endLabel]
    return min_path;         
}
