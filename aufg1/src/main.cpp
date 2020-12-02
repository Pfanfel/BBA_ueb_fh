/**-------Eigene Header-------*/
#include "argparse.hpp"
#include "ueb01.hpp"
#include "util.hpp"

/**-------System Header-------*/
#include <opencv2/opencv.hpp>
#include <stack>
#include <iostream>
#include <functional>

#define SCALE_TO_FLOAT 1.0 / 255.0
#define SCALE_TO_UCHAR 255

int main(int argc, char *argv[])
{
    bba::Operations ops = bba::parse_arguments(argc, argv); //Parameter in ops speichern
    std::stack<cv::Mat> stack;
    for (const bba::Operation &op : ops)
    {
        switch (op.action)
        {
        case bba::ACTION_INPUT:
        {
            cv::Mat img = cv::imread(op.filename, cv::IMREAD_ANYCOLOR); //Um Farb und Graubilder einzulesen
            img.convertTo(img, CV_32F, SCALE_TO_FLOAT);
            stack.push(img);
        }
        break;
        case bba::ACTION_OUTPUT:
        {
            cv::Mat img = stack.top();
            stack.pop();

            img.convertTo(img, CV_8U, SCALE_TO_UCHAR); //zurueck in 8 bit fuer pgm Format

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
        case bba::ACTION_BRIGHTNESS:
        {
            cv::Mat img = stack.top();
            stack.pop();
            bba::brightness(img, op.value[0]);
            stack.push(img);
        }
        break;
        case bba::ACTION_FACTOR:
        {
            cv::Mat img = stack.top();
            stack.pop();
            bba::factor(img, op.value[0]);
            stack.push(img);
        }
        break;
        case bba::ACTION_LINEAR:
        {
            cv::Mat img = stack.top();
            stack.pop();
            bba::linear(img, op.value[0], op.value[1]);
            stack.push(img);
        }
        break;
        case bba::ACTION_CONTRAST:
        {

            cv::Mat img = stack.top();
            stack.pop();
            bba::contrast(img, op.value[0]);
            stack.push(img);
        }
        break;
        case bba::ACTION_NORMALIZE:
        {
            cv::Mat img = stack.top();
            stack.pop();
            stack.push(bba::normalize(img));
        }
        break;
        case bba::ACTION_CLAMP:
        {
            cv::Mat img = stack.top();
            stack.pop();
            bba::clamp(img);
            stack.push(img);
        }
        break;
        case bba::ACTION_GAMMA:
        {
            cv::Mat img = stack.top();
            stack.pop();
            bba::gamma(img, op.value[0]);
            stack.push(img);
        }
        break;
        case bba::ACTION_ADD:
        {
            cv::Mat img2 = stack.top(); //Bild b
            stack.pop();
            cv::Mat img1 = stack.top(); //Bild a
            stack.pop();
            stack.push(bba::add(img1, img2));
        }
        break;
        case bba::ACTION_SUB:
        {
            cv::Mat img2 = stack.top(); //Vertauscht da Stack
            stack.pop();
            cv::Mat img1 = stack.top();
            stack.pop();
            stack.push(bba::sub(img1, img2));
        }
        break;
        case bba::ACTION_ADIFF:
        {
            cv::Mat img2 = stack.top(); //Bild b
            stack.pop();
            cv::Mat img1 = stack.top(); //Bild a
            stack.pop();
            stack.push(bba::adiff(img1, img2));
        }
        break;
        case bba::ACTION_HISTOGRAM:
        {
            cv::Mat img = stack.top();
            bba::histogramm(img, static_cast<int>(op.value[0]));
        }
        break;
        case bba::ACTION_SUM_HISTOGRAM:
        {
            cv::Mat img = stack.top();
            bba::sumHistogramm(img, static_cast<int>(op.value[0]));
        }
        break;
        case bba::ACTION_HISTOGRAM_CHECK:
        {
            cv::Mat img = stack.top();
            bba::histogrammCheck(img);
        }
        break;
        case bba::ACTION_NONE:
            break;
        }
    }

    return 0;
}