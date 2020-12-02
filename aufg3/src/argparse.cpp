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
 * Prueft ob es sich bei den naechsten Argumenten auch um affine Operationen handelt, die ineiander Multipliziert wird
 * @param argument das naechste Argument auf der Kommandozeile
 * @rreturn Ob eine weitere affine Operationen gefordert wird oder nicht
 */
static bool checkAffineArgument(std::string argument)
{
    bool res = true;
    if (argument == "--rotate" || argument == "--scale" || argument == "--shear" || argument == "--transform" || argument == "--translate" || argument == "--upright")
    {
        res = false;
    }
    return res;
}

/**
 * Schaut sich das nächste Argument an und entscheidet ob danach die Transformation angewendet wird, oder noch weitere affine Operationen eigelesen werden sollen
 * @param op der Befehl als struct
 * @param arguments die Befehle von der Kommandozeile
 * @param i die Aktuell betrachtete Position der Befehle
 */
static void handleTransformParam(bba::Operation &op, std::vector<std::string> &arguments, unsigned int &i)
{
    //Noch weitere Argumente folgen
    if (arguments.size() > i + 1)
    {
        op.applyTransform = checkAffineArgument(arguments[i + 1]);
    }
    else //Aktuelles ist letztes Argument
    {
        op.applyTransform = true;
    }
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

    auto consumeNextNArguments = [&arguments, &i, &consumeArgument](int n) {
        std::vector<float> values;
        int count;
        for (count = 0; count < n; count++)
        {
            values.push_back(convertStringToFloat(consumeArgument()));
        }
        return values;
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
            op.values.push_back(convertStringToFloat(consumeArgument()));
            ops.push_back(op);
        }
        else if (action == "--factor")
        {
            Operation op;
            op.action = bba::ACTION_FACTOR;
            op.values.push_back(convertStringToFloat(consumeArgument()));
            ops.push_back(op);
        }
        else if (action == "--linear")
        {
            Operation op;
            op.action = bba::ACTION_LINEAR;
            op.values = consumeNextNArguments(bba::NUM_PARAMS_LINEAR);
            ops.push_back(op);
        }
        else if (action == "--contrast")
        {
            Operation op;
            op.action = bba::ACTION_CONTRAST;
            op.values.push_back(convertStringToFloat(consumeArgument()));
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
            op.values.push_back(convertStringToFloat(consumeArgument()));
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
            op.values.push_back(convertStringToFloat(consumeArgument()));
            ops.push_back(op);
        }
        else if (action == "--sum-histogram")
        {
            Operation op;
            op.action = bba::ACTION_SUM_HISTOGRAM;
            op.values.push_back(convertStringToFloat(consumeArgument()));
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
        else if (action == "--values")
        {
            Operation op;
            op.action = bba::ACTION_VALUES;
            int width = (std::stoi(consumeArgument())); //Breite
            op.values.push_back(width);
            int height = (std::stoi(consumeArgument())); //Hoehe
            op.values.push_back(height);
            for (int i = 0; i < width * height; i++)
            {
                op.values.push_back(convertStringToFloat(consumeArgument())); //Alle Folgenden Werte
            }
            ops.push_back(op);
        }
        else if (action == "--edge")
        {
            Operation op;
            std::string edge = consumeArgument();
            if (edge == "null")
            {
                op.action = bba::ACTION_EDGE_NULL;
                ops.push_back(op);
            }
            else if (edge == "replicate")
            {
                op.action = bba::ACTION_EDGE_REPLICATE;
                ops.push_back(op);
            }
            else if (edge == "reflect11")
            {
                op.action = bba::ACTION_EDGE_REFLECT_11;
                ops.push_back(op);
            }
            else if (edge == "reflect101")
            {
                op.action = bba::ACTION_EDGE_REFLECT_101;
                ops.push_back(op);
            }
            else
            {
                op.action = bba::ACTION_EDGE_VALUE;
                op.values.push_back(convertStringToFloat(edge));
                ops.push_back(op);
            }
        }
        else if (action == "--fold")
        {
            Operation op;
            op.action = bba::ACTION_FOLD;
            ops.push_back(op);
        }
        else if (action == "--box")
        {
            Operation op;
            op.action = bba::ACTION_BOX;
            ops.push_back(op);
        }
        else if (action == "--gaussian")
        {
            Operation op;
            op.action = bba::ACTION_GAUSSIAN;
            ops.push_back(op);
        }
        else if (action == "--sobel-x")
        {
            Operation op;
            op.action = bba::ACTION_SOBEL_X;
            ops.push_back(op);
        }
        else if (action == "--sobel-y")
        {
            Operation op;
            op.action = bba::ACTION_SOBEL_Y;
            ops.push_back(op);
        }
        else if (action == "--laplace")
        {
            Operation op;
            op.action = bba::ACTION_LAPLACE;
            ops.push_back(op);
        }
        else if (action == "--median")
        {
            Operation op;
            op.action = bba::ACTION_MEDIAN;
            ops.push_back(op);
        }
        else if (action == "--erode")
        {
            Operation op;
            op.action = bba::ACTION_ERODE;
            ops.push_back(op);
        }
        else if (action == "--dilate")
        {
            Operation op;
            op.action = bba::ACTION_DILATE;
            ops.push_back(op);
        }
        else if (action == "--discriminator")
        {
            Operation op;
            op.action = bba::ACTION_DISCRIMINATOR;
            ops.push_back(op);
        }
        else if (action == "--rotate")
        {
            Operation op;
            op.action = bba::ACTION_ROTATE;
            op.values = consumeNextNArguments(bba::NUM_PARAMS_ROTATE);
            handleTransformParam(op, arguments, i);
            ops.push_back(op);
        }
        else if (action == "--scale")
        {
            Operation op;
            op.action = bba::ACTION_SCALE;
            op.values = consumeNextNArguments(bba::NUM_PARAMS_SCALE);
            handleTransformParam(op, arguments, i);
            ops.push_back(op);
        }
        else if (action == "--shear")
        {
            Operation op;
            op.action = bba::ACTION_SHEAR;
            op.values = consumeNextNArguments(bba::NUM_PARAMS_SHEAR);
            handleTransformParam(op, arguments, i);
            ops.push_back(op);
        }
        else if (action == "--transform")
        {
            Operation op;
            op.action = bba::ACTION_TRANSFORM;
            op.values = consumeNextNArguments(bba::NUM_PARAMS_TRANSFORM);
            handleTransformParam(op, arguments, i);
            ops.push_back(op);
        }
        else if (action == "--translate")
        {
            Operation op;
            op.action = bba::ACTION_TRANSLATE;
            op.values = consumeNextNArguments(bba::NUM_PARAMS_TRANSLATE);
            handleTransformParam(op, arguments, i);
            ops.push_back(op);
        }
        else if (action == "--upright")
        {
            Operation op;
            op.action = bba::ACTION_UPRIGHT;
            op.values = consumeNextNArguments(bba::NUM_PARAMS_UPRIGHT);
            handleTransformParam(op, arguments, i);
            ops.push_back(op);
        }
        else if (action == "--interpolation")
        {
            Operation op;
            std::string interpolatiion = consumeArgument();
            if (interpolatiion == "nearest")
            {
                op.action = bba::ACTION_NEAREST;
                ops.push_back(op);
            }
            else if (interpolatiion == "bilinear")
            {
                op.action = bba::ACTION_BILINEAR;
                ops.push_back(op);
            }
            else
            {
                throw std::runtime_error("Unbekannte Interpolation.");
            }
        }
        else
        {
            throw std::runtime_error("Unbekanntes Argument.");
        }
    }
    return ops;
}
