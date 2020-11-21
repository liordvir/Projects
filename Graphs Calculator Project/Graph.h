//
// Created by liord on 03/08/2020.
//

#ifndef MATAMFINAL_GRAPH_H
#define MATAMFINAL_GRAPH_H

#include <set>
#include <string>
#include <vector>
#include <map>

namespace final{
    class Graph;
    typedef std::pair<std::string, std::string> graph_pair;
    typedef std::map<std::string, Graph> graph_map;
    const char OPEN_SQUARE = '[';
    const char CLOSE_SQUARE = ']';
    const char SEMICOLON = ';';
    const char DOLLAR = '$';

    class Graph {
        std::set<std::string> vertices;
        std::set<graph_pair> edges;
    public:
        Graph() = default;
        Graph(const std::set<std::string> & new_vertices, const std::set<graph_pair> & new_edges);
        Graph(const Graph & graph_to_copy);
        ~Graph() = default;
        Graph & operator=(const Graph & graph);
        Graph operator+(const Graph & graph) const;
        Graph operator^(const Graph & graph) const;
        Graph operator-(const Graph & graph) const;
        Graph operator*(const Graph & graph) const;
        Graph operator!() const;

        void addVertex(std::string name);
        void addEdge(graph_pair edge);

        bool removeVertex(const std::string & name);
        bool removeEdge(const graph_pair & edge);

        std::set<std::string> getVertices() const;
        std::set<graph_pair> getEdges() const;

        friend std::ostream& operator<<(std::ostream& os, const Graph& graph);


        /**
         * checks validity of vertex name:
         * characters must be alphanumerical OR  "[" , "]" , ";"
         * ";" must be between open "[" and closing "]"
         * "[", "]" must adhere to parentheses rules
         * @param name - name to check
         * @return true if valid, false otherwise
         */
        static void testIfVertexNameValid(const std::string & name);
    };
}


#endif //MATAMFINAL_GRAPH_H
