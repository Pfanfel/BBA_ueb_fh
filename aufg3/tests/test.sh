#!/bin/bash

OK="\t \033[38;2;0;255;0m OK \033[m"

#1.0
#scale
<<SCALE
bba --input test.pgm --scale 1.0 1.0 --output - # nichts passiert
bba --input test.pgm --scale 2.0 2.0 --output - # vierfache Größe
bba --input test.pgm --scale 2.0 1.0 --output - # doppelte Breite
bba --input test.pgm --scale 1.0 0.5 --output - # halbe Höhe
bba --input test.pgm --scale  -1  -1 --output - # Punktspiegelung
SCALE
../build/bba --input ../tests/input/simple_3x5.pgm --interpolation bilinear --scale 1 1  --output ../tests/output/simple_3x5_scale11.erg.pgm
diff -w <(grep -v '^#' ../tests/expected/simple_3x5_identity.pgm) <(grep -v '^#' ../tests/output/simple_3x5_scale11.erg.pgm) && echo -e "Test scale 1 1 - ${OK}"
rm ../tests/output/simple_3x5_scale11.erg.pgm

../build/bba --input ../tests/input/simple_3x3.pgm --scale 2 2  --output ../tests/output/simple_3x3_scale22_nearest.erg.pgm
diff -w <(grep -v '^#' ../tests/expected/simple_3x3_scale22_nearest.pgm) <(grep -v '^#' ../tests/output/simple_3x3_scale22_nearest.erg.pgm) && echo -e "Test scale 2 2 - ${OK}"
rm ../tests/output/simple_3x3_scale22_nearest.erg.pgm

#rotate
<< ROTATE
bba --input test.pgm --rotate    0 --output -
bba --input test.pgm --rotate  -90 --output -
bba --input test.pgm --rotate  180 --output -
bba --input test.pgm --rotate  -10 --output -
bba --input test.pgm --rotate   45 --output -
bba --input test.pgm --rotate 0.25 --output -

ROTATE

../build/bba --input ../tests/input/simple_3x5.pgm --rotate 0 --output ../tests/output/simple_3x5_rotate_0.erg.pgm
diff -w <(grep -v '^#' ../tests/expected/simple_3x5_identity.pgm) <(grep -v '^#' ../tests/output/simple_3x5_rotate_0.erg.pgm) && echo -e "Test rotate 0 - ${OK}"
rm ../tests/output/simple_3x5_rotate_0.erg.pgm

../build/bba --input ../tests/input/simple_3x5.pgm --rotate -90 --output ../tests/output/simple_3x5_rotate_-90.erg.pgm
diff -w <(grep -v '^#' ../tests/expected/simple_3x5_rotate_-90.pgm) <(grep -v '^#' ../tests/output/simple_3x5_rotate_-90.erg.pgm) && echo -e "Test rotate -90 - ${OK}"
rm ../tests/output/simple_3x5_rotate_-90.erg.pgm

../build/bba --input ../tests/input/simple_3x5.pgm --rotate 180 --output ../tests/output/simple_3x5_rotate_180.erg.pgm
diff -w <(grep -v '^#' ../tests/expected/simple_3x5_rotate_180.pgm) <(grep -v '^#' ../tests/output/simple_3x5_rotate_180.erg.pgm) && echo -e "Test rotate 180 - ${OK}"
rm ../tests/output/simple_3x5_rotate_180.erg.pgm

#../build/bba --input ../tests/input/simple_3x5.pgm --rotate -10 --output ../tests/output/simple_3x5_rotate_-10.erg.pgm
#diff -w <(grep -v '^#' ../tests/expected/simple_3x5_rotate_-10.pgm) <(grep -v '^#' ../tests/output/simple_3x5_rotate_-10.erg.pgm) && echo -e "Test rotate -10 - ${OK}"
#rm ../tests/output/simple_3x5_rotate_-10.erg.pgm

../build/bba --input ../tests/input/simple_3x5.pgm --rotate 45 --output ../tests/output/simple_3x5_rotate_45.erg.pgm
diff -w <(grep -v '^#' ../tests/expected/simple_3x5_rotate_45.pgm) <(grep -v '^#' ../tests/output/simple_3x5_rotate_45.erg.pgm) && echo -e "Test rotate 45 - ${OK}"
rm ../tests/output/simple_3x5_rotate_45.erg.pgm

#../build/bba --input ../tests/input/simple_3x5.pgm --rotate 0.25 --output ../tests/output/simple_3x5_rotate0.erg.pgm
#diff -w <(grep -v '^#' ../tests/expected/simple_3x5.pgm) <(grep -v '^#' ../tests/output/simple_3x5_rotate0.erg.pgm) && echo -e "Test rotate 0.25 - ${OK}"
#rm ../tests/output/simple_3x5_rotate0.erg.pgm

#translate
../build/bba --input ../tests/input/simple_3x5.pgm --translate 0 0 --output ../tests/output/simple_3x5_translate_0_0.erg.pgm
diff -w <(grep -v '^#' ../tests/expected/simple_3x5_identity.pgm) <(grep -v '^#' ../tests/output/simple_3x5_translate_0_0.erg.pgm) && echo -e "Test translate 0 0 - ${OK}"
rm ../tests/output/simple_3x5_translate_0_0.erg.pgm

../build/bba --input ../tests/input/simple_3x5.pgm --translate 1 0 --output ../tests/output/simple_3x5_translate_1_0.erg.pgm
diff -w <(grep -v '^#' ../tests/expected/simple_3x5_translate_1_0.pgm) <(grep -v '^#' ../tests/output/simple_3x5_translate_1_0.erg.pgm) && echo -e "Test translate 1 0 - ${OK}"
rm ../tests/output/simple_3x5_translate_1_0.erg.pgm

../build/bba --input ../tests/input/simple_3x5.pgm --translate 0 1 --output ../tests/output/simple_3x5_translate_0_1.erg.pgm
diff -w <(grep -v '^#' ../tests/expected/simple_3x5_translate_0_1.pgm) <(grep -v '^#' ../tests/output/simple_3x5_translate_0_1.erg.pgm) && echo -e "Test translate 0 1 - ${OK}"
rm ../tests/output/simple_3x5_translate_0_1.erg.pgm

../build/bba --input ../tests/input/simple_3x5.pgm --translate 1 1 --output ../tests/output/simple_3x5_translate_1_1.erg.pgm
diff -w <(grep -v '^#' ../tests/expected/simple_3x5_translate_1_1.pgm) <(grep -v '^#' ../tests/output/simple_3x5_translate_1_1.erg.pgm) && echo -e "Test translate 1 1 - ${OK}"
rm ../tests/output/simple_3x5_translate_1_1.erg.pgm

#interpolation bilinear
../build/bba --input ../tests/input/simple_2x2.pgm --edge replicate --interpolation bilinear --scale 2.0 2.0 --output ../tests/output/interpolation.erg.pgm
diff -w <(grep -v '^#' ../tests/expected/interpolation.pgm) <(grep -v '^#' ../tests/output/interpolation.erg.pgm) && echo -e "Test interpolation bilinear - ${OK}"
rm ../tests/output/interpolation.erg.pgm

#shear

#transform