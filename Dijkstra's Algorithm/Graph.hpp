/***************************************************************************************************************************************
 * Graph.hpp (PP5)
 * Author: Matthew Sumpter
 * Description: Header file for Graph class. The Graph class provides the framework for
 *              storing a weighted, undirected graph of vertices labelled with a string.
 *              The structure of the graph is held within an adjacency list.
 * 
 *              The Graph class provides the shortestPath() function, which uses Dijkstra's Algorithm
 *              to calculate the shortest path between any two vertices.
 * 
 *              See implementation file for detailed function descriptions
 * *************************************************************************************************************************************/

#ifndef GRAPH_HPP
#define GRAPH_HPP

#include "GraphBase.hpp"

#include <list>
#include <string>
#include <map>

class Graph : public GraphBase
{
    private:
        typedef std::string vertexName;
        typedef std::pair<std::string, unsigned long> edge;   // for use in adjacency list, stores an endpoint and the associated weight
        typedef std::list<edge> edgeList;                     // a list of edges

        int numVertices;                                      // number of vertices in graph
        std::map<vertexName, edgeList*> adjList;              // the adjacency list. the keys are the vertex labels, and the list contains connected vertices and their weights

        // helper functions
        bool vertexExists(vertexName label);
        edge* findEdge(vertexName label1, vertexName label2);

    public:
        Graph(): numVertices(0) {};
        ~Graph();

        void addVertex(std::string label);
        void removeVertex(std::string label);
        void addEdge(std::string label1, std::string label2, unsigned long weight);
        void removeEdge(std::string label1, std::string label2);
        unsigned long shortestPath(std::string startLabel, std::string endLabel, std::vector<std::string> &path);
};

#endif // GRAPH_HPP