#!/bin/bash

BASEDIR=$(dirname $0)
PWD=$(pwd)
input=${@:1}
CLASSNAME=$(echo $input| cut -d '/' -f 2)
outputname="output/"$CLASSNAME"_sqc.c"
exename="bin/"$CLASSNAME
#echo $PWD ${@:1} $CLASSNAME


echo [runvalgrind] input $input 
gcc $input -lm -g -o $exename

echo [runvalgrind] exename $exename 
valgrind --leak-check=full  --track-origins=yes ./$exename 
