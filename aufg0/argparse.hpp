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
    ACTION_FILTER_IDENTITY
};

struct Operation
{
    Action action;
    std::string filename;
};

typedef std::vector<Operation> Operations;

Operations parse_arguments(int arc, char *argv[]);
} // namespace bba
