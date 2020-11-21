//
// Created by liord on 06/08/2020.
//

#include "Arithmetic.h"
#include "Exceptions.h"
#include <assert.h>
#include <iostream>

using namespace std;

namespace final {

    const string SAVE_INDICATOR = "save";
    const char STRING_END = '\0';

    /**
     * deletes specific section in vector
     */
    void deleteCommandSection(vector<string> & command, unsigned int first_index, unsigned int last_index);

    /**
     * calculates all parentheses and replaces them with local variables
     */
    void extractParentheses(string_vector & command,  graph_map & calc_vars, int * local_counter);

    /**
     * checks if complements are coupled to valid variables
     */
    void checkComplements(const string_vector & command, const graph_map & calc_vars);

    /**
     * performs all complements on coupled variables
     */
    void executeAllComplements(string_vector & command,  graph_map & calc_vars, int * local_counter);

    /**
    * checks validity of vertices names
    */
    void checkVertices(const string_vector & string_to_check, unsigned int partition);

    /**
     * checks syntax for specific edge
     */
    void checkSpecificEdge(std::stack<std::string> & possible_edge);

    /**
     * check if edges have valid syntax
     */
    void checkEdges(const string_vector & string_to_check, unsigned int partition);

    /**
     * checks if assign command is valid
     */
    void checkCreateSyntax(const string_vector & string_to_check);

    /**
     * creates new Graph
     */
    Graph createOperator(const string_vector & string_to_graph);

    /**
     * converts string to arithmetic operator
     */
    arithmetic_operators stringToArithmetic(const std::string & command);

    /**
     * checks if command is only simple variables and operators (no complements)
     */
    void checkIfRightCommandValid(const string_vector & command,  const graph_map & calc_vars);

    /**
     * searches map for Graph by name
     */
    Graph getGraph(const graph_map & calc_vars, const std::string & name);

    /**
     * performs arithmetic operator between 2 graphs
     */
    Graph executeArithmetic(const Graph & first, const Graph & second, const std::string & arithmetic,
            const graph_map & calc_vars);

    /**
     * replaces representations og graphs with local variables
     */
    void insertGraph(string_vector & command,  graph_map & calc_vars, int * local_counter);

    /**
     * checks if parentheses follow known rules
     */
    void checkParentheses(const string_vector & command);

    /**
     * loads all graphs to local variables
     */
    void loadGraph(string_vector & command,  graph_map & calc_vars,
            int * local_counter, string_vector & load_names);

    /**
     * checks syntax in parentheses valid for SAVE operator
     */
    bool checkSaveParentheses(const string & inner_save);

    /**
     * gets single vertex name from binary file
     */
    string readVertex(ifstream & inFile, Graph & graph);

    /**
     * releases allocated char * and throws connot read exception
     */
    void deleteAndThrow(const char * first, const char * second = nullptr);

    /**
     * writes vertex information to graph
     */
    void writeVertex(const string & vertex, ofstream & outFile);

    /**
     * reads and adds a vertex to graph from binary file
     */
    void readAndAddEdge(ifstream & inFile, Graph & graph);


    void commandSimplifier(vector <string> &command, map <string, Graph> &calc_vars,
            int *local_counter_ptr, vector<string> & load_names) {
        loadGraph(command, calc_vars, local_counter_ptr, load_names);
        insertGraph(command, calc_vars, local_counter_ptr);
        checkParentheses(command);
        extractParentheses(command, calc_vars, local_counter_ptr);
        checkComplements(command, calc_vars);
        executeAllComplements(command, calc_vars, local_counter_ptr);
        checkIfRightCommandValid(command, calc_vars);
    }

    void deleteCommandSection(vector<string> & command, unsigned int first_index, unsigned int last_index) {
        vector<string>::iterator first = command.begin() + first_index;
        vector<string>::iterator last = command.begin() + last_index;
        command.erase(first, last);
    }

    void extractParentheses(vector<string> & command, map<string, Graph> & calc_vars, int * local_counter) {
        unsigned int open_index = 0;
        for(unsigned int i = 0; i < command.size() ; i++) {
            if(command.at(i) == LEFT_PARENTHESES) {
                open_index = i;
            }
            if(command.at(i) == RIGHT_PARENTHESES) {
                if(i == open_index + 1) {
                    throw IllegalParentheses();
                }
                vector<string> sub_command = subVector(command, open_index + 1, i - 1);
                checkComplements(sub_command, calc_vars);
                executeAllComplements(sub_command, calc_vars, local_counter);
                checkIfRightCommandValid(sub_command, calc_vars);
                Graph local_graph = executeRightCommand(sub_command, calc_vars);
                string local_name = to_string((*local_counter)++);
                calc_vars.insert(pair<string, Graph>(local_name, local_graph));
                deleteCommandSection(command, open_index + 1, i + 1);
                command.at(open_index) = local_name;
                open_index = 0;
                i = 0;
            }
        }
    }

    void checkComplements(const vector<string> & command, const map<string, Graph> & calc_vars) {
        if(stringToArithmetic(command.at(command.size() - 1)) == COMPLEMENT) {
            throw IllegalFormat();
        }
        for(unsigned int i = 0; i < command.size() - 1; i++) {
            if(stringToArithmetic(command.at(i)) == COMPLEMENT) {
                if(stringToArithmetic(command.at(i + 1)) == COMPLEMENT) {
                    continue;
                }
                if(stringToArithmetic(command.at(i + 1)) != OTHER) {
                    throw IllegalFormat();
                }
                map<string, Graph>::const_iterator iter = calc_vars.find(command.at(i + 1));
                if(iter == calc_vars.end()) {
                    variableNameCheck(command.at(i + 1));
                }
            }
        }
    }

    void executeAllComplements(vector<string> & command, map<string, Graph> & calc_vars, int * local_counter) {
        int counter = 0;
        for(unsigned int i = 0; i < command.size() ; i++) {
            if(stringToArithmetic(command.at(i)) == COMPLEMENT) {
                counter++;
            }
            else {
                if(!(counter % TWO)) {
                    deleteCommandSection(command, i - counter, i);
                    i -= counter;
                    counter = 0;
                    continue;
                }
                map<string, Graph>::iterator iter = calc_vars.find(command.at(i));
                if(iter == calc_vars.end()) {
                    throw NameNotInMemory(command.at(i));
                }
                string local_name = to_string((*local_counter)++);
                Graph local_graph = !getGraph(calc_vars, command.at(i));
                calc_vars.insert(pair<string, Graph>(local_name, local_graph));
                deleteCommandSection(command, i - counter + 1, i + 1);
                command.at(i - counter) = local_name;
                i -= counter;
                counter = 0;
            }
        }
    }

    void checkVertices(const vector<string> & string_to_check, unsigned int partition) {
        int comma_count = 1;
        if((partition > ZERO) && (string_to_check.at(partition - 1) == COMMA)) {
            throw IllegalGraphForm();
        }
        for(unsigned int i = 0; i < partition; i++) {
            if(string_to_check.at(i) == COMMA) {
                if(comma_count > 0) {
                    throw IllegalGraphForm();
                } else {
                    comma_count++;
                    continue;
                }
            }
            else {
                if(comma_count == 0) {
                    throw IllegalGraphForm();
                }
                Graph::testIfVertexNameValid(string_to_check.at(i));
                comma_count = 0;
            }
        }
    }

    void checkSpecificEdge(stack<string> & possible_edge) {
        for(int i = 0 ; i < FIVE ; i++) {
            if((i == ZERO) && (possible_edge.top() != RIGHT_EDGE)) {
                throw IllegalGraphForm();
            }
            if(i == ONE || i == THREE) {
                Graph::testIfVertexNameValid(possible_edge.top());
            }
            if((i == TWO) && (possible_edge.top() != COMMA)) {
                throw IllegalGraphForm();
            }
            if((i == FOUR) && (possible_edge.top() != LEFT_EDGE)) {
                throw IllegalGraphForm();
            }
            possible_edge.pop();
        }
    }

    void checkEdges(const vector<string> & string_to_check, unsigned int partition) {
        if(string_to_check.size() - 1 == partition) {
            return;
        }
        if(string_to_check.at(string_to_check.size() - 1) == COMMA) {
            throw IllegalGraphForm();
        }
        stack<string> possible_edge;
        int count = 0;
        for(unsigned int i = partition + 1 ; i < string_to_check.size() ; i++) {
            possible_edge.push(string_to_check.at(i));
            count++;
            if(!(count%FIVE))
            {
                if((i < string_to_check.size() - 1) && (string_to_check.at(i+1) != COMMA)) {
                    throw IllegalGraphForm();
                }
                checkSpecificEdge(possible_edge);
                i++;
            }
        }
        if(possible_edge.size() != ZERO) {
            throw IllegalGraphForm();
        }
    }

    void checkCreateSyntax(const vector<string> & string_to_check) {
        int separation_index = 0;
        int separation_count = 0;
        int count = 0;
        for(const string & word : string_to_check) {
            if(word == SEPARATION) {
                separation_index = count;
                separation_count++;
            }
            count++;
        }
        if(separation_count > 1) {
            throw IllegalGraphForm();
        }
        if(separation_count == 1) {
            checkVertices(string_to_check, separation_index);
            checkEdges(string_to_check, separation_index);
        }
        else {
            checkVertices(string_to_check, string_to_check.size());
        }
    }

    Graph createOperator(const vector<string> & string_to_graph) {
        checkCreateSyntax(string_to_graph);
        Graph new_graph;
        unsigned int iterator = ZERO;
        for(; ((iterator < string_to_graph.size()) && string_to_graph.at(iterator) != SEPARATION) ; iterator++) {
            if (string_to_graph.at(iterator) != COMMA) {
                new_graph.addVertex(string_to_graph.at(iterator));
            }
        }
        iterator++;
        for(; iterator < string_to_graph.size(); iterator += SIX) {
            graph_pair new_edge(string_to_graph.at(iterator + ONE), string_to_graph.at(iterator+THREE));
            new_graph.addEdge(new_edge);
        }
        return new_graph;
    }

    void variableNameCheck(const string & name) {
        if(!isalpha(name.at(0))) {
            throw IllegalGraphName(name);
        }
        for (char character : name) {
            if(!isalnum(character)) {
                throw IllegalGraphName(name);
            }
        }
    }

    arithmetic_operators stringToArithmetic(const string & command) {
        map<string, arithmetic_operators>::const_iterator iter = map_of_arithmetics.find(command);
        if(iter != map_of_arithmetics.end()) {
            return (*iter).second;
        }
        return OTHER;
    }

    void checkIfRightCommandValid(const vector<string> & command,  const map<string, Graph> & calc_vars) {
        if(!(command.size()%2)) {
            throw IllegalFormat();
        }
        for(unsigned int i = 0 ; i < command.size() ; i++) {
            if(!(i % 2)) {
                map<string, Graph>::const_iterator iter = calc_vars.find(command.at(i));
                if(iter == calc_vars.end()) {
                    throw NameNotInMemory(command.at(i));
                }
            }
            else {
                arithmetic_operators action = stringToArithmetic(command.at(i));
                if(action == OTHER) {
                    throw UnknownCommand();
                }
            }
        }
    }

    Graph getGraph(const map<string, Graph> & calc_vars, const string & name) {
        map<string, Graph>::const_iterator iter = calc_vars.find(name);
        assert(iter != calc_vars.end());
        return iter->second;
    }

    Graph executeArithmetic(const Graph & first, const Graph & second, const string & arithmetic,
            const map<string, Graph> & calc_vars) {
        arithmetic_operators action = stringToArithmetic(arithmetic);
        switch (action) {
            case UNION :
                return first + second;
            case INTERSECTION :
                return first ^ second;
            case DIFFERENCE :
                return first - second;
            case PRODUCT :
                return first * second;
            default:
                return first;
        }
        return first;
    }

    Graph executeRightCommand(const vector<string> & command, const map<string, Graph> & calc_vars) {
        Graph result = getGraph(calc_vars, command.at(0));
        for(unsigned int i = 1 ; i < command.size() ; i += TWO) {
            string arithmetic = command.at(i);
            Graph temp_graph = getGraph(calc_vars, command.at(i + 1));
            result = executeArithmetic(result, temp_graph, arithmetic, calc_vars);
        }
        return result;
    }

    void insertGraph(vector<string> & command, map<string, Graph> & calc_vars, int * local_counter) {
        for(unsigned int i = 0; i < command.size() ; i++) {
            if(command.at(i) == LEFT_GRAPH_OPEN) {
                for (unsigned int j = i + 1 ; j < command.size() ; j++) {
                    if(command.at(j) == RIGHT_GRAPH_OPEN) {
                        string local_name = to_string((*local_counter)++);
                        Graph local_graph;
                        if(j != i + 1) {
                            vector<string> sub_command = subVector(command, i + 1, j - 1);
                            local_graph = createOperator(sub_command);
                        }
                        calc_vars.insert(pair<string, Graph>(local_name, local_graph));
                        deleteCommandSection(command, i + 1, j + 1);
                        command.at(i) = local_name;
                        break;
                    }
                    else {
                        if(j == (command.size() - 1)) {
                            throw IllegalFormat();
                        }
                    }
                }
            }
        }
    }

    void checkParentheses(const vector<string> & command) {
        int counter = 0;
        for (const string & str : command) {
            if(str == LEFT_PARENTHESES) {
                counter++;
            }
            if(str == RIGHT_PARENTHESES) {
                if(counter <= 0) {
                    throw IllegalParentheses();
                }
                counter--;
            }
        }
        if(counter) {
            throw IllegalParentheses();
        }
    }

    void removeEnter(string & command) {
        if(command.empty()) {
            return;
        }
        if(command.at(command.size() - 1) == ENTER_KEY) {
            command.erase(command.size() - 1);
        }
    }

    void eraseLocalVariables(map<string, Graph> & calc_vars) {
        vector<string> variable_names;
        for(pair<string, Graph> var : calc_vars) {
            variable_names.push_back(var.first);
        }
        for(string name : variable_names) {
            if(isdigit(name.at(0))) {
                calc_vars.erase(calc_vars.find(name));
            }
        }
    }

    void checkLoadSyntax(const string & name) {
        checkCommas(name);
        checkParenthesesExist(name);
    }

    void loadGraph(vector<string> & command,  map<string, Graph> & calc_vars,
            int * local_counter, vector<string> & load_names) {
        for(unsigned int i = 0; i < command.size() ; i++) {
            if(command.at(i) == LOAD) {
                if((i + TWO >= command.size()) || (command.at(i + 1) != LEFT_PARENTHESES)) {
                    continue;
                }
                for(unsigned int j = i + TWO ; j < command.size() ; j++) {
                    if(command.at(j) == RIGHT_PARENTHESES) {
                        string name = load_names.front();
                        load_names.erase(load_names.begin());
                        checkLoadSyntax(name);
                        string local_name = to_string((*local_counter)++);
                        Graph local_graph;
                        ifstream from(name, ios_base::binary);
                        if(!from) {
                            throw CannotOpenFile(name);
                        }
                        local_graph = convertBinaryToGraph(from);
                        calc_vars.insert(pair<string, Graph>(local_name, local_graph));
                        deleteCommandSection(command, i + 1, j + 1);
                        command.at(i) = local_name;
                        break;
                    }
                    if(j == command.size() - 1) {
                        throw IllegalLoadSyntax();
                    }
                }
            }
        }
    }

    void loadNames(const string & command, vector<string> & load_names) {
        for (unsigned int i = 0; i < command.length(); i++) {
            size_t load_finder = command.find(LOAD, i);
            if(load_finder == string::npos) {
                return;
            }
            i = load_finder + FOUR;
            size_t left = command.find_first_not_of(SPACE, i);
            if((left == string::npos) || (command.at(left) != LEFT_PARENTHESES_CHAR)) {
                return;
            }
            size_t right = command.find_first_of(RIGHT_PARENTHESES_CHAR, left + 1);
            if((right == string::npos) || (right == left + 1)) {
                return;
            }
            string file = trimString(command.substr(left + 1, right - left - 1));
            load_names.push_back(file);
            i = right;
        }
    }

    bool checkSaveParentheses(const string & inner_save) {
        size_t last_comma = inner_save.find_last_of(COMMA);
        if(last_comma == string::npos) {
            return false;
        }
        if(last_comma == inner_save.length() - 1) {
            return false;
        }
        return inner_save.find_first_not_of(SPACE, last_comma) != string::npos;
    }

    bool saveCheck(string command) {
        command = trimString(command);
        if(command.length() < NINE) {
            return false;
        }
        string save = command.substr(ZERO, FOUR);
        if(save != SAVE_INDICATOR) {
            return false;
        }
        size_t start = command.find_first_not_of(SPACE, FOUR);
        if(start == string::npos) {
            return false;
        }
        if((command.at(start) != LEFT_PARENTHESES_CHAR) ||
           command.at(command.length() - 1) != RIGHT_PARENTHESES_CHAR) {
            return false;
        }
        return checkSaveParentheses(command.substr(start + 1, command.length() - start - TWO));
    }

    void writeVertex(const string & vertex, ofstream & outFile) {
        const char* vertex_converted = vertex.c_str();
        unsigned int len = vertex.length();
        if(!outFile.write((const char*)&len, sizeof(unsigned int))) {
            throw CannotWrite();
        }
        if(!outFile.write(vertex_converted, len)) {
            throw CannotWrite();
        }
    }

    void convertGraphToBinary(const Graph & graph, ofstream & outFile) {
        unsigned int num_vertices = graph.getVertices().size();
        unsigned int num_edges = graph.getEdges().size();
        if(!outFile.write((const char*)&num_vertices, sizeof(unsigned int))) {
            throw CannotWrite();
        }
        if(!outFile.write((const char*)&num_edges, sizeof(unsigned int))) {
            throw CannotWrite();
        }
        for(const string & vertex : graph.getVertices()) {
            writeVertex(vertex, outFile);
        }
        for(const graph_pair & edge : graph.getEdges()) {
            writeVertex(edge.first, outFile);
            writeVertex(edge.second, outFile);
        }
    }


    void deleteAndThrow(const char * first, const char * second) {
        delete[] first;
        delete[] second;
        throw CannotRead();
    }

    void readAndAddEdge(ifstream & inFile, Graph & graph) {
        string first = readVertex(inFile, graph);
        string second = readVertex(inFile, graph);
        graph.addEdge(graph_pair(string(first), string(second)));
    }

    void readAndAddVertex(ifstream & inFile, Graph & graph) {
        string name = readVertex(inFile, graph);
        graph.addVertex(name);
    }

    string readVertex(ifstream & inFile, Graph & graph) {
        unsigned int vertex_len;
        if(!inFile.read((char*)&vertex_len, sizeof(unsigned int))) {
            throw CannotRead();
        }
        char * vertex_name = new char[vertex_len + 1];
        if(!inFile.read(vertex_name, vertex_len)) {
            deleteAndThrow(vertex_name);
        }
        vertex_name[vertex_len] = STRING_END;
        string name_str = string(vertex_name);
        delete[] vertex_name;
        return name_str;
    }

    Graph convertBinaryToGraph(ifstream & inFile) {
        Graph graph;
        unsigned int num_vertices;
        unsigned int num_edges;
        if(!inFile.read((char*)&num_vertices, sizeof(unsigned int))) {
            throw CannotRead();
        }
        if(!inFile.read((char*)&num_edges, sizeof(unsigned int))) {
            throw CannotRead();
        }
        for(unsigned int i = 0; i < num_vertices ; i++) {
            readAndAddVertex(inFile, graph);
        }
        for(unsigned int i = 0; i < num_edges ; i++) {
            readAndAddEdge(inFile, graph);
        }
        return graph;
    }
}