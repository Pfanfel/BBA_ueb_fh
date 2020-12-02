/**-------Eigene Header-------*/
#include "argparse.hpp"
#include "ueb01.hpp"
#include "ueb02.hpp"
#include "ueb03.hpp"
#include "ueb04.hpp"
#include "util.hpp"

/**-------System Header-------*/
#include <opencv2/opencv.hpp>
#include <stack>
#include <iostream>
#include <functional>

#define SCALE_TO_FLOAT 1.0 / 255.0
#define SCALE_TO_UCHAR 255

std::vector<bba::stack_operation> bind_operations(const bba::Operations &ops)
{

    //Default Funktion falls keine --edge Option angegeben wurde
    //(Ohne parameterbezeichner damit kein unused Parameter warning entsteht)
    bba::EdgeFunc edgeTreatment = bba::getEdgeNullFunc();
    bba::Interpolation interpolation = bba::INTERPOLATE_NEAREST;
    bba::TargetImageData targetImageData = {true, 0, 0};

    //Initial die Einheitsmatrix
    cv::Matx33f transformationMatrix = {1, 0, 0,
                                        0, 1, 0,
                                        0, 0, 1};

    std::vector<bba::stack_operation> stack_operations;
    std::stack<cv::Mat> stack;

    for (const bba::Operation &op : ops)
    {
        switch (op.action)
        {
        case bba::ACTION_INPUT:
        {

            const std::string &filename = op.filename;
            cv::Mat img = cv::imread(filename, cv::IMREAD_COLOR); //Um Farb und Graubilder einzulesen
            img.convertTo(img, CV_32F, SCALE_TO_FLOAT);

            stack_operations.push_back(
                [img](bba::imagestack &stack) mutable {
                    stack.push_back(img);
                    return true;
                });
        }
        break;
        case bba::ACTION_VIDEOIN:
        {
            const std::string &filename = op.filename;
            cv::VideoCapture vc(filename);
            stack_operations.push_back(
                [vc](bba::imagestack &stack) mutable {
                    cv::Mat img;
                    vc >> img;
                    img.convertTo(img, CV_32F, SCALE_TO_FLOAT);
                    stack.push_back(img);
                    return !img.empty();
                });
        }
        break;
        case bba::ACTION_OUTPUT:
        {
            const std::string &filename = op.filename;
            stack_operations.push_back(
                [filename](bba::imagestack &stack) {
                    cv::Mat img = stack.back();
                    stack.pop_back();
                    img.convertTo(img, CV_8U, SCALE_TO_UCHAR); //zurueck in 8 bit Werte
                    if (filename == "-")
                    {
                        util::writeToCout(img);
                    }
                    else
                    {
                        cv::imwrite(filename, img, std::vector<int>{cv::IMWRITE_PXM_BINARY, 0});
                    }
                    return false;
                });
        }
        break;
        case bba::ACTION_VIDEOOUT:
        {
            const std::string &filename = op.filename;
            cv::VideoWriter vw;
            stack_operations.push_back(
                [vw, filename](bba::imagestack &stack) mutable {
                    cv::Mat img = stack.back();
                    stack.pop_back();
                    img.convertTo(img, CV_8U, SCALE_TO_UCHAR); //zurueck in 8 bit Werte
                    if (!vw.isOpened())
                    {
                        auto fourcc = cv::VideoWriter::fourcc('M', 'J', 'P', 'G');
                        vw.open(filename, fourcc, 25, img.size());
                    }
                    vw << img;
                    return true;
                });
        }
        break;
        case bba::ACTION_FILTER_IDENTITY:
        {
            stack_operations.push_back(
                [](bba::imagestack &stack) {
                    cv::Mat img = stack.back();
                    stack.pop_back();
                    bba::identity(img);
                    stack.push_back(img);
                    return true;
                });
        }
        break;
        case bba::ACTION_SHOW:
        {
            stack_operations.push_back(
                [](bba::imagestack &stack) {
                    cv::Mat img = stack.back();
                    cv::imshow("Testbild", img);
                    cv::waitKey(0);
                    return true;
                });
        }
        break;
        case bba::ACTION_BRIGHTNESS:
        {
            stack_operations.push_back(
                [op](bba::imagestack &stack) {
                    cv::Mat img = stack.back();
                    stack.pop_back();
                    cv::Mat merged;
                    std::vector<cv::Mat> channels;
                    cv::split(img, channels);

                    for (unsigned long int i = 0; i < channels.size(); i++)
                    {
                        bba::brightness(channels[i], op.values[0]);
                    }
                    cv::merge(channels, merged);
                    stack.push_back(merged);

                    return true;
                });
        }
        break;
        case bba::ACTION_FACTOR:
        {
            stack_operations.push_back(
                [op](bba::imagestack &stack) {
                    cv::Mat img = stack.back();
                    stack.pop_back();
                    cv::Mat merged;
                    std::vector<cv::Mat> channels;
                    cv::split(img, channels);

                    for (unsigned long int i = 0; i < channels.size(); i++)
                    {
                        bba::factor(channels[i], op.values[0]);
                    }
                    cv::merge(channels, merged);
                    stack.push_back(merged);

                    return true;
                });
        }

        break;
        case bba::ACTION_LINEAR:
        {

            stack_operations.push_back(
                [op](bba::imagestack &stack) {
                    cv::Mat img = stack.back();
                    stack.pop_back();
                    cv::Mat merged;
                    std::vector<cv::Mat> channels;
                    cv::split(img, channels);

                    for (unsigned long int i = 0; i < channels.size(); i++)
                    {
                        bba::linear(channels[i], op.values[0], op.values[1]);
                    }
                    cv::merge(channels, merged);
                    stack.push_back(merged);

                    return true;
                });
        }
        break;
        case bba::ACTION_CONTRAST:
        {
            cv::Mat img = stack.top();
            stack.pop();
            bba::contrast(img, op.values[0]);
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
            bba::gamma(img, op.values[0]);
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
            bba::histogramm(img, static_cast<int>(op.values[0]));
        }
        break;
        case bba::ACTION_SUM_HISTOGRAM:
        {
            cv::Mat img = stack.top();
            bba::sumHistogramm(img, static_cast<int>(op.values[0]));
        }
        break;
        case bba::ACTION_HISTOGRAM_CHECK:
        {
            cv::Mat img = stack.top();
            bba::histogrammCheck(img);
        }
        break;
        case bba::ACTION_VALUES:
        {
            stack_operations.push_back(
                [op](bba::imagestack &stack) {
                    stack.push_back(bba::values(op.values));
                    return true;
                });
        }
        break;
        case bba::ACTION_FOLD:
        {
            stack_operations.push_back(
                [edgeTreatment](bba::imagestack &stack) mutable {
                    cv::Mat kernel = stack.back(); //Filterkernel
                    stack.pop_back();
                    //damit nicht korreliert sodern gefaltet wird
                    util::rotateMatrix180(kernel);
                    cv::Mat img = stack.back();
                    stack.pop_back();

                    cv::Mat merged;
                    std::vector<cv::Mat> channels;
                    cv::split(img, channels);
                    for (unsigned long int i = 0; i < channels.size(); i++)
                    {
                        channels[i] = bba::fold(kernel, channels[i], edgeTreatment);
                    }
                    cv::merge(channels, merged);
                    stack.push_back(merged);

                    return true;
                });
        }
        break;
        case bba::ACTION_BOX:
        {
            stack.push(bba::box());
        }
        break;
        case bba::ACTION_EDGE_NULL:
        {
            edgeTreatment = bba::getEdgeNullFunc();
        }

        break;
        case bba::ACTION_EDGE_VALUE:
        {
            edgeTreatment = bba::getEdgeValueFunc(op.values[0]);
        }
        break;
        case bba::ACTION_EDGE_REPLICATE:
        {
            edgeTreatment = bba::getEdgeReplicateFunc();
        }
        break;
        case bba::ACTION_EDGE_REFLECT_11:
        {
            edgeTreatment = bba::getEdgeReflect11Func();
        }

        break;
        case bba::ACTION_EDGE_REFLECT_101:
        {
            edgeTreatment = bba::getEdgeReflect101Func();
        }
        break;
        case bba::ACTION_GAUSSIAN:
        {
            stack.push(bba::gaussian());
        }
        break;
        case bba::ACTION_SOBEL_X:
        {
            stack.push(bba::sobelX());
        }
        break;
        case bba::ACTION_SOBEL_Y:
        {
            stack.push(bba::sobelY());
        }
        break;
        case bba::ACTION_LAPLACE:
        {
            stack.push(bba::laplace());
        }
        break;
        case bba::ACTION_MEDIAN:
        {
            cv::Mat img = stack.top();
            stack.pop();
            stack.push(bba::median(img));
        }
        break;
        case bba::ACTION_ERODE:
        {
            cv::Mat img = stack.top();
            stack.pop();
            stack.push(bba::erode(img));
        }
        break;
        case bba::ACTION_DILATE:
        {
            cv::Mat img = stack.top();
            stack.pop();
            stack.push(bba::dilate(img));
        }
        break;
        case bba::ACTION_DISCRIMINATOR:
        {
            cv::Mat img = stack.top();
            bba::discriminator(img, edgeTreatment);
        }
        break;
        case bba::ACTION_ROTATE:
        {

            stack_operations.push_back(
                [transformationMatrix, op, edgeTreatment, interpolation, targetImageData](bba::imagestack &stack) mutable {
                    bba::makeAffineMatrixRotate(transformationMatrix, op.values);
                    if (op.applyTransform)
                    {
                        cv::Mat img = stack.back();
                        stack.pop_back();

                        cv::Mat merged;
                        std::vector<cv::Mat> channels;
                        cv::split(img, channels);
                        for (unsigned long int i = 0; i < channels.size(); i++)
                        {
                            channels[i] = bba::affineTransform(channels[i], transformationMatrix, interpolation, edgeTreatment, targetImageData);

                            //Rotation Matrix wird in affineTransform zurueckgestezt. Muss fuer weitere Channel wieder gesetzt werden, bis auf nach dem letzten
                            if (i != channels.size() - 1)
                            {
                                bba::makeAffineMatrixRotate(transformationMatrix, op.values);
                            }
                        }
                        cv::merge(channels, merged);
                        stack.push_back(merged);
                    }

                    return true;
                });
        }
        break;
        case bba::ACTION_SCALE:
        {
            bba::makeAffineMatrixScale(transformationMatrix, op.values);
            if (op.applyTransform)
            {
                cv::Mat img = stack.top();
                stack.pop();
                stack.push(bba::affineTransform(img, transformationMatrix, interpolation, edgeTreatment, targetImageData));
            }
        }
        break;
        case bba::ACTION_SHEAR:
        {
            bba::makeAffineMatrixShear(transformationMatrix, op.values);
            if (op.applyTransform)
            {
                cv::Mat img = stack.top();
                stack.pop();
                stack.push(bba::affineTransform(img, transformationMatrix, interpolation, edgeTreatment, targetImageData));
            }
        }
        break;
        case bba::ACTION_TRANSFORM:
        {
            bba::makeAffineMatrixTransform(transformationMatrix, op.values);
            if (op.applyTransform)
            {
                cv::Mat img = stack.top();
                stack.pop();
                stack.push(bba::affineTransform(img, transformationMatrix, interpolation, edgeTreatment, targetImageData));
            }
        }
        break;
        case bba::ACTION_TRANSLATE:
        {
            bba::makeAffineMatrixTranslate(transformationMatrix, op.values);
            if (op.applyTransform)
            {
                cv::Mat img = stack.top();
                stack.pop();
                stack.push(bba::affineTransform(img, transformationMatrix, interpolation, edgeTreatment, targetImageData));
            }
        }
        break;
        case bba::ACTION_UPRIGHT:
        {
            bba::makeAffineMatrixUpright(transformationMatrix, op.values, targetImageData);
            if (op.applyTransform)
            {
                cv::Mat img = stack.top();
                stack.pop();
                stack.push(bba::affineTransform(img, transformationMatrix, interpolation, edgeTreatment, targetImageData));
            }
        }
        break;

        case bba::ACTION_NEAREST:
        {
            interpolation = bba::INTERPOLATE_NEAREST;
        }
        break;

        case bba::ACTION_BILINEAR:
        {
            interpolation = bba::INTERPOLATE_BILINEAR;
        }
        break;
        case bba::ACTION_TRACK:
        {
            bool first = true;
            int videoImageCounter = 0;
            cv::Mat templ;
            std::deque<cv::Point> prevMatchLocations;
            bba::TrackParameters params = bba::createTrackParams(op);

            stack_operations.push_back(
                [op, first, templ, videoImageCounter, prevMatchLocations, params](bba::imagestack &stack) mutable {
                    cv::Mat img = stack.back();
                    stack.pop_back();
                    stack.push_back(bba::trackFeature(img, params, first, templ, videoImageCounter, prevMatchLocations));
                    return true;
                });
        }
        break;
        case bba::ACTION_TRACK_GUI:
        {
            bool first = true;
            int videoImageCounter = 0;
            cv::Mat templ;
            std::deque<cv::Point> prevMatchLocations;
            bba::TrackParameters params = bba::createTrackParams(op);

            stack_operations.push_back(
                [op, first, templ, videoImageCounter, prevMatchLocations, params](bba::imagestack &stack) mutable {
                    cv::Mat img = stack.back();
                    stack.pop_back();
                    if (first)
                    {
                        params.trackLoc = bba::trackFeatureGUI(img);
                    }
                    stack.push_back(bba::trackFeature(img, params, first, templ, videoImageCounter, prevMatchLocations));
                    return true;
                });
        }
        break;
        case bba::ACTION_NONE:
            break;
        }
    }

    return stack_operations;
}

int main(int argc, char *argv[])
{

    bba::Operations ops = bba::parse_arguments(argc, argv);                    //Parameter in ops speichern
    std::vector<bba::stack_operation> stack_operations = bind_operations(ops); //Operationen an Funcktionen binden
    for (bool ok = true; ok;)
    {
        bba::imagestack stack;
        for (const bba::stack_operation &so : stack_operations)
        {
            ok = so(stack);
            if (!ok)
            {
                break;
            }
        }
    }

    return 0;
}
