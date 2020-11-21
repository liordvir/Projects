//
// Created by liord on 04/08/2020.
//

#ifndef MATAMFINAL_EXCEPTIONS_H
#define MATAMFINAL_EXCEPTIONS_H

#include <string>
#include <exception>

namespace final {

    class Exceptions : public std::exception {
    protected:
        const std::string error_message;

    public:
        explicit Exceptions(std::string  error_message) : error_message(error_message) {};

        const char* what() const noexcept override {
            return error_message.c_str();
        }
    };

    /**
     * illegal Graph name
     */
    class IllegalGraphName : public Exceptions {
    public:
        explicit IllegalGraphName(const std::string & name) :
            Exceptions("Error: '" + name + "' is not a valid Graph name") {};
    };

    /**
     * illegal vertex name
     */
    class IllegalVertexName : public Exceptions {
    public:
        explicit IllegalVertexName(const std::string & name) :
                Exceptions("Error: '" + name + "' is not a valid vertex name") {};
    };

    /**
     * name not in memory
     */
    class NameNotInMemory : public Exceptions {
    public:
        explicit NameNotInMemory(const std::string & name) :
                Exceptions("Error: '" + name + "' is not in memory") {};
    };

    /**
     * wrong print syntax
     */
    class IllegalPrintSyntax : public Exceptions {
    public:
        IllegalPrintSyntax() :
                Exceptions("Error: Invalid print command") {};
    };

    /**
     * wrong delete syntax
     */
    class IllegalDeleteSyntax : public Exceptions {
    public:
        IllegalDeleteSyntax() :
                Exceptions("Error: Invalid delete command") {};
    };

    /**
     * wrong quit syntax
     */
    class IllegalQuitSyntax : public Exceptions {
    public:
        IllegalQuitSyntax() :
                Exceptions("Error: Invalid quit command") {};
    };

    /**
     * wrong who syntax
     */
    class IllegalWhoSyntax : public Exceptions {
    public:
        IllegalWhoSyntax() :
                Exceptions("Error: Invalid who command") {};
    };

    /**
     * wrong reset syntax
     */
    class IllegalResetSyntax : public Exceptions {
    public:
        IllegalResetSyntax() :
                Exceptions("Error: Invalid reset command") {};
    };

    /**
     * wrong save syntax
     */
    class IllegalSaveSyntax : public Exceptions {
    public:
        IllegalSaveSyntax() :
                Exceptions("Error: Invalid save command") {};
    };

    /**
     * unknown command
     */
    class UnknownCommand : public Exceptions {
    public:
        UnknownCommand() :
                Exceptions("Error: Unknown command") {};
    };

    /**
     * invalid Graph form
     */
    class IllegalGraphForm : public Exceptions {
    public:
        IllegalGraphForm() :
                Exceptions("Error: Invalid Graph form") {};
    };

    /**
     * edge of none existing vertex
     */
    class IllegalEdge : public Exceptions {
    public:
        explicit IllegalEdge(const std::string & vertex) :
                Exceptions("Error: '" + vertex + "' is not in Graph") {};
    };

    /**
     * illegal format
     */
    class IllegalFormat : public Exceptions {
    public:
        IllegalFormat() :
                Exceptions("Error: Format of command is illegal") {};
    };

    /**
     * illegal parentheses
     */
    class IllegalParentheses : public Exceptions {
    public:
        IllegalParentheses() :
                Exceptions("Error: Not adhering to parentheses rules") {};
    };

    /**
     * illegal file parentheses
     */
    class IllegalFileParentheses : public Exceptions {
    public:
        IllegalFileParentheses() :
                Exceptions("Error: File name cannot contain parentheses") {};
    };

    /**
     * file cannot be opened
     */
    class CannotOpenFile : public Exceptions {
    public:
        explicit CannotOpenFile(const std::string & name) :
                Exceptions("Error: Cannot open file '" + name + "'") {};
    };

    /**
     * vertex already exists
     */
    class DoubleVertex : public Exceptions {
    public:
        DoubleVertex() :
                Exceptions("Error: Graph cannot contain more than one of each vertex") {};
    };

    /**
     * edge already exists
     */
    class DoubleEdge : public Exceptions {
    public:
        DoubleEdge() :
                Exceptions("Error: Graph cannot contain more than one of each edge") {};
    };

    /**
     * self-circled edge
     */
    class SelfCircle : public Exceptions {
    public:
        SelfCircle() :
                Exceptions("Error: Graph cannot contain self-circled edges") {};
    };

    /**
     * comma in file name
     */
    class IllegalComma : public Exceptions {
    public:
        IllegalComma() :
                Exceptions("Error: File name cannot contain commas") {};
    };

    /**
     * wrong load syntax
     */
    class IllegalLoadSyntax : public Exceptions {
    public:
        IllegalLoadSyntax() :
                Exceptions("Error: Invalid load command") {};
    };

    /**
     * failed reading from file
     */
    class CannotRead : public Exceptions {
    public:
        CannotRead() :
                Exceptions("Error: File ended unexpectedly") {};
    };

    /**
     * failed writing to file
     */
    class CannotWrite : public Exceptions {
    public:
        CannotWrite() :
                Exceptions("Error: Failed writing information to file") {};
    };
}

#endif //MATAMFINAL_EXCEPTIONS_H
