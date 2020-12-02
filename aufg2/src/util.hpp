/**-------System Header-------*/
#include <opencv2/opencv.hpp>
#include <functional>

namespace util
{

    /**
    * Hilfsfunkiton, die jeden Pixel im uebergebenen Bild einmal durchlaeuft
    * und eine Funtkion darauf anwendet.
    * 
    * @param img Referenz auf eine Bildmatrix
    * @param f Funktion, die auf jeden Pixel angewendet wird
    */
    void writeToCout(cv::Mat &img);

    /**
    * Funktion zum ausgeben einer pgm auf der Standartausgabe.
    * Annahme: Es werden nur 8 Bit Portable Graymaps genutzt
    * @param img Referenz auf eine Bildmatrix 
    */
    void forEachPixel(cv::Mat &img, const std::function<void(cv::Mat &m, int y, int x)> &f);

    /**
    * Hilfsmethode welche eine cv::Mat erstellt aus dem uebergebenen vector
    * mit den enthaltenden Werten welche ab dem angegebenen offset anfangen
    * @param das Bild welches gefuellt werden soll.
    * @param offset der offset ab dem die Werte in dem vector interpretiert werden sollen.
    * @param values der vector in dem die Werte fuer die cv::Mat stehen.
    * @return das befuellte Bild
    */
    cv::Mat fillMat(cv::Mat &imgToFill, int offset, const std::vector<float> &values);

    /**
    * Liefert die Umliegenden Werte eines Pixels (3x3) in einem Bild als vector
    * @param m Das Bild in dem die umliegenden Werte gesucht werden
    * @param y der aktuelle y Wert des betrachteten Pixels
    * @param x der aktuelle x Wert des betrachteten Pixels
    * @return der vector mit den umliegenden Intensitaeten der Pixel
    */
    std::vector<float> getSurroundingValues(cv::Mat &m, int y, int x);

    /**
     * Gibt das Bild als Matrix auf der Standardausgabe aus.
     * 255 soll als weiß interpretiert werden. Bei Bildern, bei denen
     * 1 als weiß interpretiert wird: Schalter auf false setzen, damit skaliert
     * wird.
     * @param img das Bild als cv::Mat
     * @param base255 soll das Bild in 255er Wertebreich ausgegeben werden?
     * */
    void printMatrix(cv::Mat &img, bool base255);

    /**
     * Rotiert eine Matrix um 180 Grad
     * @param img die Matrix, die rotiert UND veraendert wird
     * */
    void rotateMatrix180(cv::Mat &m);
} // namespace util
