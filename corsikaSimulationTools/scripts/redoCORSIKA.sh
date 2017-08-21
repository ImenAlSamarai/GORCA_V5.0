#!/bin/sh
#
# 

if [ ! -n "$1" ] 
then
 echo "./testSimulations.sh  <file list>"
exit
fi

echo $1

FILES=`cat $1`


for AFIL in $FILES
do
     echo $AFIL
#     ./mm_sub_corsika.sh $AFIL AGIS 20 250 250
     ./mm_sub_arrayjob_corsika.sh 40 $AFIL $AFIL 14 21 VERITAS
done

exit


