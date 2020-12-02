#!/bin/bash

OK="\t \033[38;2;0;255;0m OK \033[m"
<< TESTFAELLE

TESTFAELLE


#1.1
../build/bba --input ../tests/input/superSimple.pgm --brightness 10.0 --clamp --output ../tests/output/superSimpleBrightness.erg.pgm
diff -w <(grep -v '^#' ../tests/expected/superSimpleBrightness_10.0.pgm) <(grep -v '^#' ../tests/output/superSimpleBrightness.erg.pgm) && echo -e "Test 1.1 - brightness +10.0 clamp - ${OK}"
rm ../tests/output/superSimpleBrightness.erg.pgm

#1.2
../build/bba --input ../tests/input/superSimple.pgm --brightness -0.5  --output ../tests/output/superSimpleBrightness.erg.pgm
diff -w <(grep -v '^#' ../tests/expected/superSimpleBrightness_-0.5.pgm) <(grep -v '^#' ../tests/output/superSimpleBrightness.erg.pgm) && echo -e "Test 1.2 - brightness -0.5 - ${OK}"
rm ../tests/output/superSimpleBrightness.erg.pgm


#1.3
../build/bba --input ../tests/input/superSimple.pgm --brightness 0.5 --clamp --output ../tests/output/superSimpleBrightness.erg.pgm
diff -w <(grep -v '^#' ../tests/expected/superSimpleBrightness_0.5.pgm) <(grep -v '^#' ../tests/output/superSimpleBrightness.erg.pgm) && echo -e "Test 1.3 - brightness +0.5 clamp - ${OK}"
rm ../tests/output/superSimpleBrightness.erg.pgm


#2.1
../build/bba --input ../tests/input/superSimple.pgm --factor 0.25 --output ../tests/output/superSimpleFactor.erg.pgm
diff -w <(grep -v '^#' ../tests/expected/superSimpleFactor_0.25.pgm) <(grep -v '^#' ../tests/output/superSimpleFactor.erg.pgm) && echo -e "Test 2.1 - factor +0.25 - ${OK}"
rm ../tests/output/superSimpleFactor.erg.pgm

#3.1
../build/bba --input ../tests/input/superSimple.pgm --linear 1.0 0.125 --clamp --output ../tests/output/superSimpleLinear.erg.pgm
diff -w <(grep -v '^#' ../tests/expected/superSimpleLinear_1.0_0.125.pgm) <(grep -v '^#' ../tests/output/superSimpleLinear.erg.pgm) && echo -e "Test 3.1 - linear 1.0 0.125 clamp - ${OK}"
rm ../tests/output/superSimpleLinear.erg.pgm

#3.2
../build/bba --input ../tests/input/superSimple.pgm --linear -1.0 1.0 --output ../tests/output/superSimpleLinear.erg.pgm
diff -w <(grep -v '^#' ../tests/expected/superSimpleLinear_-1.0_1.0.pgm) <(grep -v '^#' ../tests/output/superSimpleLinear.erg.pgm) && echo -e "Test 3.2 - linear -1.0 1.0 - ${OK}"
rm ../tests/output/superSimpleLinear.erg.pgm

#4.1
../build/bba --input ../tests/input/superSimple.pgm --contrast 2.0 --clamp --output ../tests/output/superSimpleContrast.erg.pgm
diff -w <(grep -v '^#' ../tests/expected/superSimpleContrast_2.0.pgm) <(grep -v '^#' ../tests/output/superSimpleContrast.erg.pgm) && echo -e "Test 4.1 - contrast 2.0 clamp - ${OK}"
rm ../tests/output/superSimpleContrast.erg.pgm

#4.2
../build/bba --input ../tests/input/superSimple.pgm --contrast 0.5 --output ../tests/output/superSimpleContrast.erg.pgm
diff -w <(grep -v '^#' ../tests/expected/superSimpleContrast_0.5.pgm) <(grep -v '^#' ../tests/output/superSimpleContrast.erg.pgm) && echo -e "Test 4.2 - contrast 0.5 - ${OK}"
rm ../tests/output/superSimpleContrast.erg.pgm

#5.1
../build/bba --input ../tests/input/superSimple.pgm --gamma 0.5 --output ../tests/output/superSimpleGamma.erg.pgm
diff -w <(grep -v '^#' ../tests/expected/superSimpleGamma_0.5.pgm) <(grep -v '^#' ../tests/output/superSimpleGamma.erg.pgm) && echo -e "Test 5.1 - gamma 0.5 - ${OK}"
rm ../tests/output/superSimpleGamma.erg.pgm

#5.2
../build/bba --input ../tests/input/superSimple.pgm --gamma 2.0 --output ../tests/output/superSimpleGamma.erg.pgm
diff -w <(grep -v '^#' ../tests/expected/superSimpleGamma_2.0.pgm) <(grep -v '^#' ../tests/output/superSimpleGamma.erg.pgm) && echo -e "Test 5.2 - gamma 2.0 - ${OK}"
rm ../tests/output/superSimpleGamma.erg.pgm

#6.1
../build/bba --input ../tests/input/superSimpleHistogram.pgm --histogram 256 > ../tests/output/superSimpleHistogram.erg.txt
diff ../tests/expected/superSimpleHistogram_256.txt ../tests/output/superSimpleHistogram.erg.txt && echo -e "Test 6.1 - histogram 256 - ${OK}"
rm ../tests/output/superSimpleHistogram.erg.txt

#6.2
../build/bba --input ../tests/input/superSimple.pgm --histogram 4 > ../tests/output/superSimpleHistogram.erg.txt
diff ../tests/expected/superSimpleHistogram_4.txt ../tests/output/superSimpleHistogram.erg.txt && echo -e "Test 6.2 - histogram 4 - ${OK}"
rm ../tests/output/superSimpleHistogram.erg.txt

#6.3
../build/bba --input ../tests/input/hist100.pgm --histogram 256 > ../tests/output/hist100.erg.txt
diff ../tests/expected/hist_100.txt ../tests/output/hist100.erg.txt && echo -e "Test 6.3 - histogram 100th - ${OK}"
rm ../tests/output/hist100.erg.txt

#6.4
../build/bba --input ../tests/input/hist1000.pgm --histogram 256 > ../tests/output/hist1000.erg.txt
diff ../tests/expected/hist_1000.txt ../tests/output/hist1000.erg.txt && echo -e "Test 6.4 - histogram 1000th - ${OK}"
rm ../tests/output/hist1000.erg.txt

#7.1
../build/bba --input ../tests/input/superSimpleHistogram.pgm --sum-histogram 256 > ../tests/output/superSimpleSumHistogram.erg.txt
diff ../tests/expected/superSimpleSumHistogram_256.txt ../tests/output/superSimpleSumHistogram.erg.txt && echo -e "Test 7.1 - sum-histogram 256 - ${OK}"
rm ../tests/output/superSimpleSumHistogram.erg.txt

#8.1
../build/bba --input ../tests/input/only7.pgm --input ../tests/input/only10.pgm --add --output ../tests/output/only7+only10.erg.pgm
diff -w <(grep -v '^#' ../tests/expected/only17.pgm) <(grep -v '^#' ../tests/output/only7+only10.erg.pgm) && echo -e "Test 8.1 - add 7 + 10 - ${OK}"
rm ../tests/output/only7+only10.erg.pgm

#9.1
../build/bba --input ../tests/input/only7.pgm --input ../tests/input/only10.pgm --adiff --output ../tests/output/\|only7-only10\|.erg.pgm
diff -w <(grep -v '^#' ../tests/expected/only3.pgm) <(grep -v '^#' ../tests/output/\|only7-only10\|.erg.pgm) && echo -e "Test 9.1 - adiff |7 - 10| - ${OK}"
rm ../tests/output/\|only7-only10\|.erg.pgm

#10.1
../build/bba --input ../tests/input/only7.pgm --input ../tests/input/only10.pgm --sub --output ../tests/output/only7-only10.erg.pgm
diff -w <(grep -v '^#' ../tests/expected/only0.pgm) <(grep -v '^#' ../tests/output/only7-only10.erg.pgm) && echo -e "Test 10.1 - sub 7 - 10 - ${OK}"
rm ../tests/output/only7-only10.erg.pgm

#10.2
../build/bba --input ../tests/input/only10.pgm --input ../tests/input/only7.pgm --sub --output ../tests/output/only10-only7.erg.pgm
diff -w <(grep -v '^#' ../tests/expected/only3.pgm) <(grep -v '^#' ../tests/output/only10-only7.erg.pgm) && echo -e "Test 10.2 - sub 10 - 7 - ${OK}"
rm ../tests/output/only10-only7.erg.pgm

#10.3
../build/bba --input ../tests/input/only7.pgm --input ../tests/input/only10.pgm --sub --brightness 0.5 --output ../tests/output/only7-only10+127.erg.pgm
diff -w <(grep -v '^#' ../tests/expected/only124.pgm) <(grep -v '^#' ../tests/output/only7-only10+127.erg.pgm) && echo -e "Test 10.3 - sub 7 - 10 + 127 - ${OK}"
rm ../tests/output/only7-only10+127.erg.pgm

#11.1
../build/bba --input ../tests/input/notNormalized.pgm --normalize --output ../tests/output/normalized.erg.pgm
diff -w <(grep -v '^#' ../tests/expected/normalized.pgm) <(grep -v '^#' ../tests/output/normalized.erg.pgm) && echo -e "Test 11.1 - normalize - ${OK}"
rm ../tests/output/normalized.erg.pgm

#11.2
../build/bba --input ../tests/input/notNormalizeable.pgm --normalize --output ../tests/output/notNormalizeable.erg.pgm
diff -w <(grep -v '^#' ../tests/input/notNormalizeable.pgm) <(grep -v '^#' ../tests/output/notNormalizeable.erg.pgm) && echo -e "Test 11.2 - normalize not Normalizeable - ${OK}"
rm ../tests/output/notNormalizeable.erg.pgm

#12.1
../build/bba --input ../tests/input/lena_grau_hell.pgm --histogram-check 2>/dev/null | diff - <( echo "Bild ist hell.") && echo -e "Test 12.1 - histogram-check bright - ${OK}"

#12.2
../build/bba --input ../tests/input/lena_grau_dunkel.pgm --histogram-check 2>/dev/null | diff - <( echo "Bild ist dunkel.") && echo -e "Test 12.2 - histogram-check dark - ${OK}"

#12.3
../build/bba --input ../tests/input/lena_grau_wenigkontrast.pgm --histogram-check 2>/dev/null | diff - <( echo "Kontrast ist niedrig.") && echo -e "Test 12.3 - histogram-check low contrast - ${OK}"

#12.4
../build/bba --input ../tests/input/lena_grau_std.pgm --histogram-check 2>/dev/null | diff - <( echo "Kontrast ist gut.") && echo -e "Test 12.4 - histogram-check contrast ok - ${OK}"


#9.2
# commeted out, because opencv window would open and interrupt tests - uncomment to test
#./bba --input tests/input/color.ppm --show --output tests/output/color.erg.ppm 2>/dev/null
#diff -w <(grep -v '^#' tests/input/color.ppm) <(grep -v '^#' tests/output/color.erg.ppm) &&
#echo -e "Test 8.2 - show & store color file - ${OK}"
#rm tests/output/color.erg.ppm