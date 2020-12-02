/**-------Eigene Header-------*/
#include "ueb00.hpp"
#include "util.hpp"

/**-------System Header-------*/
#include <iostream>

/**
 * Filter, der jeden Pixel im Bild einmal durchlaeuft und nichts damit macht
 * @param img Referenz auf eine Bildmatrix
 * */
bool bba::identity(cv::Mat &img)
{
    util::forEachPixel(img, [](cv::Mat &m, int y, int x) {
        m.at<unsigned char>(y, x) = m.at<unsigned char>(y, x); //assign value at y,x to y,x i.e. do nothing
    });
    return true;
}
