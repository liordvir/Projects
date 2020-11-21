//
// Created by liord on 08/08/2020.
//

#ifndef MATAMFINAL_PYTHONINTERFACE_H
#define MATAMFINAL_PYTHONINTERFACE_H

#include "Graph.h"

typedef struct final::Graph* Graph_ptr;

Graph_ptr create();
void destroy(Graph_ptr graph);
Graph_ptr addVertex(Graph_ptr graph, char * v);
Graph_ptr addEdge(Graph_ptr graph, char * v1, char * v2);
void disp(Graph_ptr graph);
Graph_ptr graphUnion(Graph_ptr graph_in1, Graph_ptr graph_in2, Graph_ptr graph_out);
Graph_ptr graphIntersection(Graph_ptr graph_in1, Graph_ptr graph_in2, Graph_ptr graph_out);
Graph_ptr graphDifference(Graph_ptr graph_in1, Graph_ptr graph_in2, Graph_ptr graph_out);
Graph_ptr graphProduct(Graph_ptr graph_in1, Graph_ptr graph_in2, Graph_ptr graph_out);
Graph_ptr graphComplement(Graph_ptr graph_in, Graph_ptr graph_out);

#endif //MATAMFINAL_PYTHONINTERFACE_H
