/**-------Eigene Header-------*/
#include "argparse.hpp"

/**-------System Header-------*/
#include <stdexcept>
#include <iostream>

/**
 * Konvertiert String zu Float 
 * @param str String
 * @return den Float Wert 
 */
float bba::convertStringToFloat(std::string str)
{
    return std::atof(str.c_str());
}

/**
 * Parst die Argumente als LL1 Parser
 * @param argc Anzahl der Argumente
 * @param argv Array von c Strings mit den Argumenten
 * @return 
 * */
bba::Operations bba::parse_arguments(int argc, char *argv[])
{

    Operations ops;

    //Transformiert c strings in c++ strings
    std::vector<std::string> arguments(argv + 1, argv + argc);

    unsigned int i = 0;
    /**
     * Konsumiert das naechste Argument
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
        else if (action == "--brightness")
        {
            Operation op;
            op.action = bba::ACTION_BRIGHTNESS;
            op.value.push_back(convertStringToFloat(consumeArgument()));
            ops.push_back(op);
        }
        else if (action == "--factor")
        {
            Operation op;
            op.action = bba::ACTION_FACTOR;
            op.value.push_back(convertStringToFloat(consumeArgument()));
            ops.push_back(op);
        }
        else if (action == "--linear")
        {
            Operation op;
            op.action = bba::ACTION_LINEAR;
            op.value.push_back(convertStringToFloat(consumeArgument()));
            op.value.push_back(convertStringToFloat(consumeArgument()));
            ops.push_back(op);
        }
        else if (action == "--contrast")
        {
            Operation op;
            op.action = bba::ACTION_CONTRAST;
            op.value.push_back(convertStringToFloat(consumeArgument()));
            ops.push_back(op);
        }
        else if (action == "--normalize")
        {
            Operation op;
            op.action = bba::ACTION_NORMALIZE;
            ops.push_back(op);
        }
        else if (action == "--clamp")
        {
            Operation op;
            op.action = bba::ACTION_CLAMP;
            ops.push_back(op);
        }
        else if (action == "--gamma")
        {
            Operation op;
            op.action = bba::ACTION_GAMMA;
            op.value.push_back(convertStringToFloat(consumeArgument()));
            ops.push_back(op);
        }
        else if (action == "--add")
        {
            Operation op;
            op.action = bba::ACTION_ADD;
            ops.push_back(op);
        }
        else if (action == "--sub")
        {
            Operation op;
            op.action = bba::ACTION_SUB;
            ops.push_back(op);
        }
        else if (action == "--adiff")
        {
            Operation op;
            op.action = bba::ACTION_ADIFF;
            ops.push_back(op);
        }
        else if (action == "--histogram")
        {
            Operation op;
            op.action = bba::ACTION_HISTOGRAM;
            op.value.push_back(convertStringToFloat(consumeArgument()));
            ops.push_back(op);
        }
        else if (action == "--sum-histogram")
        {
            Operation op;
            op.action = bba::ACTION_SUM_HISTOGRAM;
            op.value.push_back(convertStringToFloat(consumeArgument()));
            ops.push_back(op);
        }
        else if (action == "--histogram-check")
        {
            Operation op;
            op.action = bba::ACTION_HISTOGRAM_CHECK;
            ops.push_back(op);
        }
        else if (action == "--normalize")
        {
            Operation op;
            op.action = bba::ACTION_NORMALIZE;
            ops.push_back(op);
        }
        else
        {
            throw std::runtime_error("Unbekanntes Argument.");
        }
    }
    return ops;
}
