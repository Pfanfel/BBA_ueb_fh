/**-------Eigene Header-------*/
#include "util.hpp"

/**-------System Header-------*/
#include <iostream>

void util::forEachPixel(cv::Mat &img, const std::function<void(cv::Mat &m, int y, int x)> &f)
{
    for (int y = 0; y < img.rows; y++)
    {
        for (int x = 0; x < img.cols; x++)
        {
            f(img, y, x);
        }
    }
}

void util::writeToCout(cv::Mat &img)
{
    std::vector<uchar> buff(img.rows * img.cols, 0); //Init die groesse des Buffers

    cv::imencode(".pgm", img, buff, std::vector<int>{cv::IMWRITE_PXM_BINARY, 0}); //Decodiert das Bild als .pgm und schreibt es in den buffer mit dem Flag auf 0

    for (std::vector<uchar>::iterator it = buff.begin(); it != buff.end(); ++it) //Iteriert ueber den gesamten vector mit dem iterator
    {
        std::cout << *it; //auf den Wert hinter dem Iterator zugreifen
    }
}

cv::Mat util::fillMat(cv::Mat &imgToFill, int offset, const std::vector<float> &values)
{
    auto filler = [&offset, &values](cv::Mat &m, int y, int x) {
        //Indexberechnung durch : max.Cols * row + col und Addition des offsets
        m.at<float>(y, x) = values[(m.cols * y + x) + offset];
    };

    util::forEachPixel(imgToFill, filler);
    return imgToFill;
}

std::vector<float> util::getSurroundingValues(cv::Mat &m, int y, int x)
{
    int imgHeight = m.cols;
    int imgWidth = m.rows;
    std::vector<float> res;
    for (int currentY = -1; currentY <= 1; currentY++)
    {
        for (int currentX = -1; currentX <= 1; currentX++)
        {
            if (y + currentY >= 0 && y + currentY < imgWidth && x + currentX >= 0 && x + currentX < imgHeight)
            {
                res.push_back(m.at<float>(y + currentY, x + currentX));
            }
        }
    }
    return res;
}

void util::printMatrix(cv::Mat &img, bool base255)
{
    int count = 0;
    auto out = [&count, &base255](cv::Mat &m, int y, int x) {
        float pix;
        pix = m.at<float>(y, x);
        pix = base255 ? (pix * 255) : pix;
        if ((count + 1) % m.cols == 0 && count != 0)
        {
            std::cout << pix << std::endl;
        }
        else
        {
            std::cout << pix << "  ";
        }
        count++;
    };
    util::forEachPixel(img, out);
    std::cout << std::endl;
}

void util::rotateMatrix180(cv::Mat &m)
{
    int matrixHeight = m.cols;
    int matrixWidth = m.rows;
    cv::Mat temp = cv::Mat(matrixWidth, matrixHeight, CV_32F);

    for (int y = matrixHeight - 1; y >= 0; y--)
    {
        for (int x = matrixWidth - 1; x >= 0; x--)
        {
            temp.at<float>(matrixHeight - 1 - y, matrixWidth - 1 - x) = m.at<float>(y, x);
        }
    }
    m = temp;
}

float util::degreeToRad(float degree)
{
    return (degree * M_PI) / 180.0;
}

float util::radToDegree(float rad)
{
    return (rad * 180) / M_PI;
}

cv::Mat util::convertMatx33fToMat(cv::Matx33f matx)
{
    cv::Mat res = cv::Mat(matx.cols, matx.rows, CV_32F);
    auto convert = [&matx](cv::Mat &m, int y, int x) {
        m.at<float>(y, x) = matx(y, x);
    };
    util::forEachPixel(res, convert);
    return res;
}
