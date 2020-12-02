#authors: Janik und Len
#!/bin/bash
echo "Building tests...."
make
status=$?
if [ "$status" != "0" ]
then
	echo "Build error status code: $status";
	exit $status
fi

echo
echo "Testing..."

#lib_test stellt Methoden zum einfachen auswerten eines Befehls bereit.
#--> fh_test_run [Anweisung]:                                       
#    Prüft ob ein Anweisung ausführbar ist
#
#--> fh_test_run_diff [Anweisung] [Ergebnisbild] [Erwartungsbild]: 
#    Vergleicht ein Ergebnisbild mit einem Erwartungsbild
#
#--> fh_test_run_equals [Anweisung] [Vergleichs-String]:                                            
#    Vergleicht ein gelieferten String aus einem Aufruf mit einem Erwartungsstring
. ./lib_tests.sh

#Ordnerstruktur der Ergebnis -/und Erwartungsbilder
file="./pic/"

echo
echo -e "\033[34mTesting basic intput/output implementation:\033[0m"
fh_test_run "./bba --input ${file}test.pgm"
fh_test_run_diff "./bba --input ${file}test.pgm --output ${file}res.pgm" "${file}res.pgm" "${file}test.pgm"
rm ${file}res.pgm

echo
echo -e "\033[34mTesting values implementation:\033[0m"
fh_test_run_diff "./bba --values 3 3 .1 .2 .3 .4 .5 .6 .7 .8 .9 --output ${file}res.pgm" "${file}res.pgm" "${file}test.pgm"
fh_test_run_diff "./bba --values 1 1 0 --output ${file}res.pgm" "${file}res.pgm" "${file}valTest1.pgm"
fh_test_run_diff "./bba --values 3 2 .1 .5 1.0 -1 0 2 --output ${file}res.pgm" "${file}res.pgm" "${file}valTest2.pgm"
rm ${file}res.pgm

echo
echo -e "\033[34mTesting fold/edge implementation:\033[0m"
fh_test_run_diff "./bba --input ${file}test.pgm --values 1 1 1 --fold --output ${file}res.pgm" "${file}res.pgm" "${file}foldTest1.pgm"
fh_test_run_diff "./bba --input ${file}test.pgm --values 3 3 0 0 0 0 1 0 0 0 0 --fold --output ${file}res.pgm" "${file}res.pgm" "${file}foldTest2.pgm"
fh_test_run_diff "./bba --input ${file}test.pgm --values 3 3 1 1 1 1 1 1 1 1 1 --factor .111111 --fold --output ${file}res.pgm" "${file}res.pgm" "${file}foldTest3.pgm"
fh_test_run_diff "./bba --input ${file}test.pgm --values 3 3 1 1 1 1 1 1 1 1 1 --factor .111111 --edge replicate --fold --output ${file}res.pgm" "${file}res.pgm" "${file}foldTest4.pgm"
fh_test_run_diff "./bba --input ${file}test.pgm --values 5 5  1 0 0 0 0  0 0 0 0 0  0 0 0 0 0  0 0 0 0 0  0 0 0 0 0 --fold --output ${file}res.pgm" "${file}res.pgm" "${file}foldTest5.pgm"
fh_test_run_diff "./bba --input ${file}test.pgm --values 5 5  1 0 0 0 0  0 0 0 0 0  0 0 0 0 0  0 0 0 0 0  0 0 0 0 0 --edge 1 --fold --output ${file}res.pgm" "${file}res.pgm" "${file}foldTest6.pgm"
fh_test_run_diff "./bba --input ${file}test.pgm --values 5 5  1 0 0 0 0  0 0 0 0 0  0 0 0 0 0  0 0 0 0 0  0 0 0 0 0 --edge reflect101 --fold --output ${file}res.pgm" "${file}res.pgm" "${file}foldTest7.pgm"
rm ${file}res.pgm

echo
echo -e "\033[34mTesting box implementation:\033[0m"
fh_test_run_diff "./bba --box --output ${file}res.pgm" "${file}res.pgm" "${file}box.pgm"
fh_test_run_diff "./bba --input ${file}test.pgm --box --edge reflect11 --fold --output ${file}res.pgm" "${file}res.pgm" "${file}boxTest1.pgm"
rm ${file}res.pgm

echo
echo -e "\033[34mTesting gaussian implementation:\033[0m"
fh_test_run_diff "./bba --gaussian --output ${file}res.pgm" "${file}res.pgm" "${file}gaussian.pgm"
fh_test_run_diff "./bba --input ${file}test.pgm --gaussian --fold --clamp --output ${file}res.pgm" "${file}res.pgm" "${file}gaussianTest1.pgm"
rm ${file}res.pgm

echo
echo -e "\033[34mTesting laplace implementation:\033[0m"
fh_test_run_diff "./bba --laplace --output ${file}res.pgm" "${file}res.pgm" "${file}laplace.pgm"
fh_test_run_diff "./bba --input ${file}test.pgm --laplace --fold --clamp --output ${file}res.pgm" "${file}res.pgm" "${file}laplaceTest1.pgm"
rm ${file}res.pgm

echo
echo -e "\033[34mTesting sobel implementation:\033[0m"
fh_test_run_diff "./bba --sobel-x --output ${file}res.pgm" "${file}res.pgm" "${file}sobel-x.pgm"
fh_test_run_diff "./bba --input ${file}test.pgm --values 3 3 0 0 0 0 1 0 0 0 0 --sobel-x --fold --fold --clamp --output ${file}res.pgm" "${file}res.pgm" "${file}kettenPic.pgm"
fh_test_run_diff "./bba --input ${file}test.pgm --sobel-x --values 3 3 0 0 0 0 1 0 0 0 0 --fold --fold --clamp --output ${file}res.pgm" "${file}res.pgm" "${file}kettenPic.pgm"
fh_test_run_diff "./bba --input ${file}test.pgm --values 3 3 0 0 0 0 1 0 0 0 0 --fold --sobel-x --fold --clamp --output ${file}res.pgm" "${file}res.pgm" "${file}kettenPic.pgm"
fh_test_run_diff "./bba --input ${file}test.pgm --sobel-x --fold --values 3 3 0 0 0 0 1 0 0 0 0 --fold --clamp --output ${file}res.pgm" "${file}res.pgm" "${file}kettenPic.pgm"
fh_test_run_diff "./bba --sobel-y --output ${file}res.pgm" "${file}res.pgm" "${file}sobel-y.pgm"
rm ${file}res.pgm

echo
echo -e "\033[34mTesting median implementation:\033[0m"
fh_test_run_diff "./bba --input ${file}test.pgm --median --clamp --output ${file}res.pgm" "${file}res.pgm" "${file}medianTest1.pgm"
fh_test_run_diff "./bba --input ${file}test.pgm --box --fold --median --clamp --output ${file}res.pgm" "${file}res.pgm" "${file}medianTest2.pgm"
rm ${file}res.pgm

echo
echo -e "\033[34mTesting erode implementation:\033[0m"
fh_test_run_diff "./bba --input ${file}test.pgm --erode --output ${file}res.pgm" "${file}res.pgm" "${file}erodeTest1.pgm"
fh_test_run_diff "./bba --input ${file}test.pgm --box --fold --erode --clamp --output ${file}res.pgm" "${file}res.pgm" "${file}erodeTest2.pgm"
rm ${file}res.pgm

echo
echo -e "\033[34mTesting dilate implementation:\033[0m"
fh_test_run_diff "./bba --input ${file}test.pgm --dilate --output ${file}res.pgm" "${file}res.pgm" "${file}dilateTest1.pgm"
fh_test_run_diff "./bba --input ${file}test.pgm --box --fold --dilate --clamp --output ${file}res.pgm" "${file}res.pgm" "${file}dilateTest2.pgm"
rm ${file}res.pgm

echo
echo -e "\033[34mTesting discriminator implementation:\033[0m"
fh_test_run_equals "./bba --input ${file}wald1.pgm --discriminator --output ${file}res.pgm" "wald"
fh_test_run_equals "./bba --input ${file}wald2.pgm --discriminator --output ${file}res.pgm" "wald"
fh_test_run_equals "./bba --input ${file}wald3.pgm --discriminator --output ${file}res.pgm" "wald"
fh_test_run_equals "./bba --input ${file}treppe1.pgm --discriminator --output ${file}res.pgm" "treppe"
fh_test_run_equals "./bba --input ${file}treppe2.pgm --discriminator --output ${file}res.pgm" "treppe"
fh_test_run_equals "./bba --input ${file}treppe3.pgm --discriminator --output ${file}res.pgm" "treppe"
rm ${file}res.pgm
