#!/bin/bash
#
# script to upload corsika files to UCLA
#
#
if [ ! -n "$1" ] ||  [ ! -n "$2" ] ||  [ ! -n "$3" ]
then
   echo
   echo "upload corsika files to UCLA" 
   echo
   echo "./sub_bbftp.sh <ze> <run low> <run high>"
   echo
   exit
fi

ZE=$1
RUN1=$2
RUN2=$3

DDIR="/lustre/fs5/group/cta/VERITAS/simulations/V5_FLWO/gamma_"$ZE"deg_750m/corsikaATM21/"

for ((i=$RUN1; i < $RUN2; i++))
do
   FIL=DAT$i.telescope.bz2
   echo $FIL

   if [ -e $DDIR/$FIL ]
   then
       bbftp -u bbftp -V -S -m -p 12 -e "put $DDIR/$FIL /veritas/upload/CORSIKA_upload/new_array/ATM21/$FIL" gamma1.astro.ucla.edu
   fi

done

exit
