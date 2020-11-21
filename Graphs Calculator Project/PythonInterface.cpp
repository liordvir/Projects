//
// Created by liord on 08/08/2020.
//

#include "PythonInterface.h"
#include "Exceptions.h"
#include <iostream>

using namespace std;

bool checkForNull(Graph_ptr graph_1) {
    return graph_1 == nullptr;
}

bool checkForNull(Graph_ptr graph_1, Graph_ptr graph_2) {
    return (checkForNull(graph_1) || checkForNull(graph_2));
}

bool checkForNull(Graph_ptr graph_1, Graph_ptr graph_2, Graph_ptr graph_3) {
    return (checkForNull(graph_1, graph_2) || checkForNull(graph_3));
}

bool checkForNull(Graph_ptr graph, char * v1) {
    return (checkForNull(graph) || (v1 == nullptr));
}

bool checkForNull(Graph_ptr graph, char * v1, char * v2) {
    return (checkForNull(graph, v1) || (v2 == nullptr));
}

Graph_ptr create() {
    Graph_ptr  graph = nullptr;
    try{
        graph = new final::Graph();
    }
    catch (std::exception & e) {
        cout << "Error: Problem allocating memory" << endl;
    }
    return graph;
}

void destroy(Graph_ptr graph) {
    delete graph;
}

Graph_ptr addVertex(Graph_ptr graph, char * v) {
    Graph_ptr result = nullptr;
    if(graph == nullptr) {
        cout << "Error: Cannot add vertex (received null_ptr)" << endl;
        return result;
    }
    try{
        graph->addVertex(v);
        result = graph;
    }
    catch (std::exception & e) {
        cout << e.what() << endl;
    }
    return result;
}

Graph_ptr addEdge(Graph_ptr graph, char * v1, char * v2) {
    Graph_ptr result = nullptr;
    if(checkForNull(graph, v1, v2)) {
        cout << "Error: Cannot add edge (received null_ptr)" << endl;
        return result;
    }
    try{
        graph->addEdge(pair<string,string>(v1,v2));
        result = graph;
    }
    catch (std::exception & e) {
        cout << e.what() << endl;
    }
    return result;
}

void disp(Graph_ptr graph) {
    if(checkForNull(graph)) {
        cout << "Error: Cannot display Graph (received null_ptr)" << endl;
        return;
    }
    cout << *graph;
}

Graph_ptr graphUnion(Graph_ptr graph_in1, Graph_ptr graph_in2, Graph_ptr graph_out) {
    if(checkForNull(graph_in1, graph_in2, graph_out)) {
        cout << "Error: Cannot complete union (received null_ptr)" << endl;
        return nullptr;
    }
    try {
        *graph_out = *graph_in1 + *graph_in2;
    }
    catch (std::exception & e) {
        cout << e.what() << endl;
    }
    return graph_out;
}

Graph_ptr graphIntersection(Graph_ptr graph_in1, Graph_ptr graph_in2, Graph_ptr graph_out) {
    if(checkForNull(graph_in1, graph_in2, graph_out)) {
        cout << "Error: Cannot complete intersection (received null_ptr)" << endl;
        return nullptr;
    }
    try {
        *graph_out = *graph_in1 ^ *graph_in2;
    }
    catch (std::exception & e) {
        cout << e.what() << endl;
    }
    return graph_out;
}

Graph_ptr graphDifference(Graph_ptr graph_in1, Graph_ptr graph_in2, Graph_ptr graph_out) {
    if(checkForNull(graph_in1, graph_in2, graph_out)) {
        cout << "Error: Cannot complete difference (received null_ptr)" << endl;
        return nullptr;
    }
    try {
        *graph_out = *graph_in1 - *graph_in2;
    }
    catch (std::exception & e) {
        cout << e.what() << endl;
    }
    return graph_out;
}

Graph_ptr graphProduct(Graph_ptr graph_in1, Graph_ptr graph_in2, Graph_ptr graph_out) {
    if(checkForNull(graph_in1, graph_in2, graph_out)) {
        cout << "Error: Cannot complete product (received null_ptr)" << endl;
        return nullptr;
    }
    try {
        *graph_out = *graph_in1 * *graph_in2;
    }
    catch (std::exception & e) {
        cout << e.what() << endl;
    }
    return graph_out;
}

Graph_ptr graphComplement(Graph_ptr graph_in, Graph_ptr graph_out) {
    if(checkForNull(graph_in, graph_out)) {
        cout << "Error: Cannot complete intersection (received null_ptr)" << endl;
        return nullptr;
    }
    try {
        *graph_out = !(*graph_in);
    }
    catch (std::exception & e) {
        cout << e.what() << endl;
    }
    return graph_out;
}