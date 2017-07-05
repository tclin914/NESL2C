#!/bin/bash

input=${@:1}
CLASSNAME=$(echo $input| cut -d '/' -f 2)
outputname="output/"$CLASSNAME".sqc.c"
exename="bin/"$CLASSNAME
result="$exename.output"
ans="$CLASSNAME.ans"

./NESL2C $input --sqc
gcc $outputname -lm -o $exename 
./$exename > $result
diff -q $result $ans > /dev/null 2>&1
error=$?
if [ $error -eq 0 ] 
then 
    echo "test of $exename is correct"
elif [ $error -eq 1 ] 
then    echo "[ERROR] in result of $exename"
else 
    echo "[ERROR] There was something wrong with the diff command"
fi
