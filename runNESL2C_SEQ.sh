#!/bin/bash

BASEDIR=$(dirname $0)
PWD=$(pwd)
input=${@:1}
CLASSNAME=$(echo $input| cut -d '/' -f 2)
#echo $PWD ${@:1} $CLASSNAME

./NESL2C ${@:1} --sqc 

echo [runNESL2C] class name $CLASSNAME 
outputname="output/"$CLASSNAME".sqc.c"
exename="bin/"$CLASSNAME

echo [runNESL2C] outputname $outputname 
gcc $outputname -lm -o $exename

echo [runNESL2C] exename $exename 
./$exename
