/**-------Eigene Header-------*/
#include "ueb03.hpp"
#include "ueb02.hpp"
#include "ueb01.hpp"
#include "util.hpp"

/**-------System Header-------*/
#include <iostream>
#include <math.h>
#include <opencv2/imgproc.hpp>
#include <iomanip>

/**-------Funktionsprototypen-------*/
static cv::Point calculateNewDimensions(cv::Mat &img, cv::Matx33f &transformationMatrix);
static float calcPixelIntensity(cv::Matx31f coord, cv::Mat src, bba::Interpolation interpolation, bba::EdgeFunc &treatment);

void bba::makeAffineMatrixRotate(cv::Matx33f &transformationMatrix, const std::vector<float> &vals)
{
    float degInRad = util::degreeToRad(vals[0]);
    cv::Matx33f res = {cosf(degInRad), -sinf(degInRad), 0,
                       sinf(degInRad), cosf(degInRad), 0,
                       0, 0, 1};
    transformationMatrix = res * transformationMatrix;
}

void bba::makeAffineMatrixScale(cv::Matx33f &transformationMatrix, const std::vector<float> &vals)
{
    cv::Matx33f res = {vals[0], 0, 0,
                       0, vals[1], 0,
                       0, 0, 1};
    transformationMatrix = res * transformationMatrix;
}

void bba::makeAffineMatrixShear(cv::Matx33f &transformationMatrix, const std::vector<float> &vals)
{
    cv::Matx33f res = {1, vals[0], 0,
                       0, 1, 0,
                       0, 0, 1};
    transformationMatrix = res * transformationMatrix;
}

void bba::makeAffineMatrixTranslate(cv::Matx33f &transformationMatrix, const std::vector<float> &vals)
{
    cv::Matx33f res = {1, 0, vals[0],
                       0, 1, vals[1],
                       0, 0, 1};
    transformationMatrix = res * transformationMatrix;
}

void bba::makeAffineMatrixTransform(cv::Matx33f &transformationMatrix, const std::vector<float> &vals)
{
    cv::Matx33f res = {vals[0], vals[1], 0,
                       vals[2], vals[3], 0,
                       0, 0, 1};
    transformationMatrix = res * transformationMatrix;
}

cv::Mat bba::affineTransform(cv::Mat &img, cv::Matx33f &transformationMatrix, bba::Interpolation interpolation, bba::EdgeFunc &treatment, bba::TargetImageData &targetImageData)
{
    //Neue Dimensionen des Zielbildes
    cv::Point resImageDimensions;
    //Identitaetsmatrix zum resetten der akkumulierenden Matrix
    cv::Mat identity = (cv::Mat_<float>(3, 3) << 1, 0, 0, 0, 1, 0, 0, 0, 1);

    //Falls eine Berechnung der Dimension notwendig ist
    if (targetImageData.calcNewDimensions)
    {
        resImageDimensions = calculateNewDimensions(img, transformationMatrix);
    }
    else //Bei upright explizit angegebene Groesse
    {
        resImageDimensions.x = targetImageData.width;
        resImageDimensions.y = targetImageData.height;
    }

    //Matrix fuer Translation des Bildes in den Koordinatenursprung (0,0)
    cv::Matx33f translateToCoordCenter = {1, 0, -img.cols / 2.0f + 0.5f,
                                          0, 1, -img.rows / 2.0f + 0.5f,
                                          0, 0, 1};

    //Matrix fuer Translation des Bildes aus den Ursprung (0,0) in den neuen Mitttelpunkt
    cv::Matx33f translateFromCoordCenter = {1, 0, resImageDimensions.x / 2.0f - 0.5f,
                                            0, 1, resImageDimensions.y / 2.0f - 0.5f,
                                            0, 0, 1};

    //Bild in richtiger groesse
    cv::Mat resImg = cv::Mat(resImageDimensions.y, resImageDimensions.x, CV_32FC1);

    cv::Matx33f totalTransformation = translateFromCoordCenter * transformationMatrix * translateToCoordCenter;
    cv::Matx33f totalTransformation_reverse = totalTransformation.inv();

    //Anonyme Funktion fuer die anwendung der Transformation auf jedem Pixel des Bildes
    auto transform = [&totalTransformation_reverse, &interpolation, &treatment, &img](cv::Mat &m, int y, int x) {
        cv::Matx31f currentPixel = {float(x), float(y), 1};
        //Auf jedem Pixel die Inverse Matrix anwenden
        cv::Matx31f nonDiscreteCoord = totalTransformation_reverse * currentPixel;
        m.at<float>(y, x) = calcPixelIntensity(nonDiscreteCoord, img, interpolation, treatment);
    };

    //Ueber das Quellbild laufen und in Zielbild schreiben
    util::forEachPixel(resImg, transform);

    //Nach angewandter Transformation die Matrix resetten fuer die naechsten Transformationen
    transformationMatrix = identity;
    return resImg;
}

void bba::makeAffineMatrixUpright(cv::Matx33f &transformationMatrix, const std::vector<float> &vals, bba::TargetImageData &targetImageData)
{
    //Werte zur Berechnung aus dem Vektor sammeln
    float height = vals[0];
    float width = vals[1];

    //Koordianten der Passpunkte im Quellbild
    float x1_src = vals[2];
    float y1_src = vals[3];
    float x2_src = vals[4];
    float y2_src = vals[5];
    float x3_src = vals[6];
    float y3_src = vals[7];

    //Relevante Werte fuer die folgende Anwendung der Transformation speichern
    targetImageData.width = width;
    targetImageData.height = height;
    targetImageData.calcNewDimensions = false;

    //Koordinaten in dem Zielbild der Passpunkte
    float x1_dest = 0;
    float y1_dest = (float)height - 1;
    float x2_dest = 0;
    float y2_dest = 0;
    float x3_dest = (float)width - 1;
    float y3_dest = 0;

    //Matrix fuer die loesung des LGS befuellen
    cv::Mat srcPoints = (cv::Mat_<float>(6, 6) << x1_src, y1_src, 1, 0, 0, 0,
                         0, 0, 0, x1_src, y1_src, 1,
                         x2_src, y2_src, 1, 0, 0, 0,
                         0, 0, 0, x2_src, y2_src, 1,
                         x3_src, y3_src, 0, 0, 0, 0,
                         0, 0, 0, x3_src, y3_src, 1);

    cv::Matx61f destPoints = {
        x1_dest,
        y1_dest,
        x2_dest,
        y2_dest,
        x3_dest,
        y3_dest};

    cv::Matx61f transformationParameters;
    //Die Transformation aus den Passpunkten berechnen
    int res = cv::solve(srcPoints, destPoints, transformationParameters);
    if (!res)
    {
        throw std::runtime_error("Src is singular.");
    }

    cv::Matx33f resultTransformation = {transformationParameters(0, 0), transformationParameters(0, 1), transformationParameters(0, 2),
                                        transformationParameters(0, 3), transformationParameters(0, 4), transformationParameters(0, 5),
                                        0, 0, 1};

    transformationMatrix = resultTransformation * transformationMatrix;
}

/**
 * Prueft ob im richtigen Wertebereich und entscheidet ob Treatment aufgerufen werden soll obder nicht.
 * @param src Das Quellbild
 * @param y die y Position im Quellbild
 * @param x die x Position im Quellbild
 * @param treatment die Randbehandlungsfunktion
 */
static float getIntensity(cv::Mat src, int y, int x, bba::EdgeFunc &treatment)
{
    float result = 0;
    if (x < 0 || x >= src.cols || y < 0 || y >= src.rows)
    {
        result = treatment(src, y, x);
    }
    else //Alles okay
    {
        result = src.at<float>(y, x);
    }
    return result;
}

/**
 * Berechnet fuer eine nichtdiskrete Koordinate die Intensität ausgehend aus dem dazugehoerigen Quellbild bei der Transformation
 * 
 * @param coord die nichtdiskrete Koordinate an der die Intensität berechnet werden soll.
 * @praram src das Quellbild aus dem die Intensität berechnet/entnommen wird.
 * @param interpolation die Interpolationsmethode, welche bei der bestimmung verwendet werden soll.
 * @param treatment die Randbehandlungsmethode
 * @return der berechnete Intensitätswert an der uebergebenen Koordinate
 * 
 */
static float calcPixelIntensity(cv::Matx31f coord, cv::Mat src, bba::Interpolation interpolation, bba::EdgeFunc &treatment)
{
    float intensity = 0;
    cv::Point2f posInSrc;
    posInSrc.x = coord(0, 0);
    posInSrc.y = coord(0, 1);

    switch (interpolation)
    {
    case bba::INTERPOLATE_NEAREST:
    {
        //Den naehesten Pixel zum undiskreten Pixelwert berechnen und im Ursprungsbild an dieser Stelle Intensitaet nehmen
        intensity = getIntensity(src, roundf(posInSrc.y), roundf(posInSrc.x), treatment);
        break;
    }
    case bba::INTERPOLATE_BILINEAR:
    {

        //Die naechsten 4 Punkte berechnen/holen
        float topLeft = getIntensity(src, floor(posInSrc.y), floor(posInSrc.x), treatment);

        float topRight = getIntensity(src, floor(posInSrc.y), ceil(posInSrc.x), treatment);

        float bottomLeft = getIntensity(src, ceil(posInSrc.y), floor(posInSrc.x), treatment);

        float bottomRight = getIntensity(src, ceil(posInSrc.y), ceil(posInSrc.x), treatment);

        //Die "ausseren Raender" des aktuell bertrachteten Pixels holen um die Abstaende zu berechen
        int leftBorder = floor(posInSrc.x);
        int lowerBorder = ceil(posInSrc.y);

        //Interpolieren mit kreuzweiser multiplikation der Abstaende

        //Abstand von "linker Kante" (xPos von topLeft bzw. bottomLeft)
        float deltaX = posInSrc.x - leftBorder;
        //Abstand von "unterer Kante" (yPos von bootomRight bzw. bottomLeft)
        float deltaY = lowerBorder - posInSrc.y;

        //Nach VL Formel die Interpolation berechnen
        intensity = (1 - deltaX) * (1 - deltaY) * bottomLeft + (1 - deltaX) * deltaY * topLeft + deltaX * (1 - deltaY) * bottomRight + deltaX * deltaY * topRight;
        break;
    }
    default:
    {
        throw std::runtime_error("Unbekannte Interpolation.");
        break;
    }
    }

    return intensity;
}

/**
 * Berechnet ausgehend von einem Bild und der daruaf anzuwendenden Transformationsmatrix die groesse des Zielbildes
 * 
 * @param img das Quellbild
 * @param transformationMatrix die daruaf anzuwendende Transformationsmatrix
 * @return die Breite und die Hoehe des Zielbildes 
 * 
 */
static cv::Point calculateNewDimensions(cv::Mat &img, cv::Matx33f &transformationMatrix)
{
    cv::Point newDimensions;
    float minWidth = 0;
    float minHeight = 0;

    cv::Matx31f topLeftOrig = {0, 0, 1};
    cv::Matx31f topRightOrig = {(float)img.cols, 0, 1};
    cv::Matx31f bottomRightOrig = {(float)img.cols, (float)img.rows, 1};
    cv::Matx31f bottomLeftOrig = {0, (float)img.rows, 1};

    cv::Matx31f topLeftTransformed = transformationMatrix * topLeftOrig;
    cv::Matx31f topRightTransformed = transformationMatrix * topRightOrig;
    cv::Matx31f bottomRightTransformed = transformationMatrix * bottomRightOrig;
    cv::Matx31f bottomLeftTransformed = transformationMatrix * bottomLeftOrig;

    minWidth = std::max({topLeftTransformed(0, 0), topRightTransformed(0, 0), bottomRightTransformed(0, 0), bottomLeftTransformed(0, 0)}) - std::min({topLeftTransformed(0, 0), topRightTransformed(0, 0), bottomRightTransformed(0, 0), bottomLeftTransformed(0, 0)});
    minHeight = std::max({topLeftTransformed(0, 1), topRightTransformed(0, 1), bottomRightTransformed(0, 1), bottomLeftTransformed(0, 1)}) - std::min({topLeftTransformed(0, 1), topRightTransformed(0, 1), bottomRightTransformed(0, 1), bottomLeftTransformed(0, 1)});

    newDimensions.x = static_cast<int>(roundf(minWidth));
    newDimensions.y = static_cast<int>(roundf(minHeight));

    return newDimensions;
}
