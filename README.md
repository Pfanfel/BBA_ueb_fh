# Bildbearbeitung und Analyse Übung

Bei diesem Projekt handelt es sich um eine Übung, welche dazu dient grundlegende Funktionen in OpenCV selber zu implementieren.
Die einzelnen Ordner sind jeweils Zwischenstände, auf denen weiter aufgebaut wurde.
Das Ergebnis ist eine Kommandozeilenanwendung, welche Funktionen nach den Vorgaben der Aufgabenstellung bereitstellt.
Die Aufgeben wurden von Hermann Höhne erstellt.

---

## Aufgabe 0
Einstieg in die Programmierung von
bildorientierten Anwendungen in C++ mit OpenCV. Das Verarbeiten von
Kommandozeilenparametern sowie das Laden, Organisieren, Anzeigen,
Ausgeben und Speichern von Bildern steht im Mittelpunkt.
Das Datenformat muss der Definition der [Portable Anymap](https://de.wikipedia.org/wiki/Portable_Anymap) (PNM) in der menschenlesbaren ASCII-Variante genügen.

#### Aufrufbeispiele
    bba --input eingabe.pgm # Bild laden und vergessen
    bba --input eingabe.pgm --output - # Ausgabe auf Standardausgabe

    bba --input eingabe.pgm --identity --output - # den Identitäts-Filter auf ein Bild anwenden

    bba --input eingabe1.pgm --input eingabe2.pgm --output - --output - # Funktion des Stacks prüfen

    bba --input eingabe.pgm --identity --identity --output - # mehrere Filter hintereinander anwenden

    bba --input eingabe.pgm --output ausgabe.png # in anderem Dateiformat speichern
    bba --input eingabe1.pgm --input eingabe2.pgm --output ausgabe2.pgm --output ausgabe1.pgm # Funktion des Stacks prüfen

    bba --input eingabe.pgm --show # Bild laden, anzeigen und vergessen
    bba --input eingabe.pgm --show --output - # Bild laden, anzeigen und Ausgeben

---

## Aufgabe 1
In dieser Aufgabe geht es um pixelweise Operationen, Histogramm-Transformation und -analyse sowie Datentypen und ihre Wertebereiche.

*  In dieser Aufgabe werden nur Graustufenbilder gelesen und geschrieben.

 *  Der Wertebereich für die auf der Kommandozeile übergebenen Intensitätswerte der Filteroptionen sei [0.0, 1.0], wobei ein Intensitätswert von 0.0 einen schwarzen Pixel und ein Intensitätswert von 1.0 einen weißen Pixel beschreibt. Die nachfolgenden Funktionsdefinitionen erwarten ebenfalls diesen Wertebereich. Mehr zum Thema Wertebereiche ist im Abschnitt "Hinweise" weiter unten zu finden.  

 *  Filter `clamp`  
    Dieser Filter begrenzt alle Werte auf den Wertebereich [0, 1]. Viele Filter können Werte erzeugen, welche außerhalb des Wertebereichs liegen. Vor dem Speichern oder dem Anzeigen sollte daher der Filter `clamp` aufgerufen werden.  

    Beispielaufrufe in den folgenden Beispielen.

 *  Filter `brightness`  
    Dieser Filter manipuliert die Helligkeit des gesamten Bildes.  

    Beispielaufruf:  

        bba --input test.pgm --brightness 0.25 --clamp --output -
        bba --input test.pgm --brightness -0.25 --clamp --output -

 *  Filter `multiply`  
    Dieser Filter manipuliert die Intensitäten des gesamten Bildes anhand eines Faktors.  

    Beispielaufruf:  

        bba --input test.pgm --multiply 0.25 --output -

 *  Filter `linear`  
    Dieser Filter ist die Verallgemeinerung von `brightness` und `factor`. Er führt eine lineare Histogrammtransformation durch.  

    Beispielaufruf:  

        bba --input test.pgm --linear 1.0 0.125 --clamp --output -
        bba --input test.pgm --linear -1.0 1.0 --output -

    Wobei der erste Parameter die Steigung und der zweite Parameter den Achsenabschnitt angibt.

 *  Filter `contrast`  
    Dieser Filter ändert den Kontrast des gesamten Bildes anhand einer linearen Abbildung.  

    Beispielaufruf:  

        bba --input test.pgm --contrast 2.0 --clamp --output -
        bba --input test.pgm --contrast 0.5 --output -

 *  Filter `gamma`  
    Dieser Filter führt eine Gamma-Anpassung durch.  

    Beispielaufruf:  

        bba --input test.pgm --gamma 0.5 --output -
        bba --input test.pgm --gamma 2.0 --output -

 *  Filter `add`  
    Dieser Filter summiert zwei Bilder pixelweise.

    Beispielaufruf:  

        bba --input a.pgm --input b.pgm --add  --clamp --output -

 *  Filter `sub`  
    Dieser Filter subtrahiert zwei Bilder pixelweise.

    Beispielaufruf:  

        bba --input a.pgm --input b.pgm --sub --clamp --output -

    Dies führt pixelweise a-b aus.

 *  Filter `adiff`  
    Dieser Filter bestimmt pixelweise die absolute Differenz zweier Bilder.

    Beispielaufruf:  

        bba --input a.pgm --input b.pgm --adiff --clamp --output -

    Dies führt pixelweise |a-b| aus.

 *  Filter `normalize`  
    Dieser Filter führt eine "automatische Kontrastanpassung" des gesamten Bildes anhand einer linearen Abbildung durch.  
    Der kleinste im konkreten Bild vorkommende Wert wird auf 0 gesetzt, der größte im konkreten Bild vorkommende Wert wird auf 1 gesetzt. Die Werte dazwischen werden linear Abgebildet. Kann das Bild nicht normiert werden, weil es keinen Kontrast (nur einen konkreten sich gegebenenfalls wiederholenden Wert) hat, bleibt das Bild unverändert.

    Beispielaufruf:  

        bba --input a.pgm --input b.pgm --adiff --normalize --output -

 *  Filter `histogram`  
    Dieser Pseudo-Filter erstellt ein Histogramm. Der Filter ändert das Bild nicht, sondern gibt den Inhalt des Histogramms auf Standardausgabe aus. Das Format der Ausgabe sei eine komma-separierte Liste von Fließkommazahlen, eingeschlossen von rechteckigen Klammern. Die Anzahl von Nachkommastellen muss nicht festgelegt werden, es darf die Standardeinstellung beibehalten werden. Das Einfügen von Leerzeichen ist für bessere Lesbarkeit erlaubt.  
    Die Häufigkeiten im Histogramm seien gemäß der Bildgröße normiert. Besteht das Bild exklusiv aus Pixeln mit derselben Intensität, erreicht diese Intensität im Histogramm den Wert 1.0.  
    Es ist erlaubt, eine passende Funktion aus Open﻿CV zu verwenden.

    Beispielaufruf:

        bba --input test.pgm --histogram 2
        bba --input test.pgm --histogram 4
        bba --input test.pgm --histogram 8
        bba --input test.pgm --histogram 16
        bba --input test.pgm --histogram 256

    Der Parameter gibt die Anzahl der Klassen des Histogramms an. Die Klassen decken den gesamten Wertebereich ab und seien gleich breit.

    Beispiel eines Histogramms auf der Standardausgabe:

        [0.23, 0.27, 0.35, 0.15]

 *  Filter `sum-histogram`  
    Dieser Pseudo-Filter erstellt ein Summen-Histogramm. Der Filter ändert das Bild nicht, sondern gibt den Inhalt des Histogramms auf der Standardausgabe aus. Das Format entspreche dem des einfachen Histogramms.  
    Es ist erlaubt, eine passende Funktion aus Open﻿CV zu verwenden.

    Beispielaufrufe:

        bba --input test.pgm --sum-histogram 2
        bba --input test.pgm --sum-histogram 4
        bba --input test.pgm --sum-histogram 8
        bba --input test.pgm --sum-histogram 16
        bba --input test.pgm --sum-histogram 256

    Der Parameter gibt die Anzahl der Klassen des Histogramms an. Die Klassen seien gleich breit.

    Beispiel eines Histogramms auf der Standardausgabe:

        [0, 0.25, 0.5, 1]

    Zur einfacheren visuellen Kontrolle der Werte kann das Histogramm nach gnuplot geleitet werden.

        bba --input test.pgm --sum-histogram 256 \
        | tr -d [], | sed 's/[ ]\+/\n/g' | \
        gnuplot -p -e "plot '< cat -' using 0:1 with lines notitle"

 *  Filterketten  
    Es ist nicht zu vergessen, dass mehrere Filter hintereinander angefordert werden können:

    Beispielaufruf:  

        bba --input test.pgm --brightness 0.25 --histogram 4
        
 *  Filter `histogram-check`  
    Dieser Pseudo-Filter bewertet die Qualität eines Bildes anhand seines Histogramms. Das Urteil wird als Text auf der Standardausgabe ausgegeben. Die Bildüberschriften der Beispiele stellen die vier möglichen Urteile dar.

     *  Bild ist hell.  
     *  Bild ist dunkel.  
    Ist das Bild weder zu hell noch zu dunkel, wird der Kontrast bewertet.

     *  Kontrast ist niedrig.  
     *  Kontrast ist gut.  

    Das Urteil wird genau so auf die erste Zeile der Standardausgabe geschrieben, damit ein Zeichenkettenvergleich stattfinden kann. Detailiertere Informationen dürfen auf der Standardfehlerausgabe ausgegeben werden.

    Beispielaufruf:

        bba --input lena_grau.pgm --histogram-check

 *  Das Implementieren der Filter *ist* die Aufgabe. Die äquivalenten Funktionen `cv::Mat::convertTo` und `cv::convertScaleAbs` aus Open﻿CV sollen nicht zur Implementierung der eigentlichen Funktionalität benutzt werden. Sie dürfen aber als Referenz verwendet werden. Ebenso sollen die arithmetischen Operatoren von `cv::Mat` nicht verwendet werden.

 *  Filter `leavesdetect` (optional)  
    Bestimmt den Anteil der Bildfläche, welche von Blättern eingenommen wird. Dieser Filter gibt den Bedeckungsgrad (in Prozent als Ganzzahl) auf der Standardausgabe aus.
    
    Beispielaufruf:

        bba --input blätter.jpg --leavesdetect
        
    Beispielausgabe:

        15

---

## Aufgabe 2

In dieser Aufgabe geht es um morphologische Filter, lokale Filter und die dafür notwendige Randbehandlung.

 *  In dieser Aufgabe werden nur Graustufenbilder gelesen und geschrieben.

 *  Die Filter sollen manuell implementiert werden. Das Aufrufen der jeweiligen Impelementierung aus OpenCV ist nicht erlaubt.

 *  Bildeingabe 

     *  Filter `values`  
        Erstellt aus den vom Benutzer übergebenen Parametern ein Bild. Der erste Parameter gibt die Breite an, der zweite Parameter die Höhe.

            bba --input test.pgm --values 3 2 .1 .2 .3 4 5 6 --output -

        Der letzte Aufruf führt intern zu diesem Bild:

            0.1  0.2  0.3
              4    5    6
            
        Interessant in Kombination mit dem Filter `fold`.

 *  Randbehandlung `--edge <Option>`  
    Wenn lokale Filter am Rand des Bildes arbeiten, möchten sie Intensitäten von Pixeln lesen, welche außerhalb des Quellbildes liegen. Daher ist eine Randbehandlung notwendig. Diese Behandlung ist pixelweise algorithmisch zu lösen, sodass – theoretisch – auch dann passende Werte geliefert werden, wenn die Indizes gegen unendlich wachsen. `copyMakeBorder` darf nicht verwendet werden (das Implementieren ist die Aufgabe). Es ist nicht erlaubt, ein temporäres Bild zu erzeugen, welches größer als notwendig ist. Diese Methoden zur Randbehandlung sollen implementiert werden:

     *  `--edge null`  
        Rand wird als schwarz behandelt. Dies ist die Standardeinstellung für das Programm dieses Praktikums.

     *  `--edge <Wert>`  
        Rand wird als konstante Intensität des angegebenen Werts behandelt. 0.0 sei schwarz, 1.0 die volle Intensität (weiß).  
        *Für Hörer der Veranstaltung "Computergraphik": Dies ist wie `GL_CLAMP_TO_BORDER`.*

     *  `--edge replicate`  
        Rand hat die Intensität des nächstgelegenen gültigen Pixels.  
        *Für Hörer der Veranstaltung "Computergraphik": Dies ist wie `GL_CLAMP_TO_EDGE`.*

     *  `--edge reflect11`  
        Das Bild wird über die Kante hinweg gespiegelt.  
        *Für Hörer der Veranstaltung "Computergraphik": Dies ist wie `GL_MIRRORED_REPEAT`.*

     *  `--edge reflect101`  
        Das Bild wird über den am Rand liegenden Pixel hinweg gespiegelt. Der Sonderfall des 1-Pixel-Bildes darf ignoriert werden.

 *  Lineare lokale Filter

     *  Filter `fold`  
        Interpretiert das oberste Bild auf dem Stapel als Matrix. Wendet die Matrix als Kern eines lokalen Filters auf das zweite Bild an. Beide Bilder werden vom Stapel entfernt, das Ergebnisbild auf den Stapel gelegt. `fold` geht davon aus, dass die Matrix quadratisch ist und eine ungerade Anzahl an Pixeln auf jeder Seite hat, sodass die Mitte eindeutig bestimmt ist.
     
        Beispielaufruf:

            bba --input test.pgm --values 1 1 1 --fold --output - # Identität
            bba --input test.pgm --values 3 3 0 0 0 0 1 0 0 0 0 --fold --output - # Identität
            bba --input test.pgm --values 3 3 1 1 1 1 1 1 1 1 1 --factor .111111 --fold --output - # Mittelwert
            bba --input test.pgm --values 3 3 1 1 1 1 1 1 1 1 1 --factor .111111 --edge replicate --fold --output - # Mittelwert (Rand wird nicht dunkel)
            bba --input test.pgm --values 5 5  1 0 0 0 0  0 0 0 0 0  0 0 0 0 0  0 0 0 0 0  0 0 0 0 0 --fold --output - # diagonales Verschieben
            bba --input test.pgm --values 5 5  1 0 0 0 0  0 0 0 0 0  0 0 0 0 0  0 0 0 0 0  0 0 0 0 0 --edge 1 --fold --output - # diagonales Verschieben (Ecke wird weiß)

     *  Filter `box`  
        Der Box-Filter bestimmt den Mittelwert.

        Beispielaufruf:

            bba --input test.pgm --box --fold --output -

     *  Filter `gaussian`  
        Erzeugt ein Bild für den Gauß-Filter und legt ihn auf den Stapel. Der Gauß-Filter ist ein Weichzeichner.  

        Dieser Kern soll verwendet werden:

            1/16 2/16 1/16
            2/16 4/16 2/16
            1/16 2/16 1/16

        Beispielaufruf:

            bba --input test.pgm --gaussian --fold --output -

     *  Filter `sobel-x` und `sobel-y`  
        Erzeugt ein Bild für den jeweiligen Sobel-Filter und legt ihn auf den Stapel. Der Sobel Filter ist ein gerichteter Kantendetektor.

        Beispielaufruf:

            bba --input test.pgm --sobel-x --fold --clamp --output - # hebt horizontale Kontrastübergänge hervor
            bba --input test.pgm --sobel-y --fold --clamp --output - # hebt vertikale Kontrastübergänge hervor
            
     *  Filter `laplace`  
        Erzeugt ein Bild und legt es auf den Stapel. Der Laplace Filter ist ein bidirektionaler Kantendetektor.

            0  1  0
            1 -4  1
            0  1  0

        Beispielaufruf:

            bba --input test.pgm --laplace --fold --clamp --output -

 *  Rangordnungsfilter  
    Diese Filter benötigen keine Randbehandlung: Wo keine Pixel zu lesen sind, wird keiner ausgewählt.

     *  Filter `median`  
        Der Median Filter wählt den mittleren Wert aus. Hat das Bild eine gerade Anzahl an Bildpunkten, wird der kleinere Wert (Untermedian) ausgewählt (es sollen keine Intensitätswerte entstehen können, die nicht schon vorher im Bild waren).
        
        Beispielaufruf:

            bba --input test.pgm --median --output -

     *  Filter `erode`  
        Die Erosion wählt das einfache Minimum aus den neun (3 x 3 Pixel Quadrat) umliegenden Werten aus. Es sollen keine Additionen stattfinden.

        Beispielaufruf:

            bba --input test.pgm --erode --output -

     *  Filter `dilate`  
        Die Dilatation wählt das einfache Maximum aus den neun (3 x 3 Pixel Quadrat) umliegenden Werten aus. Es sollen keine Additionen stattfinden.

        Beispielaufruf:

            bba --input test.pgm --dilate --output -

 *  Filterketten  
    Es soll immer noch möglich sein, Filter beliebig zu verketten.

    Beispielaufruf:

        bba --input test.pgm --gaussian --laplace --fold --fold --clamp --output -

    Dies ist eine Variante des "Mexican Hat" bzw. "Marr-Hildreth" Filters.

 *  Bildanalyse  
    Graustufenbilder von exakt zwei Motiven:

     *  Wälder mit **vertikal** stehenden Bäumen.
     *  Treppen mit **horizontal** verlaufenden Stufen.
     
    Filter `discriminator`, welcher die Bilder unterscheidet. Zum Erstellen des Filters sind die in dieser und den vorigen Aufgaben erstellten Funktionen ausreichend. Weitere OpenCV Funktionen sollen daher nicht verwendet werden. Die Antwort soll als Zeichenkette `wald` oder `treppe` auf der Standardausgabe erfolgen, sodass ein Zeichenkettenvergleich möglich ist. Debugausgaben sollten daher auf der Standardfehlerausgabe gemacht werden.

    Beispielaufrufe:

        $ bba --input wald1.pgm --discriminator
        wald
        $ bba --input treppe1.pgm --discriminator
        treppe

---

## Aufgabe 3

Diese Aufgabe dreht sich um geometrische Bildtransformation.

In dieser Aufgabe sollen die affinen Transformationen mit Interpolation bei inverser Abbildung implementiert werden.
In OpenCV sind Datenstrukturen für Punkte und Matrizen bereits vorgegeben. Auch die Matrixmultiplikation und das Invertieren einer Matrix ist in OpenCV implementiert. Diese Funktionen bzw. Operatoren von OpenCV sollen verwendet werden.  
Das Implementieren der eigentlichen Transformation ist Teil dieser Aufgabe. Konsequenterweise dürfen `cv::warpAffine`, `cv::flip`, `cv::rotate`, `cv::resize`, `cv::transpose`, `cv::transform` und ihre Alternativen zur Lösung der Aufgabe nicht verwendet werden.  
Das bedeutet, dass Punkte und Matrizen manuell erzeugt und in homogene Form überführt werden müssen. Als Datentyp für die homogene Matrix bietet sich eine der streng typisierten Varianten von `cv::Matx` an.

 *  Die Größe des Zielbildes ist unmittelbar vor dem Durchführen der inversen Abbildung zur geometrischen Bildtransformation zu berechnen. Das transformierte Eingabebild sei in der Ausgabe vollständig und zentriert eingepasst zu sehen. Die Randbehandlung aus Aufgabe 2 muss bestehen bleiben.

 *  Filter `rotate`  
    Der Parameter gebe die Drehung in Grad im mathematisch negativen Drehsinn (umgangssprachlich "im Uhrzeigersinn") an.  
    Rotationen um (Vielfache von) 90 Grad müssen **exakt** (verlustfrei) sein.

    Beispielaufrufe:

        bba --input test.pgm --rotate    0 --output -
        bba --input test.pgm --rotate  -90 --output -
        bba --input test.pgm --rotate  180 --output -
        bba --input test.pgm --rotate  -10 --output -
        bba --input test.pgm --rotate   45 --output -
        bba --input test.pgm --rotate 0.25 --output -

 *  Filter `scale`  
    Die Parameter geben die Skalierung in x- und y-Richtung an.  
    Skalierungen um Ganzzahlige positive Werte sollen exakt sein. Bei einer Skalierung um 2x2 wird jeder Pixel durch die Wahl des nächsten Nachbarn auf genau vier Pixel abgebildet.

    Beispielaufrufe:

        bba --input test.pgm --scale 1.0 1.0 --output - # nichts passiert
        bba --input test.pgm --scale 2.0 2.0 --output - # vierfache Größe
        bba --input test.pgm --scale 2.0 1.0 --output - # doppelte Breite
        bba --input test.pgm --scale 1.0 0.5 --output - # halbe Höhe
        bba --input test.pgm --scale  -1  -1 --output - # Punktspiegelung

 *  Filter `shear`  
    Der Parameter gibt den Scherfaktor an.

    Beispielaufrufe:

        bba --input test.pgm --shear 0.0 --output - # nichts passiert
        bba --input test.pgm --shear 1.0 --output -

 *  Filter `transform`  
    Die Parameter repräsentieren die Elemente der Abbildungsmatrix.

    Beispielaufrufe:

        bba --input test.pgm --transform 1 0 0 1 --output - # nichts passiert
        bba --input test.pgm --transform 2 0 0 1 --output - # doppelte Breite
        bba --input test.pgm --transform 1 2 3 4 --output -
    
    Der letzte Aufruf führt intern zu dieser homogenen Abbildungsmatrix:

            1  2  0
            3  4  0
            0  0  1

 *  Filter `upright`  
    Berechnet mit Hilfe der Passpunktmethode eine Transformationsmatrix, welche einen angegebenen Bildbereich wieder Rechteckig abbildet. Der Filter nimmt als Parameter die Zielgröße sowie drei Koordinaten der Punkte links unten, links oben und rechts oben: `Breite Höhe x₁ y₁ x₂ y₂ x₃ y₃` Da der Benutzer die Koordinaten angibt, kann es sein, dass Bildteile abgeschnitten werden oder zusätzlicher Rand entsteht. Der vom Benutzer ausgewählte Ausschnitt soll in das Ergebnisbild eingepasst sein.

    Beispielaufrufe:

        bba --input test_256x256.pgm --upright 256 256  0 255  0 0  255 0 --output - # nichts passiert
        bba --input test.pgm --upright 360 512  66 430  292 48  672 317 --output -

    Für das Lösen des Gleichungssystems bietet sich eine Funktion in OpenCV an. Sie darf gesucht, gefunden und verwendet werden.

 *  Verkettung  
    Stehen mehrere affine Abbildungen (`rotate`, `scale`, `shear`, `transform`) hintereinander an, sollen ihre Abbildungsmatritzen kombiniert werden, sodass die eigentliche Arbeit auf dem Bild nur einmal stattfinden muss. `upright` darf, muss aber nicht in diesem Kontext beachtet werden. Diese Aufrufe sind äquivalent und liefern das gleiche Ergebnisbild: 

        bba --input test.pgm --scale 0.5 0.5 --scale 2.0 2.0 --rotate 45 --rotate -45 --output -
        bba --input test.pgm --scale 2.0 2.0 --rotate 45 --scale 0.5 0.5 --rotate -45 --output -
        bba --input test.pgm --rotate 45 --scale 2.0 2.0 --scale 0.5 0.5 --rotate -45 --output -

 *  Interpolationsmethoden  
    Hierfür wird ein neuer Parameter `--interpolation` zur Auswahl der Interpolationsmethode definiert. Die Auswahl wirkt global für alle Filter mit inverser Abbildung.

     *  Nächster Nachbar: `nearest`  
        Dies ist die Standardeinstellung, wenn keine Interpolationsmethode explizit angegeben wurde. Beim Implementieren der inversen Abbildung entsteht durch intuitive Programmierung im Normalfall automatisch eine Interpolation fehlender Bildpunkte durch Auswahl des nächsten Nachbarn.  
     *  Bilinear: `bilinear`  
        Bilineare Interpolation.  

    Beispielaufruf:

        # Vierfache Größe mit Nächstem Nachbarn als Interpolation
        bba --input test.pgm --scale 2 2 --output - # hat den gleichen Effekt wie der nächste Aufruf
        bba --input test.pgm --interpolation nearest --scale 2 2 --output -

        # Doppelte Höhe mit bilinearer Interpolation
        bba --input test.pgm --interpolation bilinear --scale 1 2 --output -

 *  Randbehandlung
     
    Die Randbehaldlung aus dem Basisteil von Aufgabe 2 hat auch in Aufgabe 3 bestand.
     
        bba --input test.pgm --edge replicate --interpolation bilinear --rotate 45 --output -

---

## Aufgabe 4

In dieser Aufgabe geht es um die bildweise Bearbeitung von Videos.

Für den Basisteil muss die Programmstruktur so angepasst werden, dass anstelle von Bildern auch Videos gelesen und geschrieben werden können.

Es ist überhaupt nicht sinnvoll und deswegen explizit nicht **nicht erlaubt**, das gesamte Video auf einmal in den Arbeitsspeicher zu laden. Dadurch wird die maximale Länge der zu verarbeitenden Videos unnötig durch die Größe des verfügbaren Arbeitsspeichers begrenzt.

Das Verwenden sämtlicher Funktionen aus OpenCV ist ohne Einschränkung erlaubt.

Es darf angenommen werden, dass alle Videos eine Bildrate von 25 verwenden.

 *  Videos

    Beispielaufrufe:

        bba --videoin test.avi --output firstframe.ppm # Speichert erstes Bild des Videos
        bba --videoin test.avi --output - # Gibt erstes Bild des Videos aus
        bba --videoin test.avi --framerate 25 --videoout out.avi # Schreibt Video unverändert
        bba --videoin test.avi --framerate 50 --videoout timelapse.avi # Erzeugt 2x Zeitraffer

 *  Filter `track`:  

    Der Filter verfolgt mittels Template Matching einen Bildbereich durch das Video und blendet einen Text ein. Der Effekt wurde in der BBC Serie "Sherlock" populär genutzt. Der Filter erwartet die Parameter `x y b r g b text`, wobei (x,y) einen Bildpunkt indiziert, b die Länge der Seite des quadratischen Musters, und (r,g,b) die Farbe angibt. Die Vorlage wird aus dem ersten Bilde des Videos extrahiert. Die Koordinate gibt dabei die Mitte an. Es wird erwartet, dass die Seitenlänge ungerade ist. `42 84 15 1 0 0 "Hier"` verfolgt den Bildbereich, welcher im ersten Bild um den Punkt (42,84) zu sehen ist. Der Text "Hier" wird rot in der Nähe eingezeichnet.


    Beispielaufruf:
    
        bba --videoin test.avi --track 42 84 15 1 0 0 "Hier" --videoout track.avi
        
*  Mit Szenenwechseln im Video muss nicht gerechnet werden.

 *  Stabilisierung durch iterative Anpassung der Vorlage  
    Im Verlaufe eines Videos bleibt die zu verfolgende Vorlage nicht perfekt gleich. Schleichende Veränderungen können dafür sorgen, dass das Verfolgen "entgleist". Eine behutsame Aktualisierung der Vorlage schafft Abhilfe.
    
 *  zeitliche Stabilisierung  
    Das Finden der Vorlage im Bild ist keine vollkommen exakte Wissenschaft. Geringfügige Abweichungen können entstehen. Wegen Bildrauschen können benachbarte Positionen durchaus von einem Videobild zum nächsten hin und her "flattern". Das sieht wenig hübsch aus und kann verbessert werden, indem die aktuelle Position nicht nur anhand des jüngsten Videobildes bestimmt wird. Es ist möglich, die Position im Video im Kontext des zeitlichen Verlaufes zu sehen und zwecks Stabilisierung zu verrechnen.

 *  Unterbereichsuche  
    Wird jedes Videobild komplett nach der Vorlage durchsucht, dauert das unnötig lang. Die Suche kann auf einen angemessen großen Unterbereich eingeschränkt werden.
    
 *  Feature durch Anklicken auswählen  
    Eine Variante des Filters `track-gui` nimmt die gleichen Parameter, verzichtet jedoch auf die Koordinaten. Stattdessen wird beim ersten Bild des Videos ein Fenster mittels `cv::imshow` geöffnet. Auf diesem Fenster soll der Benutzer 1 Punkt anklicken, der als Zentrum der Vorlage gelten soll. Mit einem Tastendruck wird die Eingabe bestätigt und der Filter läuft weiter.
    
    Beispielaufruf:
    
        bba --videoin test.avi --track-gui 15 1 0 0 "Hier" --videoout track.avi

 *  Alte Filter

    Beispielaufrufe:
        
        bba --videoin dark.avi --brightness 0.25 --videoout videoout.avi # Hellt Video auf
        bba --videoin noisy.avi --values 3 3 1 1 1 1 1 1 1 1 1 --factor .111111 --fold --videoout videoout.avi # Entfernt Rauschen
        bba --videoin vertical.avi --rotate -90 --videoout horizontal.avi # Dreht Video

