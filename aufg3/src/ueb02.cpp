/**-------Eigene Header-------*/
#include "ueb02.hpp"
#include "ueb01.hpp"
#include "util.hpp"

/**-------System Header-------*/
#include <iostream>
#include <math.h>
#include <opencv2/imgproc.hpp>
#include <iomanip>

static float useKernel(cv::Mat &kernel, cv::Mat &img, int y, int x, bba::EdgeFunc &treatment)
{
    int kernelHeight = kernel.cols;
    int kernelWidth = kernel.rows;
    int imgHeight = img.cols;
    int imgWidth = img.rows;
    float result = 0;

    //Umrechnung von Kernelkoordinaten in Quellbildkoordinaten
    for (int yOffset = -((kernelHeight - 1) / 2); yOffset <= ((kernelHeight - 1) / 2); yOffset++)
    {
        for (int xOffset = -((kernelWidth - 1) / 2); xOffset <= ((kernelWidth - 1) / 2); xOffset++)
        {
            //Koordinaten im Bild
            int kernelInImgY = yOffset + y;
            int kernelInImgX = xOffset + x;
            //Koordinaten in Kernel
            int kernelY = yOffset + (kernelHeight - 1) / 2;
            int kernelX = xOffset + (kernelWidth - 1) / 2;
            //Pruefen ob Pixel im Wertebereich
            /*  links aussen      | rechts aussen            | oben drueber      | unten drunter */
            if (kernelInImgX < 0 || kernelInImgX >= imgWidth || kernelInImgY < 0 || kernelInImgY >= imgHeight)

            {
                result += kernel.at<float>(kernelY, kernelX) * treatment(img, kernelInImgY, kernelInImgX);
                //std::cout << treatment(img, kernelInImgY, kernelInImgX) << std::endl;
            }
            else //Alles okay
            {
                result += kernel.at<float>(kernelY, kernelX) * img.at<float>(kernelInImgY, kernelInImgX);
            }
        }
    }
    return result;
}

cv::Mat bba::fold(cv::Mat &kernel, cv::Mat &img, bba::EdgeFunc &treatment)
{
    util::rotateMatrix180(kernel); //Damit gefaltet und nicht korreliert wird
    // std::cout << "ueb02.cpp - der Kernel: " << std::endl
    //           << kernel << std::endl;
    // std::cout << "ueb02.cpp - das Bild: " << std::endl
    //           << img << std::endl;

    auto fold = [&img, &kernel, &treatment](cv::Mat &m, int y, int x) {
        m.at<float>(y, x) = useKernel(kernel, img, y, x, treatment); //kernel anwendung
    };
    cv::Mat newImg = cv::Mat(img.rows, img.cols, CV_32F);
    util::forEachPixel(newImg, fold);
    return newImg;
}

cv::Mat bba::values(const std::vector<float> &values)
{
    cv::Mat img = cv::Mat(values[1], values[0], CV_32F); //Erzeugt ein Bild mit den uebergebenen Dimensionen
    return util::fillMat(img, 2, values);
}

cv::Mat bba::box(void)
{
    std::vector<float> boxValues = {1.0 / 9, 1.0 / 9, 1.0 / 9,
                                    1.0 / 9, 1.0 / 9, 1.0 / 9,
                                    1.0 / 9, 1.0 / 9, 1.0 / 9};

    cv::Mat result = cv::Mat(3, 3, CV_32F);

    return util::fillMat(result, 0, boxValues);
}

cv::Mat bba::gaussian(void)
{
    std::vector<float> gaussianValues = {0, 0, 0, 0, 0,
                                         0, (1 / 16.0), (2 / 16.0), (1 / 16.0), 0,
                                         0, (2 / 16.0), (4 / 16.0), (2 / 16.0), 0,
                                         0, (1 / 16.0), (2 / 16.0), (1 / 16.0), 0,
                                         0, 0, 0, 0, 0};

    cv::Mat res = cv::Mat(5, 5, CV_32F);

    return util::fillMat(res, 0, gaussianValues);
}

cv::Mat bba::sobelX(void)
{
    cv::Mat res(cv::Matx33f(-1, 0, 1,
                            -2, 0, 2,
                            -1, 0, 1));

    return cv::Mat(res);
}

cv::Mat bba::sobelY(void)
{
    cv::Mat res(cv::Matx33f(1, 2, 1,
                            0, 0, 0,
                            -1, -2, -1));

    return cv::Mat(res);
}

cv::Mat bba::laplace(void)
{
    cv::Mat res(cv::Matx33f(0, 1, 0,
                            1, -4, 1,
                            0, 1, 0));

    return cv::Mat(res);
}

cv::Mat bba::median(cv::Mat &img)
{
    cv::Mat resImg = cv::Mat(img.rows, img.cols, CV_32F);

    auto median = [&resImg](cv::Mat &m, int y, int x) {
        std::vector<float> surroundingPixel = util::getSurroundingValues(m, y, x); //Umliegende Pixel bestimmen
        float median = 0;
        sort(surroundingPixel.begin(), surroundingPixel.end()); //Aufsteigend sortieren
        //Checken wie viele im Vector und je nachdem den Median berechnen
        if (surroundingPixel.size() % 2 != 0) //Ungerade
        {
            median = surroundingPixel.at(surroundingPixel.size() / 2); //Ganzzahlige Division 3->[1], 5->[2], 7->[3], 9->[4]
        }
        else //Gerade -> Untermedian muss gebildet werden
        {
            median = surroundingPixel.at((surroundingPixel.size() / 2) - 1); //Untermedian 4-> (4/2) -1 = [1]
        }
        //Erg in neues Bild schreiben
        resImg.at<float>(y, x) = median;
    };

    util::forEachPixel(img, median); //Ueber Quellbild laufen und in Zielbild schreiben

    return resImg;
}

cv::Mat bba::erode(cv::Mat &img)
{
    cv::Mat resImg = cv::Mat(img.rows, img.cols, CV_32F);
    auto erode = [&resImg](cv::Mat &m, int y, int x) {
        std::vector<float> surroundingPixel = util::getSurroundingValues(m, y, x);           //Umliegende Pixel bestimmen
        float minimum = *std::min_element(surroundingPixel.begin(), surroundingPixel.end()); //Min. dessen bestimmen
        resImg.at<float>(y, x) = minimum;                                                    //Min. in neues Bild schreiben
    };
    util::forEachPixel(img, erode); //Ueber Quellbild laufen und in Zielbild schreiben

    return resImg;
}

cv::Mat bba::dilate(cv::Mat &img)
{
    cv::Mat resImg = cv::Mat(img.rows, img.cols, CV_32F);
    auto dilate = [&resImg](cv::Mat &m, int y, int x) {
        std::vector<float> surroundingPixel = util::getSurroundingValues(m, y, x);           //Umliegende Pixel bestimmen
        float maximum = *std::max_element(surroundingPixel.begin(), surroundingPixel.end()); //Max. dessen bestimmen
        resImg.at<float>(y, x) = maximum;                                                    //Max. in neues Bild schreiben
    };
    util::forEachPixel(img, dilate); //Ueber Quellbild laufen und in Zielbild schreiben

    return resImg;
}

void bba::discriminator(cv::Mat &img, bba::EdgeFunc &treatment)
{
    int bins = 256;
    std::vector<float> histHorizontal(bins);
    std::vector<float> histVertical(bins);
    cv::Mat gaussian = bba::gaussian();
    cv::Mat sobelX = bba::sobelX();
    cv::Mat sobelY = bba::sobelY();

    cv::Mat smoothed = cv::Mat(img.rows, img.cols, CV_32F);
    cv::Mat horizontalLines = cv::Mat(img.rows, img.cols, CV_32F);
    cv::Mat verticalLines = cv::Mat(img.rows, img.cols, CV_32F);
    smoothed = bba::fold(gaussian, img, treatment);
    verticalLines = bba::fold(sobelX, smoothed, treatment);
    horizontalLines = bba::fold(sobelY, smoothed, treatment);

    /*Damit die Werte fuer das Histogram im zulaesigen Wertebereich [0-1] liegen*/
    bba::clamp(verticalLines);
    bba::clamp(horizontalLines);

    bba::calcHistogramm(horizontalLines, bins, histHorizontal);
    bba::calcHistogramm(verticalLines, bins, histVertical);

    /*Wenn der Sobel Y mehr weiße Werte als der Sobel X liefert, befinden sich mehr horizontale Linien im Bild -> treppe*/
    histHorizontal.at(255) > histVertical.at(255) ? std::cout << "treppe" : std::cout << "wald";
    std::cout << std::endl;
}

bba::EdgeFunc bba::getEdgeReflect11Func(void)
{
    return [](cv::Mat &m, int kernelY, int kernelX) {
        int xPos, yPos;

        xPos = kernelX > 0 ? kernelX : abs(kernelX) + (2 * m.cols - 1);
        yPos = kernelY > 0 ? kernelY : abs(kernelY) + (2 * m.rows - 1);

        //Position der Wiederholung finden
        xPos %= 2 * m.cols;
        yPos %= 2 * m.rows;

        //Berechnete Werte auf Index im Bild abbilden
        if (xPos >= m.cols)
        {
            xPos = (2 * m.cols - 1) - xPos;
        }

        if (yPos >= m.rows)
        {
            yPos = (2 * m.rows - 1) - yPos;
        }
        return m.at<float>(yPos, xPos);
    };
}

bba::EdgeFunc bba::getEdgeReflect101Func(void)
{
    return [](cv::Mat &m, int kernelY, int kernelX) {
        //Vorgehen wie bei reflct11
        int xPos, yPos;

        //Skalen gleichsetzen
        xPos = kernelX > 0 ? kernelX : abs(kernelX) + (2 * m.cols - 2);
        //2*m.cols -2), weil die Reihe sich eine Stelle vorher wiederholt, als bei reflect11
        yPos = kernelY > 0 ? kernelY : abs(kernelY) + (2 * m.rows - 2);

        //Punkt der Wiederholung
        xPos %= 2 * (m.cols - 1);
        yPos %= 2 * (m.rows - 1);

        //Index bestimmen
        if (xPos >= m.cols)
        {
            xPos = (2 * m.cols - 2) - xPos;
        }

        if (yPos >= m.rows)
        {
            yPos = (2 * m.rows - 2) - yPos;
        }
        return m.at<float>(yPos, xPos);
    };
}

bba::EdgeFunc bba::getEdgeReplicateFunc(void)
{
    return [](cv::Mat &m, int kernelY, int kernelX) {
        int y = kernelY < 0 ? 0 : kernelY > (m.rows - 1) ? (m.rows - 1) : kernelY; //keine Prüfung, ob kernelY wirklich ausserhalb liegt
        int x = kernelX < 0 ? 0 : kernelX > (m.cols - 1) ? (m.cols - 1) : kernelX; //keine Prüfung, ob kernelX wirklich ausserhalb liegt
        return m.at<float>(y, x);
    };
}

bba::EdgeFunc bba::getEdgeNullFunc(void)
{
    return [](cv::Mat &, int, int) {
        return 0.0;
    };
}

bba::EdgeFunc bba::getEdgeValueFunc(float value)
{
    return [value](cv::Mat &, int, int) {
        return value;
    };
}