#!/bin/bash

make all
./NESL2C test/qs_test.nesl -pfc
#gcc output/qs_test_pfc -o qs_test_pfc
./PartialFlatten/runPF.sh output/qs_test_pfc.c
nvcc -arch=sm_30 -ccbin=gcc-4.8 -I./PartialFlatten/include/ -o nvme qs_test_pfc.cu 
./nvme
echo "###ENDOFSEQDEMO###"

