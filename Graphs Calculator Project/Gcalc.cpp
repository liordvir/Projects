//
// Created by liord on 03/08/2020.
//

#include <iostream>
#include <map>
#include "Graph.h"
#include <stack>
#include "Auxiliaries.h"
#include "Exceptions.h"
#include "Arithmetic.h"


using namespace std;


namespace final {

    const string start_message = "Gcalc> ";
    enum key_words {PRINT, DELETE, RESET, QUIT, WHO, NONE, BATCH, SAVE};


    const map<string, key_words> map_of_keys =
            {{"print", PRINT},
             {"delete", DELETE},
             {"reset", RESET},
             {"quit", QUIT},
             {"who", WHO},
             {"txt", BATCH},
             {"save", SAVE}};

    /**
     * translates command string to enum
     */
    key_words commandToNum(const string & command);

    /**
     * terminates shell
     */
    void quitOperator(const vector<string> & command, bool * run);

    /**
     * prints simple and complex graph arguments
     */
    void printOperator(const vector<string> & command, map<string, Graph> & calc_vars,
                       bool batch, ofstream & outFile, vector<string> & load_names);

    /**
     * erases all variables
     */
    void resetOperator(const vector<string> & command,  map<string, Graph> & calc_vars);

    /**
     * prints all variable names
     */
    void whoOperator(const vector<string> & command,  const map<string, Graph> & calc_vars,
                     bool batch, ofstream & outFile);

    /**
     * performs either assignment of defines new variable
     */
    void defineAndAssignOperator(vector<string> & command, map<string, Graph> & calc_vars, vector<string> & load_names);

    /**
     * saves graph argument to binary file
     */
    void saveOperator(vector<string> & command, map<string, Graph> & calc_vars, vector<string> & load_names);

    /**
     * decides which operator is performed
     */
    void allocateCommand(vector<string> & command, map<string, Graph> & calc_vars, bool * run,
                         bool batch, ofstream & outFile, vector<string> & load_names);


    key_words commandToNum(const string & command) {
        map<string, key_words>::const_iterator iter = map_of_keys.find(command);
        if(iter != map_of_keys.end()) {
            return (*iter).second;
        }
        return NONE;
    }

    void quitOperator(const vector<string> & command, bool * run) {
        if(command.size() != ONE) {
            throw IllegalQuitSyntax();
        }
        *run = false;
    }

    void printOperator(const vector<string> & command, map<string, Graph> & calc_vars,
            bool batch, ofstream & outFile, vector<string> & load_names) {
        if(command.size() < FOUR) {
            throw IllegalPrintSyntax();
        }
        if((command.at(ONE) != LEFT_PARENTHESES) || (command.at(command.size() - 1) != RIGHT_PARENTHESES)) {
            throw IllegalPrintSyntax();
        }

        int local_counter = 0;
        int * local_counter_ptr = &local_counter;
        vector<string> sub_command = subVector(command, TWO, command.size() - TWO);
        commandSimplifier(sub_command, calc_vars, local_counter_ptr, load_names);
        Graph result = executeRightCommand(sub_command, calc_vars);
        eraseLocalVariables(calc_vars);
        if(batch) {
            outFile << result;
        }
        else {
            cout << result;
        }
    }

    void resetOperator(const vector<string> & command,  map<string, Graph> & calc_vars) {
        if(command.size() != ONE) {
            throw IllegalResetSyntax();
        }
        calc_vars.clear();
    }

    void whoOperator(const vector<string> & command,  const map<string, Graph> & calc_vars,
            bool batch, ofstream & outFile) {
        if(command.size() != ONE) {
            throw IllegalWhoSyntax();
        }
        for (pair<string, Graph> var : calc_vars) {
            if(batch) {
                outFile << var.first << endl;
            }
            else {
                cout << var.first << endl;
            }
        }
    }

    void deleteOperator(const vector<string> & command,  map<string, Graph> & calc_vars) {
        if(command.size() != FOUR) {
            throw IllegalDeleteSyntax();
        }
        if((command.at(ONE) != LEFT_PARENTHESES) || (command.at(THREE) != RIGHT_PARENTHESES)) {
            throw IllegalDeleteSyntax();
        }
        map<string, Graph>::const_iterator iter = calc_vars.find(command.at(TWO));
        if(iter == calc_vars.end()) {
            throw NameNotInMemory(command.at(TWO));
        }
        calc_vars.erase(iter);
    }

    void defineAndAssignOperator(vector<string> & command, map<string, Graph> & calc_vars, vector<string> & load_names) {
        if(command.size() < THREE || command.at(ONE) != EQUAL_SIGN) {
            throw UnknownCommand();
        }
        int local_counter = 0;
        int * local_counter_ptr = &local_counter;
        vector<string> sub_command = subVector(command, TWO, command.size() - 1);
        commandSimplifier(sub_command, calc_vars, local_counter_ptr, load_names);
        Graph result = executeRightCommand(sub_command, calc_vars);
        map<string, Graph>::iterator iter = calc_vars.find(command.at(ZERO));
        if(iter != calc_vars.end()) {
            iter->second = result;
        }
        else {
            variableNameCheck(command.at(ZERO));
            calc_vars.insert(pair<string, Graph>(command.at(ZERO), result));
        }
        eraseLocalVariables(calc_vars);
    }

    void saveOperator(vector<string> & command, map<string, Graph> & calc_vars, vector<string> & load_names) {
        if(command.size() < SIX) {
            throw IllegalSaveSyntax();
        }
        if((command.at(ONE) != LEFT_PARENTHESES) || (command.at(command.size() - 1) != RIGHT_PARENTHESES)) {
            throw IllegalSaveSyntax();
        }
        if(command.at(command.size() - THREE) != COMMA) {
            throw IllegalSaveSyntax();
        }
        string file_name = command.at(command.size() - TWO);
        checkLoadSyntax(file_name);
        int local_counter = 0;
        int * local_counter_ptr = &local_counter;
        vector<string> sub_command = subVector(command, TWO, command.size() - FOUR);
        commandSimplifier(sub_command, calc_vars, local_counter_ptr, load_names);
        Graph result = executeRightCommand(sub_command, calc_vars);
        eraseLocalVariables(calc_vars);
        ofstream to(file_name, ios_base::binary);
        if(!to) {
            throw CannotOpenFile(file_name);
        }
        convertGraphToBinary(result, to);
    }

    void allocateCommand(vector<string> & command, map<string, Graph> & calc_vars, bool * run,
                         bool batch, ofstream & outFile, vector<string> & load_names) {
        if(command.empty()) {
            return;
        }
        key_words start = commandToNum(command.at(ZERO));
        switch(start) {
            case PRINT :
                printOperator(command, calc_vars, batch, outFile, load_names);
                break;
            case DELETE :
                deleteOperator(command, calc_vars);
                break;
            case RESET :
                resetOperator(command, calc_vars);
                break;
            case WHO :
                whoOperator(command, calc_vars, batch, outFile);
                break;
            case QUIT :
                quitOperator(command, run);
                break;
            case NONE :
                defineAndAssignOperator(command, calc_vars, load_names);
                break;
            case SAVE :
                saveOperator(command, calc_vars, load_names);
                break;
            default:
                break;
        }
    }

    void batch(char* input, char* output) {
        bool running = true;
        vector<string> load_names;
        bool * running_ptr = &running;
        ifstream from(input);
        ofstream to(output);
        map<string, Graph> calc_vars;
        try {
            if (!from) {
                cerr << "Error: Cannot open " + string(input) << endl;
            }
            if (!to) {
                cerr << "Error: Cannot open " + string(output) << endl;
            }
        }
        catch (std::exception &e) {
            cout << e.what() << endl;
        }
        string line;
        while (getline(from, line) && (running)) {
            removeEnter(line);
            load_names.clear();
            eraseLocalVariables(calc_vars);
            loadNames(line, load_names);
            string line_command = addSpaces(line);
            vector<string> vector_command = splitString(line_command);
            try {
                allocateCommand(vector_command, calc_vars, running_ptr, true, to, load_names);
            }
            catch (bad_alloc& e) {
                cerr << "Error: Out of memory (fatal)" << endl;
                throw e;
            }
            catch (exception &e) {
                to << e.what() << endl;
            }
        }
    }

    /**
     * main program (user)
     */
    void gcalc_user() {
        map<string, Graph> calc_vars;
        vector<string> load_names;
        bool running = true;
        bool * run = &running;
        ofstream outFile;
        vector<string> split_command;
        while (running) {
            load_names.clear();
            eraseLocalVariables(calc_vars);
            cout << start_message;
            string user_command;
            if (!getline(cin, user_command)) {
                break;
            }
            removeEnter(user_command);
            loadNames(user_command, load_names);
            string new_command = addSpaces(user_command);
            split_command = splitString(new_command);
            try {
                allocateCommand(split_command, calc_vars, run, false, outFile, load_names);
            }
            catch (bad_alloc& e) {
                cerr << "Error: Out of memory (fatal)" << endl;
                throw e;
            }
            catch (std::exception &e) {
                cout << e.what() << endl;
            }
        }
    }
}