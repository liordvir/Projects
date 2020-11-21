//
// Created by liord on 03/08/2020.
//

#ifndef MATAMFINAL_AUXILIARIES_H
#define MATAMFINAL_AUXILIARIES_H

#include <string>
#include <map>
#include <vector>
#include <fstream>

namespace final {


    const std::string COMMA = ",";
    const std::string LEFT_PARENTHESES = "(";
    const std::string RIGHT_PARENTHESES = ")";
    const char LEFT_PARENTHESES_CHAR = '(';
    const char RIGHT_PARENTHESES_CHAR = ')';
    const std::string SPACE = " ";
    const std::string SEPARATION = "|";
    const std::string LEFT_EDGE = "<";
    const std::string RIGHT_EDGE = ">";
    const std::string LEFT_GRAPH_OPEN = "{";
    const std::string RIGHT_GRAPH_OPEN = "}";
    const std::string EQUAL_SIGN = "=";
    const char ENTER_KEY = '\r';
    const std::string LOAD = "load";

    typedef std::vector<std::string> string_vector;

    enum Numbers {ZERO = 0, ONE = 1, TWO = 2, THREE = 3, FOUR = 4, FIVE = 5, SIX = 6 , NINE = 9};
    enum arithmetic_operators {UNION, INTERSECTION, DIFFERENCE, PRODUCT, COMPLEMENT, OTHER};
    const std::map<std::string, arithmetic_operators> map_of_arithmetics =
            {{"+", UNION},
             {"^", INTERSECTION},
             {"-", DIFFERENCE},
             {"*", PRODUCT},
             {"!", COMPLEMENT} ,
             {"other", OTHER}};

    /**
     * erases spaces from sides
     * @param to_trim
     * @return
     */
    std::string trimString(const std::string & to_trim);

    /**
     * adds spaces between words and operators
     * @param command_line
     * @return
     */
    std::string addSpaces(const std::string & command_line);

    /**
    * gets a command line and splits it into vector word for word
    * @param command_line
    * @return
    */
    std::vector<std::string> splitString(const std::string & command_line);

    /**
     * returns sub vector from start to last
     * @param full_vector
     * @param start
     * @param end
     * @return
     */
    std::vector<std::string> subVector(const std::vector<std::string> & full_vector,
            unsigned int start, unsigned int end);

    /**
     * check is command is of type SAVE
     * @param command
     * @return
     */
    bool saveCheck(std::string command);

    /**
     * checks is commas exist. throws if so
     * @param name
     */
    void checkCommas(const std::string & name);

    /**
     * check if there are parentheses at all. throw if so
     * @param name
     */
    void checkParenthesesExist(const std::string & name);

}
#endif //MATAMFINAL_AUXILIARIES_H
