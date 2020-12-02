/**-------Eigene Header-------*/
#include "ueb01.hpp"
#include "util.hpp"

/**-------System Header-------*/
#include <iostream>
#include <math.h>
#include <opencv2/imgproc.hpp>
#include <iomanip>

/**
 * Filter, der jeden Pixel im Bild einmal durchlaeuft und nichts damit macht.
 * 
 * @param img Referenz auf eine Bildmatrix
 * */
bool bba::identity(cv::Mat &img)
{
    util::forEachPixel(img, [](cv::Mat &m, int y, int x) {
        m.at<float>(y, x) = m.at<float>(y, x); //Wert von y,x auf y,x zuweisen aka. nichts tun
    });
    return true;
}

/**
 * Filter, der die Helligkeit auf jedem Pixel um einen uebergebenen Wert erhoeht oder verringert
 * 
 * @param img Referenz auf eine Bildmatrix
 * @param value der Wert um den die Helligkeit veraendert wird
 * */
bool bba::brightness(cv::Mat &img, float value)
{
    auto brighter = [&value](cv::Mat &m, int y, int x) {
        m.at<float>(y, x) = m.at<float>(y, x) + value; //i + a
    };
    util::forEachPixel(img, brighter);

    return true;
}

/**
 * Filter, der die Intensitaeten jedes Pixels um einen Faktor erhoeht
 * 
 * @param img Referenz auf eine Bildmatrix
 * @param value Faktor um der die Intensitaet erhoeht wird
 * */
bool bba::factor(cv::Mat &img, float value)
{
    auto factor = [&value](cv::Mat &m, int y, int x) {
        m.at<float>(y, x) = m.at<float>(y, x) * value; //i*a
    };
    util::forEachPixel(img, factor);
    return true;
}

/**
 * Filter, welcher die Intensitaeten des Bildes linear anpasst
 * 
 * @param img Referenz auf eine Bildmatrix
 * @param slope die Steigung der Funktion
 * @param y_intercept der Y-Achsen Abschnitt
 * */
bool bba::linear(cv::Mat &img, float slope, float y_intercept)
{
    auto linear = [&slope, &y_intercept](cv::Mat &m, int y, int x) {
        m.at<float>(y, x) = m.at<float>(y, x) * slope + y_intercept; //m*i+b
    };
    ;
    util::forEachPixel(img, linear);
    return true;
}

/**
 * Filter, welcher den Konstrast des Bildes anhand einen linearen Abbildung aendert
 * 
 * @param img Referenz auf eine Bildmatrix
 * @param value der Wert des Steigung
 * */
bool bba::contrast(cv::Mat &img, float value)
{
    auto contrast = [&value](cv::Mat &m, int y, int x) {
        m.at<float>(y, x) = (m.at<float>(y, x) - 0.5) * value + 0.5; //(i-0.5)*a + 0.5
    };
    util::forEachPixel(img, contrast);
    return true;
}

/**
 * Filter, welcher eine Gamma anpassung durchfueht
 * 
 * @param img Referenz auf eine Bildmatrix
 * @param value der Wert um den jeder Pixel potenziert wird
 * */
bool bba::gamma(cv::Mat &img, float value)
{
    auto gamma = [&value](cv::Mat &m, int y, int x) {
        m.at<float>(y, x) = pow(m.at<float>(y, x), value); //i^a
    };
    util::forEachPixel(img, gamma);
    return true;
}

/**
 * Filter, welcher den Wertebereich einer Bildmatrix zwichen [0,...,1] begrenzt
 * 
 * @param img Referenz auf eine Bildmatrix
 * */
bool bba::clamp(cv::Mat &img) //Wie loesen wir das mit dem einlesen und Testen der Methode?
{
    auto clamp = [](cv::Mat &m, int y, int x) {
        float valueAt = m.at<float>(y, x);
        if (valueAt < 0.0)
        {
            m.at<float>(y, x) = 0.0;
        }
        else if (valueAt > 1.0)
        {
            m.at<float>(y, x) = 1.0;
        }
        else
        {
            m.at<float>(y, x) = valueAt;
        }
    };
    util::forEachPixel(img, clamp);
    return true;
}

/**
 * Filter, welcher zwei Bilder summiert und das neue Bild als Ergebniss liefert
 * 
 * @param img1 Referenz auf die erste Bildmatrix
 * @param img2 Referenz auf die zweite Bildmatrix
 * @return Die neu entstandene Bildmatrix
 * */
cv::Mat bba::add(cv::Mat &img1, cv::Mat &img2)
{
    auto add = [&img1, &img2](cv::Mat &m, int y, int x) {
        m.at<float>(y, x) = img1.at<float>(y, x) + img2.at<float>(y, x);
    };
    cv::Mat newImg = cv::Mat(img1.rows, img1.cols, CV_32F);
    util::forEachPixel(newImg, add);
    return newImg;
}

/**
 * Filter, welcher von einem Bild ein anderes subtrahiert und das neue Bild als Ergebniss liefert
 * 
 * @param img1 Referenz auf die erste Bildmatrix
 * @param img2 Referenz auf die zweite Bildmatrix
 * @return Die neu entstandene Bildmatrix
 * */
cv::Mat bba::sub(cv::Mat &img1, cv::Mat &img2)
{
    auto sub = [&img1, &img2](cv::Mat &m, int y, int x) {
        m.at<float>(y, x) = img1.at<float>(y, x) - img2.at<float>(y, x);
    };
    cv::Mat newImg = cv::Mat(img1.rows, img1.cols, CV_32F);
    util::forEachPixel(newImg, sub);
    return newImg;
}

/**
 * Filter, welcher die absolute Differenz zwischen zwei Bildern bildet und das neue Bild als Ergebniss liefert
 * 
 * @param img1 Referenz auf die erste Bildmatrix
 * @param img2 Referenz auf die zweite Bildmatrix
 * @return Die neu entstandene Bildmatrix
 * */
cv::Mat bba::adiff(cv::Mat &img1, cv::Mat &img2)
{
    auto sub = [&img1, &img2](cv::Mat &m, int y, int x) {
        m.at<float>(y, x) = abs(img1.at<float>(y, x) - img2.at<float>(y, x));
    };
    cv::Mat newImg = cv::Mat(img1.rows, img1.cols, CV_32F);
    util::forEachPixel(newImg, sub);
    return newImg;
}

/**
 * Hilfsfunktion welche ein Histogramm berechnet und dieses in den uebergebenen Vektor schreibt
 * 
 * @param img Referenz auf eine Bildmatrix
 * @param bins Anzahl der Wertabstufungen
 * @param hist Referenz auf einen Vektor der groesse bins welcher mit den Werten befuellt wird
 * */
void bba::calcHistogramm(cv::Mat &img, int bins, std::vector<float> &hist)
{
    float binWidth = 1.0 / bins;
    auto calcHist = [&bins, &binWidth, &hist](cv::Mat &m, int y, int x) {
        float intensity = m.at<float>(y, x);
        bool found = false;
        //Ueber alle bins laufen solange der passende nicht gefunden
        for (int i = 0; i < bins && !found; i++)
        {
            //Wenn Wert unter der Bin-Grenze, dann lieg es innerhalb
            if (intensity <= binWidth * (i + 1))
            {
                hist[i] += 1.0 / (m.rows * m.cols); //Normiert uber die Aufloesung des Bildes
                //Damit es fuer diesen Pixel die Bins nicht weiter ablaufen muss
                found = true;
            }
        }
    };
    util::forEachPixel(img, calcHist);
}

/**
 * Hilfsfunktion welche ein uebergebenes Histogramm auf stdout schreibt.
 * Das Format ist [X.XX, X.XX]
 * 
 * @param hist Referenz auf einen Vektor welcher ausgegeben werden soll
 * */
void bba::printHistogramm(std::vector<float> &hist)
{
    //2 Nachkommastellen setzen mit prinf und %g
    printf("[");
    for (std::vector<float>::iterator itr = std::begin(hist); itr != std::end(hist); itr++)
    {
        if ((next(itr) == hist.end()))
        {
            printf("%.2g", round(*itr * 100.0) / 100.0);
        }
        else
        {
            printf("%.2g, ", round(*itr * 100.0) / 100.0);
        }
    }
    printf("]\n");
}

/**
 * Pseudo-Filter welcher das Histogramm eines uebergebenen Bildes berechnet und dieses auf stdout ausgibt.
 * Das Bild wird dabei nicht veraendert.
 *  
 * @param img Referenz auf eine Bildmatrix
 * @param bins Anzahl der Wertabstufungen
 * */
bool bba::histogramm(cv::Mat &img, int bins)
{
    std::vector<float> hist(bins);
    bba::calcHistogramm(img, bins, hist);
    bba::printHistogramm(hist);
    return true;
}

/**
 * Pseudo-Filter welcher das Summenhistogramm eines uebergebenen Bildes berechnet und dieses auf stdout ausgibt.
 * Das Bild wird dabei nicht veraendert.
 *  
 * @param img Referenz auf eine Bildmatrix
 * @param bins Anzahl der Wertabstufungen
 * */
bool bba::sumHistogramm(cv::Mat &img, int bins)
{
    std::vector<float> hist(bins);
    bba::calcHistogramm(img, bins, hist);
    //Summieren der Werte im berechneten Histogramm
    for (int i = 1; i < bins; i++)
    {
        hist[i] += hist[i - 1];
    }
    bba::printHistogramm(hist);
    return true;
}

/**
 * Filter, welcher eine "automatische Kontrastanpassung" des gesamten Bildes anhand einer linearen Abbildung durchfuehrt.
 * Der Kleineste Wert im Bild wird auf 0 der groesste auf 1 gesetzt. Werte dazwichen werden linear abgebildet.
 * 
 * @param img Referenz auf eine Bildmatrix
 * @return Die angepaqsste Bildmatrix
 * */
cv::Mat bba::normalize(cv::Mat &img)
{
    float min = std::numeric_limits<float>::max();
    float max = std::numeric_limits<float>::lowest();
    auto minmax = [&min, &max](cv::Mat &m, int y, int x) {
        float value = m.at<float>(y, x);
        min = std::min(min, value);
        max = std::max(max, value);
    };

    auto normalize = [&min, &max](cv::Mat &m, int y, int x) {
        m.at<float>(y, x) = ((m.at<float>(y, x) - min) / (max - min)) * 1.0; // ((Wert-min) / (max-min) )* maxIntensity
    };
    util::forEachPixel(img, minmax);
    std::cout << "min: " << min << std::endl;
    std::cout << "max: " << max << std::endl;
    if (min != max) //Normalisiere nur, wenn das Bild mehr als einen Wert hat
    {
        util::forEachPixel(img, normalize);
    }
    return img;
}

/**
 * Hilfsfunktion, die das Ergebnis des Histograms auf std::cout ausgibt.
 * @param res Das Ergebnis des Checks als Wert des HistResult enums
 * */
static void printHistogrammCheckResult(bba::HistResult res)
{
    switch (res)
    {
    case bba::HIST_BRIGHTNESS_LOW:
        std::cout << "Bild ist dunkel." << std::endl;
        break;
    case bba::HIST_BRIGHTNESS_HIGH:
        std::cout << "Bild ist hell." << std::endl;
        break;
    case bba::HIST_CONTRAST_HIGH:
        std::cout << "Kontrast ist niedrig." << std::endl;
        break;
    case bba::HIST_CONTRAST_OK:
        std::cout << "Kontrast ist gut." << std::endl;
        break;
    case bba::HIST_NOT_ANALYZED:
        std::cout << "Histogram nicht analysiert." << std::endl;
        break;
    }
}

/**
 * Pseudo-Filter welcher die Qualitaet eines Bildes anhand seines Histogramms bewertet.
 * Das Urteil wird als Text auf der Standardausgabe ausgegeben.
 * Das Bild wird dabei nicht veraendert.
 *  
 * @param img Referenz auf eine Bildmatrix
 * */
bool bba::histogrammCheck(cv::Mat &img)
{
    //Fuer nachkommagenauigkeit bei stderr ausgabe
    std::cerr << std::setprecision(2) << std::fixed;
    const float upperBorder = 0.90;
    const float lowerBorder = 0.10;
    const float brightnessThreshold = 0.25;
    const int contrastIntervalThreshold = 100;
    int bins = 256;
    int lowestEntry = 0;
    int highestEntry = 0;
    float lowerBins = bins * lowerBorder;
    float upperBins = bins * upperBorder;
    float lowerPercentage = 0;
    float upperPercentage = 0;
    HistResult res = bba::HIST_NOT_ANALYZED;

    std::vector<float>
        hist(bins);

    bba::calcHistogramm(img, bins, hist);

    //Die niedrigen Intensitaetswerte ablaufen
    for (int i = 0; i < lowerBins; i++)
    {
        lowerPercentage += hist[i];
    }
    //Wenn bei den niedrigen Intensitaetswerten viele gefunden wurden
    if (lowerPercentage >= brightnessThreshold)
    {
        res = bba::HIST_BRIGHTNESS_LOW;
        std::cerr << (lowerPercentage * 100) << "% der Intensitaetswerte befinden sich in den unteren 10% des Wertebereichs" << std::endl;
    }
    //Die oberen Intensitaetswerte ablaufen
    for (int i = upperBins; res == bba::HIST_NOT_ANALYZED && i < bins; i++)
    {
        upperPercentage += hist[i];
    }
    //Wenn bei den hohen Intensitaetswerten viele gefunden wurden
    if (upperPercentage >= brightnessThreshold)
    {
        res = bba::HIST_BRIGHTNESS_HIGH;
        std::cerr << (upperPercentage * 100) << "% der Intensitaetswerte befinden sich in den oberen 10% des Wertebereichs" << std::endl;
    }

    //Wenn Helligkeit okay, dann den Kontrast pruefen
    if (res == bba::HIST_NOT_ANALYZED)
    {
        for (int i = 0; i < bins; i++)
        {
            if (hist[i] != 0)
            {
                if (lowestEntry == 0)
                {
                    lowestEntry = i;
                }
                highestEntry = i;
            }
        }
        //Wenn Kontrastinvervall zu niedrig
        if (highestEntry - lowestEntry < contrastIntervalThreshold)
        {
            res = HIST_CONTRAST_HIGH;
            std::cerr << "Das Intervall zwischen niedrigsten und hoechsten Intensitaetswert !=0 betraegt: " << (highestEntry - lowestEntry) << std::endl;
        }
        else
        {
            res = HIST_CONTRAST_OK;
            std::cerr << "Das Intervall zwischen niedrigsten und hoechsten Intensitaetswert !=0 betraegt: " << (highestEntry - lowestEntry) << std::endl;
        }
    }
    printHistogrammCheckResult(res);

    return true;
}
