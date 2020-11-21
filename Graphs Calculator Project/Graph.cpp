//
// Created by liord on 03/08/2020.
//

#include "Graph.h"
#include "Exceptions.h"
#include <cctype>
#include <algorithm>
#include <iostream>

namespace final {
    using std::string;
    using std::set;
    using std::vector;
    using std::cout;
    using std::endl;


    /**
     * checks if character is alphanumerical
     * @param character - character to check
     * @return true if alphanumerical. false otherwise
     */
    bool testIfAlphaNumerical(char character);

    /**
     * checks if character contains "[", "]" or ";"
     * @param character - character to check
     * @return true if contains special letters. false otherwise
     */
    bool testIfSpecial(char character);

    void Graph::testIfVertexNameValid(const string & name) {
        int bracket_count = 0;
        for (char character : name) {
            if (testIfAlphaNumerical(character)) {
                continue;
            }
            if (!testIfSpecial(character)) {
                throw IllegalVertexName(name);
            }
            switch (character) {
                case OPEN_SQUARE:
                    bracket_count++;
                    break;
                case CLOSE_SQUARE:
                    if (bracket_count > 0) {
                        bracket_count--;
                    } else {
                        throw IllegalVertexName(name);
                    }
                    break;
                case SEMICOLON:
                    if (bracket_count <= 0) {
                        throw IllegalVertexName(name);
                    }
                    break;
            }
        }
        if (bracket_count != 0) {
            throw IllegalVertexName(name);
        }
    }

    bool testIfAlphaNumerical(char character) {
        return std::isalnum(character);
    }

    bool testIfSpecial(char character) {
        vector<char> special_letters = {OPEN_SQUARE, CLOSE_SQUARE, SEMICOLON};
        for (char letter : special_letters) {
            if (character == letter) {
                return true;
            }
        }
        return false;
    }

    void Graph::addVertex(string name) {
        testIfVertexNameValid(name);
        if(this->vertices.count(name)) {
            throw DoubleVertex();
        }
        (*this).vertices.insert(name);
    }

    void Graph::addEdge(graph_pair edge) {
        if(!this->vertices.count(edge.first)) {
            throw IllegalEdge(edge.first);
        }
        if(!this->vertices.count(edge.second)) {
            throw IllegalEdge(edge.second);
        }
        if(edge.first == edge.second) {
            throw SelfCircle();
        }
        if(this->edges.count(edge)) {
            throw DoubleEdge();
        }
        this->edges.insert(edge);
    }

    Graph::Graph(const Graph & graph_to_copy) {
        this->vertices = graph_to_copy.getVertices();
        this->edges = graph_to_copy.getEdges();
    }

    Graph::Graph(const set<string> & new_vertices, const set<graph_pair> & new_edges) :
            vertices(new_vertices), edges(new_edges) {}


    Graph Graph::operator+(const Graph &graph) const {
        Graph union_graph;
        for(string vertex : (*this).vertices) {
            union_graph.vertices.insert(vertex);
        }
        for(string vertex : graph.vertices) {
            union_graph.vertices.insert(vertex);
        }
        for(graph_pair edge : (*this).edges) {
            union_graph.edges.insert(edge);
        }
        for(graph_pair edge : graph.edges) {
            union_graph.edges.insert(edge);
        }
        return union_graph;
    }

    std::set<std::string> Graph::getVertices() const {
        return this->vertices;
    }

    std::set<graph_pair> Graph::getEdges() const {
        return this->edges;
    }

    bool Graph::removeVertex(const std::string & name) {
        return this->vertices.erase(name) != 0;
    }

    bool Graph::removeEdge(const graph_pair & edge) {
        return this->edges.erase(edge) != 0;
    }

    Graph & Graph::operator=(const Graph & graph) {
        this->vertices = graph.getVertices();
        this->edges = graph.getEdges();
        return *this;
    }

    Graph Graph::operator-(const Graph & graph) const {
        Graph diff_graph = *this;
        for(string vertex : graph.vertices) {
            diff_graph.removeVertex(vertex);
        }
        const set<string> intersect_vertices = diff_graph.getVertices();
        const set<graph_pair> edge_iterator = diff_graph.getEdges();
        for(graph_pair edge : edge_iterator) {
            if((intersect_vertices.find(edge.first) == intersect_vertices.end()) ||
                    (intersect_vertices.find(edge.second) == intersect_vertices.end()) ) {
                diff_graph.removeEdge(edge);
            }
        }
        return diff_graph;
    }

    Graph Graph::operator^(const Graph & graph) const {
        Graph intersected_graph;
        for (string vertex_one : this->getVertices()) {
            for (string vertex_two : graph.getVertices()) {
                if(!vertex_one.compare(vertex_two)) {
                    intersected_graph.addVertex(vertex_one);
                }
            }
        }
        for (graph_pair edge_one : this->getEdges()) {
            for (graph_pair edge_two : graph.getEdges()) {
                if(edge_one == edge_two) {
                    intersected_graph.addEdge(edge_one);
                }
            }
        }
        return intersected_graph;
    }

    Graph Graph::operator!() const {
        Graph complement_graph;
        for (string vertex : this->getVertices()) {
            complement_graph.addVertex(vertex);
        }
        for (string vertex_prime : complement_graph.getVertices()) {
            for (string vertex_aux : complement_graph.getVertices()) {
                if(vertex_prime == vertex_aux) {
                    continue;
                }
                graph_pair new_edge(vertex_prime, vertex_aux);
                complement_graph.addEdge(new_edge);
            }
        }
        for (graph_pair edge : this ->getEdges()) {
            complement_graph.removeEdge(edge);
        }
        return complement_graph;
    }

    string vertexUnite(const string & vertex_prime, const string & vertex_aux) {
        return OPEN_SQUARE + vertex_prime + SEMICOLON + vertex_aux + CLOSE_SQUARE;
    }

    graph_pair edgeUnite(const graph_pair & edge_prime, const graph_pair & edge_aux) {
        string first = vertexUnite(edge_prime.first, edge_aux.first);
        string second = vertexUnite(edge_prime.second, edge_aux.second);
        return graph_pair(first, second);
    }

    Graph Graph::operator*(const Graph & graph) const {
        Graph product_graph;
        for(string vertex_prime : this->getVertices()) {
            for(string vertex_aux : graph.getVertices()) {
                string new_vertex = vertexUnite(vertex_prime, vertex_aux);
                product_graph.addVertex(new_vertex);
            }
        }
        for(graph_pair edge_prime : this->getEdges()) {
            for(graph_pair edge_aux : graph.getEdges()) {
                graph_pair new_edge = edgeUnite(edge_prime, edge_aux);
                product_graph.addEdge(new_edge);
            }
        }
        return product_graph;
    }

    std::ostream &operator<<(std::ostream &os, const Graph& graph) {
        for(string vertex : graph.getVertices()) {
            os << vertex << endl;
        }
        os << DOLLAR << endl;
        for(graph_pair edge : graph.getEdges()) {
            os << edge.first + " " + edge.second << endl;
        }
        return os;
    }
}
