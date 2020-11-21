//
// Created by liord on 06/08/2020.
//

#ifndef MATAMFINAL_ARITHMETIC_H
#define MATAMFINAL_ARITHMETIC_H

#include <map>
#include <string>
#include <vector>
#include "Graph.h"
#include <stack>
#include "Auxiliaries.h"

namespace final {


    /**
     * simplifies command to only variables and binary operators
     */
    void commandSimplifier(string_vector &command, std::map<std::string, Graph> &calc_vars,
            int *local_counter_ptr, string_vector & load_names);

    /**
     * checks if name is valid
     */
    void variableNameCheck(const std::string & name);

    /**
     * performs simple operator chain
     */
    Graph executeRightCommand(const string_vector & command, const graph_map & calc_vars);

    /**
     * erased all local variables from map memory
     */
    void eraseLocalVariables(graph_map & calc_vars);

    /**
     * removes windows enter key from command's end
     * @param command
     */
    void removeEnter(std::string & command);

    /**
     * load names to memory
     */
    void loadNames(const std::string & command, string_vector & load_names) ;

    /**
     * throws if file name contains apostrophes, commas or parentheses
     */
    void checkLoadSyntax(const std::string & name);

    /**
     * converts Graph to binary file
     */
    void convertGraphToBinary(const Graph & graph, std::ofstream & outFile);

    /**
     * converts binary file to Graph
     */
    Graph convertBinaryToGraph(std::ifstream & inFile);

}

#endif //MATAMFINAL_ARITHMETIC_H
