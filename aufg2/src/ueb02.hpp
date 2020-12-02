/**-------System Header-------*/
#include <functional>
#include <opencv2/opencv.hpp>

namespace bba
{

    /**
    * Der Typ der Funktion die benutzt wird, um festzusellen welche Werte im Randbereich bei Faltung genutzt werden sollen 
    */
    typedef std::function<float(cv::Mat &origImg, int kernelY, int kernelX)> EdgeFunc;

    /**
    * Erstellt aus den vom Benutzer übergebenen Parametern ein Bild.
    * Der erste Wert im Vektor gibt die Breite an, der zweite die Höhe.
    * 
    * @param values Die Bilddaten.
    * @return Ein Bild aus den Bilddaten.
    */
    cv::Mat values(const std::vector<float> &values);

    /**
    * Fueht eine Faltung zweiter Bilder durch.
    * 
    * fold geht davon aus, dass die Matrix quadratisch ist 
    * und eine ungerade Anzahl an Pixeln auf jeder Seite hat,
    * sodass die Mitte eindeutig bestimmt ist.
    * 
    * @param kernel der Anzuwendende Filterkernel/Matrix.
    * @param img das Zielbild auf welchem der Kernel angewendet wird.
    * @return Das Faltungsergebniss.
    */
    cv::Mat fold(cv::Mat &kernel, cv::Mat &img, bba::EdgeFunc &treatment);

    /**
    * Erzeugt einen 3x3 Box-Kernel
    * 
    * @return der 3x3 Kernel.
    */
    cv::Mat box(void);

    /**
    * Erzeugt einen 5x5 Gauss-Kernel
    * 
    * @return der 5x5 Kernel.
    */
    cv::Mat gaussian(void);

    /**
    * Erzeugt einen 3x3 Sobel-X-Kernel
    * 
    * @return der 3x3 Kernel.
    */
    cv::Mat sobelX(void);

    /**
    * Erzeugt einen 3x3 Sobel-Y-Kernel
    * 
    * @return der 3x3 Kernel.
    */
    cv::Mat sobelY(void);

    /**
    * Erzeugt einen 3x3 Laplace-Kernel
    * 
    * @return der 3x3 Kernel.
    */
    cv::Mat laplace(void);

    /**
     * Wendet einen Mittelwertfilter (3 x 3) an
     * @param img das Bild auf dem der Filter angewendet werden soll
     * @return Das Ergebniss des Filters
     */
    cv::Mat median(cv::Mat &img);

    /**
     * Erosion wählt das einfache Minimum aus den neun (3 x 3) umliegenden Werten aus.
     * @param img das Bild auf dem der Filter angewendet werden soll
     * @return Das Ergebniss des Filters
     */
    cv::Mat erode(cv::Mat &img);

    /**
     * Dilatation wählt das einfache Maximum aus den neun (3 x 3) umliegenden Werten aus
     * @param img das Bild auf dem der Filter angewendet werden soll
     * @return Das Ergebniss des Filters
     */
    cv::Mat dilate(cv::Mat &img);

    /**
     * Wertet ein Bild aus und gibt entweder "treppe" oder "wald" auf std::cout aus.
     * @param img das auzuwertende Bild
     * @param treatment Funktion zum Umgang mit Bildkanten bei Faltung
     */
    void discriminator(cv::Mat &img, bba::EdgeFunc &treatment);

    /**
     * Stellt eine bba::EdgeFunc Funktion bereit, die zu einem Pixel ausserhalb
     * des Bildes einen passenden Pixel findet, indem das Bild an den aeusseren
     * Kanten gespiegelt wird.
     * Vorgehen:
     * 1) Skalen gleichsetzen, sodass gleiche Buchstaben an gleichen Positionen sind
     *    --> Nur einmal pro Seite rechnen muessen
     * 2) Abstand finden, ab dem sich das Muster wiederholt
     * 3) Berechnete Zahlen auf index im Bild abbilden
     *
     * Skalen gleichsetzen, sodass Zaehlung für negative Seiten (links und oben)
     * immer bei 2*m.cols + 1 beginnt.
     * Dadurch sind geleiche Buchstaben an der gleichen Stelen und oben ist
     * auf unten bzw. links auf rechts abgebildet
     * Notwendig, da wir festgelegt haben, dass oben und links drueber bei -1 starten,
     * unten und rechts drueber bei (m.cols bzw. m.rows) + 1
     * Oberer Teil eines Bildes:
     *     (-1,-1)    (-1, 0) (-1, 1)    (-1, 2)
     *             __________________
     *   ( 0,-1)  | ( 0, 0) ( 0, 1) |  ( 0, 2)
     *   ( 1,-1)  | ( 1, 0) ( 1, 1) |  ( 1, 2)
     * @return Die reflect11 Funktion
     */
    bba::EdgeFunc getEdgeReflect11Func(void);

    /**
     * Stellt eine bba::EdgeFunc Funktion bereit, die zu einem Pixel ausserhalb
     * des Bildes einen passenden Pixel findet, indem das Bild an dem letzten
     * Wert vor der aeusseren Kante gespiegelt wird.
     * Vorgehen, wie bei getEdgeReflect11Func().
     * @return Die reflect101 Funktion
     */
    bba::EdgeFunc getEdgeReflect101Func(void);

    /**
     * Stellt eine bba::EdgeFunc Funktion bereit, die zu einem Pixel ausserhalb
     * des Bildes einen passenden Pixel findet, der naechste Bildpixel
     * zurueckgegeben wird.
     * @return Die replicate Funktion
     */
    bba::EdgeFunc getEdgeReplicateFunc(void);

    /**
     * Stellt eine bba::EdgeFunc Funktion bereit, die zu einem Pixel ausserhalb
     * des Bildes einen passenden Pixel findet, indem immer der Wert 0 zurueck-
     * gegeben wird
     * @return Die edge null Funktion
    */
    bba::EdgeFunc getEdgeNullFunc(void);

    /**
     * Stellt eine bba::EdgeFunc Funktion bereit, die zu einem Pixel ausserhalb
     * des Bildes einen passenden Pixel findet, indem immer der uebergebene
     * Wert zurueckgeliefert wird.
     * @return Die edge value Funktion
    */
    bba::EdgeFunc getEdgeValueFunc(float value);
} // namespace bba
