#!/bin/bash

OK="\t \033[38;2;0;255;0m OK \033[m"
<< TESTFAELLE
    1 bba --input eingabe.pgm                                                                     # Bild laden und vergessen - check              done
  2.1 bba --input eingabe.pgm --output -                                                          # Ausgabe auf Standardausgabe                   done
  2.2 bba --input eingabe.pgm --output ausgabe.pgm                                                # Ausgabe in Datei grau                         done      
  2.2 bba --input eingabe.ppm --output ausgabe.ppm                                                # Ausgabe in Datei farbig                       done                                                                                              
  3.1 bba --input eingabe.pgm --identity --output -                                               # den Identitäts-Filter auf ein Bild anwenden   done
  3.2 bba --input eingabe.pgm --identity --output ausgabe.pgm                                     # den Identitäts-Filter auf ein Bild anwenden   done
  4.1 bba --input eingabe1.pgm --input eingabe2.pgm --output - --output -                         # Funktion des Stacks prüfen                    done
  4.2 bba --input eingabe1.pgm --input eingabe2.pgm --output ausgabe1.pgm --output ausgabe2.pgm   # Funktion des Stacks prüfen                    done
  5.1 bba --input eingabe.pgm --identity --identity --output -                                    # mehrere Filter hintereinander anwenden        done
  5.2 bba --input eingabe.pgm --identity --identity --output ausgabe.pgm                          # mehrere Filter hintereinander anwenden        done
    6 bba --input eingabe.pgm --output ausgabe.png                                                # in anderem Dateiformat speichern              done
  7.1 bba --input eingabe.pgm --show                                                              # Graues Bild laden, anzeigen und vergessen     done
  7.2 bba --input eingabe.ppm --show                                                              # Farbiges Bild laden, anzeigen und vergessen   done
  8.1 bba --input eingabe.pgm --show --output -                                                   # graues Bild anzeigen und speichern in stdout  done
  8.2 bba --input eingabe.pgm --show --output ausgabe.pgm                                         # graues Bild anzeigen und speichern in file    done
  9.1 bba --input eingabe.ppm --show --output -                                                   # farbiges Bild anziegen und speichern in stdoutdone
  9.2 bba --input eingabe.ppm --show --output ausgabe.ppm                                         # farbiges Bild anzeigen und speichern in file  done
TESTFAELLE
#1
{ ./bba --input ; } 2>/dev/null || echo -e "Test 1 - laod image and forget - ${OK}"

#2.1
./bba --input tests/input/simple.pgm --output - 2>/dev/null >> tests/output/stdout.pgm
diff -w <(grep -v '^#' tests/input/simple.pgm) <(grep -v '^#' tests/output/stdout.pgm) && echo -e "Test 2.1 - output to std::cout - ${OK}"
rm tests/output/stdout.pgm

#2.2
./bba --input tests/input/simple.pgm --output tests/output/simple.erg.pgm 2>/dev/null
diff -w <(grep -v '^#' tests/input/simple.pgm) <(grep -v '^#' tests/output/simple.erg.pgm) && echo -e "Test 2.2 - output in file grey - ${OK}"
rm tests/output/simple.erg.pgm

#2.3
./bba --input tests/input/color.ppm --output tests/output/color.erg.ppm 2>/dev/null
diff -w <(grep -v '^#' tests/input/color.ppm) <(grep -v '^#' tests/output/color.erg.ppm) && echo -e "Test 2.3 - output in file color - ${OK}"
rm tests/output/color.erg.ppm


#3.1
./bba --input tests/input/simple.pgm --identity --output - 2>/dev/null >> tests/output/stdout.pgm
diff -w <(grep -v '^#' tests/input/simple.pgm) <(grep -v '^#' tests/output/stdout.pgm) &&
echo -e "Test 3.1 - identity filter on image - ${OK}"
rm tests/output/stdout.pgm

#3.2
./bba --input tests/input/simple.pgm --identity --output tests/output/simple.erg.pgm 2>/dev/null
diff -w <(grep -v '^#' tests/input/simple.pgm) <(grep -v '^#' tests/output/simple.erg.pgm) &&
echo -e "Test 3.2 - identity filter on image - ${OK}"
rm tests/output/simple.erg.pgm

#4.1
./bba --input tests/input/simple.pgm --input tests/input/second.pgm --output - >> tests/output/stdout1.pgm 2>/dev/null
diff -w  <(grep -v '^#' tests/input/second.pgm) <(grep -v '^#' tests/output/stdout1.pgm) &&
echo -e "Test 4.1 - check stack functionality - ${OK}"
rm tests/output/stdout1.pgm

#4.2
./bba --input tests/input/simple.pgm --input tests/input/second.pgm --output tests/output/ausgabe1.pgm --output tests/output/ausgabe2.pgm 2>/dev/null
diff -w  <(grep -v '^#' tests/input/simple.pgm) <(grep -v '^#' tests/output/ausgabe2.pgm) &&
diff -w  <(grep -v '^#' tests/input/second.pgm) <(grep -v '^#' tests/output/ausgabe1.pgm) &&
echo -e "Test 4.2 - check stack functionality - ${OK}"
rm tests/output/ausgabe1.pgm
rm tests/output/ausgabe2.pgm

#5.1
./bba --input tests/input/simple.pgm --identity --identity --output - >> tests/output/stdout.pgm 2>/dev/null
diff -w <(grep -v '^#' tests/input/simple.pgm) <(grep -v '^#' tests/output/stdout.pgm) && echo -e "Test 5.2 - multiple identity filters - ${OK}"
rm tests/output/stdout.pgm
 
#5.2
./bba --input tests/input/simple.pgm --identity --identity --output tests/output/simple.erg.pgm 2>/dev/null
diff -w <(grep -v '^#' tests/input/simple.pgm) <(grep -v '^#' tests/output/simple.erg.pgm) && echo -e "Test 5.2 - multiple identity filters - ${OK}"
rm tests/output/simple.erg.pgm

#6
./bba --input tests/input/simple.pgm --output tests/output/ausgabe_in_png.png 2>/dev/null
./bba --input tests/output/ausgabe_in_png.png --output tests/output/ausgabe_in_pgm.pgm 2>/dev/null
diff -w <(grep -v '^#' tests/input/simple.pgm) <(grep -v '^#' tests/output/ausgabe_in_pgm.pgm) && echo -e "Test 6 - different file format - ${OK}"
rm tests/output/ausgabe_in_png.png
rm tests/output/ausgabe_in_pgm.pgm

#7.1
#commeted out, because opencv window would open and interrupt tests - uncomment to test
./bba --input tests/input/bigGrey.pgm --show && echo -e "Test 7.1 - load  grey image and show - ${OK}"

#7.2
#commeted out, because opencv window would open and interrupt tests - uncomment to test
./bba --input tests/input/color.ppm --show && echo -e "Test 7.2 - load color image and show - ${OK}"


#8.1
#commeted out, because opencv window would open and interrupt tests - uncomment to test
./bba --input tests/input/bigGrey.pgm --show --output - >> tests/output/stdout.pgm 2>/dev/null
diff -w <(grep -v '^#' tests/input/bigGrey.pgm) <(grep -v '^#' tests/output/stdout.pgm) &&
echo -e "Test 8.1 - show & store grey stdout - ${OK}"
rm tests/output/stdout.pgm

#8.2
# commeted out, because opencv window would open and interrupt tests - uncomment to test
./bba --input tests/input/bigGrey.pgm --show --output tests/output/bigGrey.erg.pgm 2>/dev/null
diff -w <(grep -v '^#' tests/input/bigGrey.pgm) <(grep -v '^#' tests/output/bigGrey.erg.pgm) &&
echo -e "Test 8.2 - show & store grey file - ${OK}"
rm tests/output/bigGrey.erg.pgm

#9.1
#commeted out, because opencv window would open and interrupt tests - uncomment to test
./bba --input tests/input/color.ppm --show --output - >> tests/output/stdout.ppm 2>/dev/null
diff -w <(grep -v '^#' tests/input/color.ppm) <(grep -v '^#' tests/output/stdout.ppm) &&
echo -e "Test 9.1 - show & store color stdout - ${OK}"
rm tests/output/stdout.ppm

#9.2
# commeted out, because opencv window would open and interrupt tests - uncomment to test
./bba --input tests/input/color.ppm --show --output tests/output/color.erg.ppm 2>/dev/null
diff -w <(grep -v '^#' tests/input/color.ppm) <(grep -v '^#' tests/output/color.erg.ppm) &&
echo -e "Test 8.2 - show & store color file - ${OK}"
rm tests/output/color.erg.ppm