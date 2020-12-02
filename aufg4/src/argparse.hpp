#pragma once

/**-------System Header-------*/
#include <vector>
#include <string>

namespace bba
{

    const int NUM_PARAMS_LINEAR = 2;
    const int NUM_PARAMS_ROTATE = 1;
    const int NUM_PARAMS_SCALE = 2;
    const int NUM_PARAMS_TRANSLATE = 2;
    const int NUM_PARAMS_SHEAR = 1;
    const int NUM_PARAMS_TRANSFORM = 4;
    const int NUM_PARAMS_UPRIGHT = 8;
    const int NUM_PARAMS_TRACK = 6;
    const int NUM_PARAMS_TRACK_GUI = 4;

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
        ACTION_HISTOGRAM_CHECK,
        ACTION_VALUES,
        ACTION_EDGE_NULL,
        ACTION_EDGE_VALUE,
        ACTION_EDGE_REPLICATE,
        ACTION_EDGE_REFLECT_11,
        ACTION_EDGE_REFLECT_101,
        ACTION_FOLD,
        ACTION_BOX,
        ACTION_GAUSSIAN,
        ACTION_SOBEL_X,
        ACTION_SOBEL_Y,
        ACTION_LAPLACE,
        ACTION_MEDIAN,
        ACTION_ERODE,
        ACTION_DILATE,
        ACTION_DISCRIMINATOR,
        ACTION_ROTATE,
        ACTION_SCALE,
        ACTION_SHEAR,
        ACTION_TRANSFORM,
        ACTION_TRANSLATE, //Extra
        ACTION_UPRIGHT,
        ACTION_NEAREST,
        ACTION_BILINEAR,
        ACTION_VIDEOIN,
        ACTION_VIDEOOUT,
        ACTION_TRACK,
        ACTION_TRACK_GUI
    };

    struct Operation
    {
        Action action;
        std::string filename;
        std::string trackText;
        std::vector<float> values;
        bool applyTransform;
    };

    typedef std::vector<Operation> Operations;

    Operations parse_arguments(int arc, char *argv[]);

    float convertStringToFloat(std::string str);

} // namespace bba
