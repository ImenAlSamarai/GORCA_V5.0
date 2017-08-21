#!/bin/sh
#
# 

if [ ! -n "$1" ] 
then
 echo "./redoSimulations.sh  <file list>"
exit
fi

echo $1

FILES=`cat $1`


for AFIL in $FILES
do
     echo $AFIL
    ./mm_sub_grisudet_VERITAS_VBF.sh flor10 20 0.5 $AFIL 1 1
done

exit


