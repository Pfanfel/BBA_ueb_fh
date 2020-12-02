#pragma once

/**-------System Header-------*/
#include <vector>
#include <string>

namespace bba
{

    enum Action
    {
        ACTION_NONE,
        ACTION_INPUT,
        ACTION_OUTPUT,
        ACTION_SHOW,
        ACTION_FILTER_IDENTITY,
        ACTION_BRIGHTNESS,
        ACTION_FACTOR,
        ACTION_LINEAR,
        ACTION_CONTRAST,
        ACTION_NORMALIZE,
        ACTION_CLAMP,
        ACTION_GAMMA,
        ACTION_ADD,
        ACTION_SUB,
        ACTION_ADIFF,
        ACTION_HISTOGRAM,
        ACTION_SUM_HISTOGRAM,
        ACTION_HISTOGRAM_CHECK
    };

    struct Operation
    {
        Action action;
        std::string filename;
        std::vector<float> value;
    };

    typedef std::vector<Operation> Operations;

    Operations parse_arguments(int arc, char *argv[]);

    float convertStringToFloat(std::string str);

} // namespace bba
