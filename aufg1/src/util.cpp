/**-------Eigene Header-------*/
#include "util.hpp"

/**-------System Header-------*/
#include <iostream>

/**
 * Hilfsfunkiton, die jeden Pixel im uebergebenen Bild einmal durchlaeuft
 * und eine Funtkion darauf anwendet.
 * 
 * @param img Referenz auf eine Bildmatrix
 * @param f Funktion, die auf jeden Pixel angewendet wird
 * */
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

/**
 * Funktion zum ausgeben einer pgm auf der Standartausgabe.
 * Annahme: Es werden nur 8 Bit Portable Graymaps genutzt
 * @param img Referenz auf eine Bildmatrix 
 * */
void util::writeToCout(cv::Mat &img)
{
    std::vector<uchar> buff(img.rows * img.cols, 0);                              //Init the size of the Buffer
    cv::imencode(".pgm", img, buff, std::vector<int>{cv::IMWRITE_PXM_BINARY, 0}); //Encodes the img as a .pgm and writes into buff using the flag
    for (std::vector<uchar>::iterator it = buff.begin(); it != buff.end(); ++it)  //Iterating over the vector using a iterator given by .begin()
    {
        std::cout << *it; //Dereferencing to get the content to which the iterator is currently pointing
    }
}