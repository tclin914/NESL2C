#!/bin/bash

#make all
#./NESL2C test/qs_test.nesl -pfc
#gcc output/qs_test_pfc -o qs_test_pfc
./PartialFlatten/runPF.sh output/qs_translate_1000000fix.c
echo "###PFtranslatedone###"
nvcc -arch=sm_30 -ccbin=gcc-4.8 -I./PartialFlatten/include/ -o qstranslate1000000fix qs_translate_1000000fix.cu 
echo "###start executing###"
./qstranslate1000000fix 1000000
echo "###ENDOFSEQDEMO###"

