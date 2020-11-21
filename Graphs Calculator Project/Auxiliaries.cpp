//
// Created by liord on 03/08/2020.
//

#include "Auxiliaries.h"
#include <string>
#include <vector>
#include "Exceptions.h"


using namespace std;

namespace final {
    const string SEMICOLON_STR = ";";
    const string SQUARE_OPEN_STR = "[";
    const string SQUARE_CLOSE_STR = "]";

    /**
     * removes empty vector cells
     */
    void removeEmpty(std::vector<std::string> &to_remove);


    /**
     * trims all vector cells
     * @param to_trim
     */
    void trimAll(std::vector<std::string> & to_trim);

    string trimString(const string & to_trim) {
        string new_trimmed;
        size_t first = to_trim.find_first_not_of(SPACE);
        size_t last = to_trim.find_last_not_of(SPACE);
        if((first == string::npos) || (last == string::npos)) {
            return "";
        }
        return to_trim.substr(first, last - first + 1);
    }

    void removeEmpty(vector<string> & to_remove) {
        for(unsigned int i = 0; i < to_remove.size() ; i++) {
            if(to_remove.at(i).empty()) {
                to_remove.erase(to_remove.begin() + i--);

            }
        }
    }

    bool checkIfAllowedSymbols(const string & letter) {
        return ((letter == SQUARE_OPEN_STR) || (letter == SQUARE_CLOSE_STR) || (letter == SEMICOLON_STR));
    }

    string addSpaces(const string & command_line) {
        string new_command;
        string trimmed = trimString(command_line);
        if(trimmed.empty()) {
            return trimmed;
        }
        unsigned int size = trimmed.size();
        bool is_save = false;
        if(saveCheck(trimmed)) {
             size = trimmed.find_last_of(COMMA) + 1;
             is_save = true;
        }
        for (unsigned int i = 0 ; i < size ; i++) {
            string letter(1, trimmed.at(i));
            if(ispunct(trimmed.at(i)) && (!checkIfAllowedSymbols(letter))) {
                string temp = SPACE + letter += SPACE;
                new_command.append(temp);
            } else {
                new_command.append(letter);
            }
        }
        if(is_save) {
            string leftovers = trimmed.substr(size , trimmed.length() - size - 1);
            new_command.append((" " + leftovers + " " + RIGHT_PARENTHESES));
        }
        return new_command;
    }

    void trimAll(vector<string> & to_trim) {
        for(string & str : to_trim) {
            str = trimString(str);
        }
    }

     vector<string> splitString(const string & command_line) {
        string trimmed = trimString(command_line);
        vector<string> split;
        bool is_save = saveCheck(command_line);
        unsigned int size = is_save ? command_line.find_last_of(COMMA) + 1 : command_line.size();
        unsigned int current, previous = 0;
        current = command_line.find(SPACE);
        while (current < size) {
            split.push_back(command_line.substr(previous, current - previous));
            previous = current + 1;
            current = command_line.find(SPACE, previous);
        }
        split.push_back(command_line.substr(previous, current - previous));
        if(is_save) {
            split.push_back(command_line.substr(size, command_line.length() - size - 1));
            split.push_back(RIGHT_PARENTHESES);
        }
        removeEmpty(split);
        trimAll(split);
        return split;
    }

    vector<string> subVector(const vector<string> & full_vector, unsigned int start, unsigned int end) {
        vector<string> sub_vector;
        for(unsigned int i = start ; i <= end ; i++) {
            sub_vector.push_back(full_vector.at(i));
        }
        return sub_vector;
    }


    void checkCommas(const string & name) {
        size_t place = name.find_first_of(COMMA);
        if(place != string::npos) {
            throw IllegalComma();
        }
    }

    void checkParenthesesExist(const string & name) {
        size_t place_left = name.find_first_of(LEFT_PARENTHESES);
        size_t place_right = name.find_first_of(RIGHT_PARENTHESES);
        if((place_left != string::npos) || (place_right != string::npos)) {
            throw IllegalFileParentheses();
        }
    }
}