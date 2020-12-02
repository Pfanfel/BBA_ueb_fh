/**-------Eigene Header-------*/
#include "argparse.hpp"

/**-------System Header-------*/
#include <stdexcept>
#include <iostream>

/**
 * Parst die Argumente als LL1 Parser
 * @param argc Anzahl der Argumente
 * @param argv Array von c Strings mit den Argumenten
 * @return 
 * */
bba::Operations bba::parse_arguments(int argc, char *argv[])
{

    Operations ops;

    //Transforms the c Strings to C++ Strings
    std::vector<std::string> arguments(argv + 1, argv + argc);

    unsigned int i = 0;
    /**
     * Handels the options demanding more than one argument
     * */
    auto consumeArgument = [&arguments, &i]() {
        if (arguments.size() > i + 1)
        {
            return arguments[++i];
        }
        else
        {
            throw std::runtime_error("Nicht genügend Argumente");
        }
    };

    for (; i < arguments.size(); i++)
    {
        std::string &action = arguments[i];
        if (action == "--input")
        {
            Operation op;
            op.action = bba::ACTION_INPUT;
            op.filename = consumeArgument();
            ops.push_back(op);
        }
        else if (action == "--output")
        {
            Operation op;
            op.action = bba::ACTION_OUTPUT;
            op.filename = consumeArgument();
            ops.push_back(op);
        }
        else if (action == "--show")
        {
            Operation op;
            op.action = bba::ACTION_SHOW;
            ops.push_back(op);
        }
        else if (action == "--identity")
        {
            Operation op;
            op.action = bba::ACTION_FILTER_IDENTITY;
            ops.push_back(op);
        }
        else
        {
            throw std::runtime_error("Unbekanntes Argument.");
        }
    }
    return ops;
}
