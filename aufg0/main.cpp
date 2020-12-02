/**-------Eigene Header-------*/
#include "argparse.hpp"
#include "ueb00.hpp"
#include "util.hpp"

/**-------System Header-------*/
#include <opencv2/opencv.hpp>
#include <stack>
#include <iostream>

int main(int argc, char *argv[])
{
    bba::Operations ops = bba::parse_arguments(argc, argv); //Saving the params in var
    std::stack<cv::Mat> stack;
    for (const bba::Operation &op : ops)
    {
        switch (op.action)
        {
        case bba::ACTION_INPUT:
        {
            cv::Mat img = cv::imread(op.filename, cv::IMREAD_ANYCOLOR); //To read any image color/greayscale
            stack.push(img);
        }
        break;
        case bba::ACTION_OUTPUT:
        {
            cv::Mat img = stack.top();
            stack.pop();
            if (op.filename == "-")
            {
                util::writeToCout(img);
            }
            else
            {
                cv::imwrite(op.filename, img, std::vector<int>{cv::IMWRITE_PXM_BINARY, 0});
            }
        }
        break;
        case bba::ACTION_FILTER_IDENTITY:

        {
            cv::Mat img = stack.top();
            stack.pop();
            bba::identity(img);
            stack.push(img);
        }
        break;
        case bba::ACTION_SHOW:
        {
            cv::Mat img = stack.top();
            cv::imshow("Testbild", img);
            cv::waitKey(0);
        }
        break;
        case bba::ACTION_NONE:
            break;
        }
    }

    return 0;
}