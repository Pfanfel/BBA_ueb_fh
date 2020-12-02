/**-------System Header-------*/
#include <iostream>
#include <cmath>
#include <opencv2/imgproc.hpp>
#include <iomanip>

#include "ueb04.hpp"

/*---Funktionsprototypen---*/
static cv::Mat adjustTemplate(cv::Mat &img, cv::Mat templ, cv::Point &matchLoc, int &roiWidth, int &counter);
static cv::Point smoothenMatchLocation(std::deque<cv::Point> &prevMatchLocations, cv::Point matchLoc);
static void onClick(int event, int x, int y, int, void *img);

cv::Mat bba::getTemplateFromImage(cv::Mat &img, int x, int y, int width)
{
    cv::Mat tmplt;
    cv::Rect roi;
    roi.x = x - ((width - 1) / 2);
    roi.y = y - ((width - 1) / 2);
    roi.width = width;
    roi.height = width;

    img(roi).copyTo(tmplt);
    return tmplt;
}

cv::Rect bba::getSubregionAroundPoint(cv::Mat &img, cv::Point &trackLoc, int width)
{
    cv::Rect subregion;
    int half = (width - 1) / 2;

    int top, right, bottom, left;
    //Begrenzungen der 4 Seiten berechnen
    top = (trackLoc.y - half) >= 0 ? trackLoc.y - half : 0;
    right = (trackLoc.x + half) < img.cols ? trackLoc.x + half : img.cols - 1;
    bottom = (trackLoc.y + half) < img.rows ? trackLoc.y + half : img.rows - 1;
    left = (trackLoc.x - half) >= 0 ? trackLoc.x - half : 0;

    subregion.x = left;
    subregion.y = top;

    subregion.height = bottom - top + 1;
    subregion.width = right - left + 1;

    return subregion;
}

cv::Mat bba::trackFeature(cv::Mat &img, bba::TrackParameters &params, bool &first, cv::Mat &templ, int &counter, std::deque<cv::Point> &prevMatchLocations)
{
    cv::Mat result;

    if (first)
    {
        templ = bba::getTemplateFromImage(img, params.trackLoc.x, params.trackLoc.y, params.templateWidth);
        first = false;
    }

    cv::Rect subregion = bba::getSubregionAroundPoint(img, params.trackLoc, SUBREGION_WIDTH);

    //Groesse der Filterantwort definieren
    //Kleiner als das src-Bild um groesse des Templ.
    int result_cols = subregion.width - templ.cols + 1;
    int result_rows = subregion.height - templ.rows + 1;
    result.create(result_rows, result_cols, CV_32FC1);

    //Andere Verfahren: TM_SQDIFF, TM_CCORR, TM_CCORR_NORMED ...
    cv::matchTemplate(img(subregion), templ, result, cv::TM_SQDIFF_NORMED);
    //Auf wertebrereich zwischen 0-255 normalisieren
    cv::normalize(result, result, 0, 1, cv::NORM_MINMAX, -1, cv::Mat());

    double minVal;
    double maxVal;
    cv::Point minLoc;
    cv::Point maxLoc;
    cv::minMaxLoc(result, &minVal, &maxVal, &minLoc, &maxLoc, cv::Mat());
    //Je nach Verfahren wird hier der max oder min Wert zugewiesen
    //TM_SQDIFF_NORMED -> min
    params.trackLoc = minLoc;

    //Gefundene Pos. um groessenunterschied zwischen res und subregion korrigieren
    //Das Resultbild ist kleiner als subregion, weil template ganz raufpassen muss zum matchen
    params.trackLoc.x += (templ.cols - 1) / 2;
    params.trackLoc.y += (templ.rows - 1) / 2;

    //Subregion Koordianten in Bildkoordianten
    params.trackLoc.x += subregion.x;
    params.trackLoc.y += subregion.y;

    //Template aktualisieren
    templ = adjustTemplate(img, templ, params.trackLoc, params.templateWidth, counter);

    //params.trackLoc in Liste mit vorherigen tackLocs schreiben und Mittelwert bilden
    params.trackLoc = smoothenMatchLocation(prevMatchLocations, params.trackLoc);

    //Text neben den gematchten Punkt
    cv::Point txtPos(params.trackLoc.x + templ.cols, params.trackLoc.y + templ.rows);

    cv::putText(img,              //target image
                params.trackText, //text
                txtPos,           //top-left position
                cv::FONT_HERSHEY_DUPLEX,
                1.0,
                CV_RGB(params.textColorRGB[0], params.textColorRGB[1], params.textColorRGB[2]), //font color BGR
                2);

    return img;
}

cv::Point bba::trackFeatureGUI(cv::Mat &img)
{
    cv::Point clickLoc(-1, -1);
    //Kopie und nicht das Original anzeigen
    cv::Mat displayCopy = img.clone();
    cv::namedWindow("Choose Tracking Location", 1);
    cv::setMouseCallback("Choose Tracking Location", onClick, &clickLoc);

    while (clickLoc.x == -1 && clickLoc.y == -1)
    {
        cv::imshow("Choose Tracking Location", displayCopy);
        cv::waitKey(0);
        cv::destroyWindow("Choose Tracking Location");
    }

    return clickLoc;
}

bba::TrackParameters bba::createTrackParams(bba::Operation op)
{
    bba::TrackParameters params;
    if (op.values.size() == 6)
    {
        params.trackLoc.x = op.values[0];
        params.trackLoc.y = op.values[1];
        params.templateWidth = op.values[2];
        params.textColorRGB.push_back(op.values[3]);
        params.textColorRGB.push_back(op.values[4]);
        params.textColorRGB.push_back(op.values[5]);
        params.trackText = op.trackText;
    }
    else if (op.values.size() == 4)
    {
        params.templateWidth = op.values[0];
        params.textColorRGB.push_back(op.values[1]);
        params.textColorRGB.push_back(op.values[2]);
        params.textColorRGB.push_back(op.values[3]);
        params.trackText = op.trackText;
    }
    return params;
}

static cv::Mat adjustTemplate(cv::Mat &img, cv::Mat templ, cv::Point &matchLoc, int &roiWidth, int &counter)
{
    //per Konstante einstellbare Neuberechnungsrate des Templates
    if (counter % bba::TEMPLATE_UPDATE_COUNT == 0 && counter != 0)
    {
        templ = bba::getTemplateFromImage(img, matchLoc.x, matchLoc.y, roiWidth);
    }
    counter++;
    return templ;
}

static cv::Point getRoundedAveragePoint(std::deque<cv::Point> points)
{
    cv::Point result;
    int x = 0;
    int y = 0;
    for (cv::Point p : points)
    {
        x += p.x;
        y += p.y;
    }
    result.x = round(x / points.size());
    result.y = round(y / points.size());
    return result;
}

static cv::Point smoothenMatchLocation(std::deque<cv::Point> &prevMatchLocations, cv::Point matchLoc)
{
    cv::Point newMatchLoc = matchLoc;

    if (prevMatchLocations.size() >= bba::LOCATION_BUFFER_SIZE)
    {
        //Location vorne entfernen
        prevMatchLocations.pop_front();
    }
    //neue Location vorne entfernen
    prevMatchLocations.push_back(matchLoc);

    //Durchschnitt Bilden
    newMatchLoc = getRoundedAveragePoint(prevMatchLocations);

    return newMatchLoc;
}

//onClick hat Signatur: onClick(int event, int x, int y, int flags, void *data),
//desshalb muss C-Style Pointer Syntax benutzt werden bei data.
static void onClick(int event, int x, int y, int, void *data)
{
    if (event == cv::EVENT_LBUTTONDOWN)
    {
        //void-Pointer zu cv::Point-Pointer casten und auf dessen x und y Member schreiben
        ((cv::Point *)data)->x = x;
        ((cv::Point *)data)->y = y;
    }
}
