#!/bin/bash

make all
./NESL2C test/qs_test.nesl -sqc
gcc output/qs_test_sqc.c -o qs_test_sqc -g
./qs_test_sqc


#./PartialFlatten/runPF.sh output/qs_test_sqc.c
#nvcc -arch=sm_30 -ccbin=gcc-4.8 -I./PartialFlatten/include/ -o nvme qs_test_sqc.cu 

