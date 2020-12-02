/**-------System Header-------*/
#include <functional>
#include <opencv2/opencv.hpp>
/**-------Eigene Header-------*/
#include "argparse.hpp"
#include "ueb02.hpp"

namespace bba
{

    /**
     * Interpolationsmethoden
     */
    enum Interpolation
    {
        INTERPOLATE_NEAREST,
        INTERPOLATE_BILINEAR,
    };

    /**
     * Zusatzinformationen fuer die Anwendung der Transformation
     */
    struct TargetImageData
    {
        bool calcNewDimensions;
        float width;
        float height;
    };

    /**
     * Generiert eine homogene 3x3 Rotationsmatrix
     * 
     * @param vals der Vector in dem der Winkel in Grad um den rotiert werden soll steht.
     * @param transformationMatrix Matrix in welche die generierte Matrix hineinmultipliziert wird
     */
    void makeAffineMatrixRotate(cv::Matx33f &transformationMatrix, const std::vector<float> &vals);

    /**
     * Generiert eine homogene 3x3 Skalierungsmatrix
     * 
     * @param vals der Vector in dem der Werte um die in x und y skaliert werden soll stehen.
     * @param transformationMatrix Matrix in welche die generierte Matrix hineinmultipliziert wird
     */
    void makeAffineMatrixScale(cv::Matx33f &transformationMatrix, const std::vector<float> &vals);

    /**
     * Generiert eine homogene 3x3 Scherungsmatrix
     * 
     * @param vals der Vector in dem der Factor um den geschert werden soll steht.
     * @param transformationMatrix Matrix in welche die generierte Matrix hineinmultipliziert wird
     */
    void makeAffineMatrixShear(cv::Matx33f &transformationMatrix, const std::vector<float> &vals);

    /**
     * Generiert eine homogene 3x3 Translationsmatrix
     * 
     * @param vals der Vector in dem die Werte um welche transliert werden soll steht.
     *        dabei in der Reihenfolge x, y
     * @param transformationMatrix Matrix in welche die generierte Matrix hineinmultipliziert wird
     */
    void makeAffineMatrixTranslate(cv::Matx33f &transformationMatrix, const std::vector<float> &vals);

    /**
     * Generiert eine homogene 3x3 Transformationsmatrix
     * 
     * @param vals der Vector in dem die Werte um welche transformiert werden soll steht.
     *        dabei in der Reihenfolge (x,y) (0,0) (0,1) (1,0) (1,1)
     * @param transformationMatrix Matrix in welche die generierte Matrix hineinmultipliziert wird
     */
    void makeAffineMatrixTransform(cv::Matx33f &transformationMatrix, const std::vector<float> &vals);

    /**
     * Wendet eine affine Transformation auf das Bild an
     * 
     * @param img das Bild auf das die Transformation angewendet wird
     * @param transformationMatrix Matrix mit welcher die Transformation angewendet werden soll 
     * @param treatment Funktion welche fuer die Randbehandlung verwendet werden soll
     * @param targetImageData zusaetzliche Informationen fuer die Anwendung der Transfomrationen
     * @return das Transformierte Bild
     */
    cv::Mat affineTransform(cv::Mat &img, cv::Matx33f &transformationMatrix, Interpolation interpolation, EdgeFunc &treatment, bba::TargetImageData &targetImageData);

    /**
     * Berechnet mit Hilfe der Passpunktmethode eine Transformationsmatrix, welche einen angegebenen Bildbereich wieder Rechteckig abbildet.
     * Der Filter nimmt als Parameter die Zielgröße sowie drei Koordinaten der Punkte links unten, links oben und rechts oben: Höhe Breite x₁ y₁ x₂ y₂ x₃ y₃ 
     * Da der Benutzer die Koordinaten angibt, kann es sein, dass Bildteile abgeschnitten werden oder zusätzlicher Rand entsteht.
     * 
     * @param transformationMatrix Matrix in welche die generierte Matrix hineinmultipliziert wird
     * @param vals der Vector in dem die Zielgröße sowie drei Koordinaten der Punkte im Quellbild stehen
     * @param targetImageData zusaetzliche Informationen fuer die spaetere Anwendung der Transfomrationen
     *
     */
    void makeAffineMatrixUpright(cv::Matx33f &transformationMatrix, const std::vector<float> &vals, bba::TargetImageData &targetImageData);

} // namespace bba