#!/bin/bash

OK="\t \033[38;2;0;255;0m OK \033[m"
<< TESTFAELLE

TESTFAELLE


#1.1

../build/bba --input ../tests/input/simple_2x2.pgm --input ../tests/input/only0,25_5x5.pgm --edge reflect11 --fold --output ../tests/output/simple_2x2_reflect_11_5x5_0,25.erg.pgm
diff -w <(grep -v '^#' ../tests/expected/simple_2x2_reflect_11_5x5_0,25.pgm) <(grep -v '^#' ../tests/output/simple_2x2_reflect_11_5x5_0,25.erg.pgm) && echo -e "Test 1.1 edge reflect11 5x5 0,25/64 - ${OK}"
rm ../tests/output/simple_2x2_reflect_11_5x5_0,25.erg.pgm

#1.2
../build/bba --input ../tests/input/only10_3x3.pgm --box --edge reflect11 --fold --output ../tests/output/only10_3x3_reflect_11_3x3_1.erg.pgm
diff -w <(grep -v '^#' ../tests/expected/only10_3x3_reflect_11_box.pgm) <(grep -v '^#' ../tests/output/only10_3x3_reflect_11_3x3_1.erg.pgm) && echo -e "Test 1.2 edge reflect11 3x3 1 - ${OK}"
rm ../tests/output/only10_3x3_reflect_11_3x3_1.erg.pgm

#1.3
../build/bba --input ../tests/input/simple_3x3.pgm --box --edge 0 --fold --output ../tests/output/simple_3x3_reflect_null_3x3_1.erg.pgm
diff -w <(grep -v '^#' ../tests/expected/simple_3x3_reflect_null_box.pgm) <(grep -v '^#' ../tests/output/simple_3x3_reflect_null_3x3_1.erg.pgm) && echo -e "Test 1.3 edge reflect11 3x3 1 - ${OK}"
rm ../tests/output/simple_3x3_reflect_null_3x3_1.erg.pgm

#1.4
../build/bba --input ../tests/input/simple_1x1.pgm --input ../tests/input/only0,25_5x5.pgm --edge reflect11 --fold --output ../tests/output/simple_1x1_reflect_11_5x5_0,25.erg.pgm
diff -w <(grep -v '^#' ../tests/expected/simple_1x1_reflect_11_5x5_0,25.pgm) <(grep -v '^#' ../tests/output/simple_1x1_reflect_11_5x5_0,25.erg.pgm) && echo -e "Test 1.4 edge reflect11 1x1 0,25/64 - ${OK}"
rm ../tests/output/simple_1x1_reflect_11_5x5_0,25.erg.pgm

#1.5
../build/bba --input ../tests/input/simple_2x2.pgm --input ../tests/input/only10_3x3.pgm --edge reflect101 --fold --output ../tests/output/simple_2x2_reflect_101_3x3_10.erg.pgm
diff -w <(grep -v '^#' ../tests/expected/simple_2x2_reflect_101_3x3_10.pgm) <(grep -v '^#' ../tests/output/simple_2x2_reflect_101_3x3_10.erg.pgm) && echo -e "Test 1.5 edge reflect101 3x3 0,04/10 - ${OK}"
rm ../tests/output/simple_2x2_reflect_101_3x3_10.erg.pgm

#1.6
../build/bba --input ../tests/input/simple_2x2.pgm --sobel-x --edge reflect101 --fold --output ../tests/output/simple_2x2_reflect_101_sobel_x.erg.pgm
diff -w <(grep -v '^#' ../tests/expected/only0_2x2.pgm) <(grep -v '^#' ../tests/output/simple_2x2_reflect_101_sobel_x.erg.pgm) && echo -e "Test 1.6 edge reflect101 2x2 sobel-x - ${OK}"
rm ../tests/output/simple_2x2_reflect_101_sobel_x.erg.pgm

#2.0
../build/bba --input ../tests/input/simple_3x3.pgm --values 3 3 0 0 0 0 1 0 0 0 0 --sobel-x --fold --fold --clamp --output ../tests/output/simple_3x3_kommutativ_und_assoziativ.erg.pgm
diff -w <(grep -v '^#' ../tests/expected/simple_3x3_kommutativ_und_assoziativ.pgm) <(grep -v '^#' ../tests/output/simple_3x3_kommutativ_und_assoziativ.erg.pgm) && echo -e "Test 2.0 aus erg. zu assiz und kommut - ${OK}"
rm ../tests/output/simple_3x3_kommutativ_und_assoziativ.erg.pgm

#2.1
../build/bba --input ../tests/input/simple_3x3.pgm --sobel-x --values 3 3 0 0 0 0 1 0 0 0 0 --fold --fold --clamp --output ../tests/output/simple_3x3_kommutativ_und_assoziativ.erg.pgm
diff -w <(grep -v '^#' ../tests/expected/simple_3x3_kommutativ_und_assoziativ.pgm) <(grep -v '^#' ../tests/output/simple_3x3_kommutativ_und_assoziativ.erg.pgm) && echo -e "Test 2.1 aus erg. zu assiz und kommut - ${OK}"
rm ../tests/output/simple_3x3_kommutativ_und_assoziativ.erg.pgm

#2.2
../build/bba --input ../tests/input/simple_3x3.pgm --values 3 3 0 0 0 0 1 0 0 0 0 --fold --sobel-x --fold --clamp --output ../tests/output/simple_3x3_kommutativ_und_assoziativ.erg.pgm
diff -w <(grep -v '^#' ../tests/expected/simple_3x3_kommutativ_und_assoziativ.pgm) <(grep -v '^#' ../tests/output/simple_3x3_kommutativ_und_assoziativ.erg.pgm) && echo -e "Test 2.2 aus erg. zu assiz und kommut - ${OK}"
rm ../tests/output/simple_3x3_kommutativ_und_assoziativ.erg.pgm

#2.3
../build/bba --input ../tests/input/simple_3x3.pgm --sobel-x --fold --values 3 3 0 0 0 0 1 0 0 0 0 --fold --clamp --output ../tests/output/simple_3x3_kommutativ_und_assoziativ.erg.pgm
diff -w <(grep -v '^#' ../tests/expected/simple_3x3_kommutativ_und_assoziativ.pgm) <(grep -v '^#' ../tests/output/simple_3x3_kommutativ_und_assoziativ.erg.pgm) && echo -e "Test 2.3 aus erg. zu assiz und kommut - ${OK}"
rm ../tests/output/simple_3x3_kommutativ_und_assoziativ.erg.pgm

#3.0
../build/bba --input ../tests/input/median_3x3.pgm --median --output ../tests/output/median_3x3_median.erg.pgm
diff -w <(grep -v '^#' ../tests/expected/median_3x3_median.pgm) <(grep -v '^#' ../tests/output/median_3x3_median.erg.pgm) && echo -e "Test 3.0 median 3x3 - ${OK}"
rm ../tests/output/median_3x3_median.erg.pgm

#3.1
../build/bba --input ../tests/input/median_3x3.pgm --erode --output ../tests/output/median_3x3_erode.erg.pgm
diff -w <(grep -v '^#' ../tests/expected/median_3x3_erode.pgm) <(grep -v '^#' ../tests/output/median_3x3_erode.erg.pgm) && echo -e "Test 3.1 erode 3x3 - ${OK}"
rm ../tests/output/median_3x3_erode.erg.pgm

#3.2
../build/bba --input ../tests/input/median_3x3.pgm --dilate --output ../tests/output/median_3x3_dilate.erg.pgm
diff -w <(grep -v '^#' ../tests/expected/median_3x3_dilate.pgm) <(grep -v '^#' ../tests/output/median_3x3_dilate.erg.pgm) && echo -e "Test 3.2 dilate 3x3 - ${OK}"
rm ../tests/output/median_3x3_dilate.erg.pgm

#4.0
../build/bba --input ../tests/input/wald1.png --discriminator | diff - <( echo "wald") && echo -e "Test 4.0 - discriminator wald1 - ${OK}"


#4.1
../build/bba --input ../tests/input/wald2.png --discriminator | diff - <( echo "wald") && echo -e "Test 4.1 - discriminator wald2 - ${OK}"

#4.2
../build/bba --input ../tests/input/wald3.png --discriminator | diff - <( echo "wald") && echo -e "Test 4.2 - discriminator wald3 - ${OK}"

#4.3
../build/bba --input ../tests/input/treppe1.png --discriminator | diff - <( echo "treppe") && echo -e "Test 4.3 - discriminator treppe1 - ${OK}"

#4.4
../build/bba --input ../tests/input/treppe2.png --discriminator | diff - <( echo "treppe") && echo -e "Test 4.4 - discriminator treppe2 - ${OK}"

#4.5
../build/bba --input ../tests/input/treppe3.png --discriminator | diff - <( echo "treppe") && echo -e "Test 4.5 - discriminator treppe3 - ${OK}"

#5.0
../build/bba --input ../tests/input/simple_2x2_box.pgm --box --fold --output ../tests/output/box_2x2.erg.pgm
diff -w <(grep -v '^#' ../tests/expected/only10_2x2.pgm) <(grep -v '^#' ../tests/output/box_2x2.erg.pgm) && echo -e "Test 5.0 box 2x2 - ${OK}"
rm ../tests/output/box_2x2.erg.pgm

#6.0
../build/bba --values 5 5  0 0 0 0 0  0 0 0 0 0  0 0 1 0 0  0 0 0 0 0  0 0 0 0 0 --values 5 5  1 0 0 0 0  0 0 0 0 0  0 0 0 0 0  0 0 0 0 0  0 0 0 0 0 --edge 1 --fold --output ../tests/output/values_5x5.erg.pgm
diff -w <(grep -v '^#' ../tests/expected/values_5x5.pgm) <(grep -v '^#' ../tests/output/values_5x5.erg.pgm) && echo -e "Test 6.0 values 5x5 - ${OK}"
rm ../tests/output/values_5x5.erg.pgm

#7.0
../build/bba --values 3 1  0.25 0.5 0.75 --values 5 5 0 0 0 0 0 0 0 0 0 0 0 0 0 0 1 0 0 0 0 0 0 0 0 0 0 --edge 0.5 --fold --output ../tests/output/values_3x1_edge_0.5.erg.pgm
diff -w <(grep -v '^#' ../tests/expected/values_3x1.pgm) <(grep -v '^#' ../tests/output/values_3x1_edge_0.5.erg.pgm) && echo -e "Test 7.0 values 3x1 - ${OK}"
rm ../tests/output/values_3x1_edge_0.5.erg.pgm

#8
../build/bba --input ../tests/input/simple_2x2.pgm --box --edge replicate --fold --output ../tests/output/edge_replicate_box.erg.pgm
diff -w <(grep -v '^#' ../tests/expected/simple_2x2_replicate_box.pgm) <(grep -v '^#' ../tests/output/edge_replicate_box.erg.pgm) && echo -e "Test 8.0 replicate - ${OK}"
rm ../tests/output/edge_replicate_box.erg.pgm

#9
../build/bba --input ../tests/input/simple_3x3.pgm --box --edge replicate --fold --output ../tests/output/simple_3x3_replicate_box.erg.pgm
diff -w <(grep -v '^#' ../tests/expected/simple_3x3_replicate_box.pgm) <(grep -v '^#' ../tests/output/simple_3x3_replicate_box.erg.pgm) && echo -e "Test 9.0 edge replicate 3x3 - ${OK}"
rm ../tests/output/simple_3x3_replicate_box.erg.pgm