#!/bin/bash

BASEDIR=$(dirname $0)
PWD=$(pwd)
input=${@:1}
CLASSNAME=$(echo $input| cut -d '/' -f 2)
CLASSNAME=$(echo $CLASSNAME | cut -d '.' -f 1)
#echo $PWD ${@:1} $CLASSNAME

./NESL2C ${@:1} -sqc 

echo $CLASSNAME 
outputname="output/"$CLASSNAME"_sqc.c"
exename="bin/"$CLASSNAME
gcc $outputname -lm -o $exename
#exenameO3="bin/"$CLASSNAME"O3"
#gcc $outputname -lm -o $exenameO3 -O3
#echo $outputname
#echo $exename
valgrind ./$exename
#./$exenameO3
