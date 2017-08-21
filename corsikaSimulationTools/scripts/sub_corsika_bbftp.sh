#!/bin/bash
#
# Revision $Id: sub_corsika_bbftp.sh,v 1.1.1.1 2011/07/21 20:35:56 gmaier Exp $
#
# upload a large number of corsika files to UCLA
#
#

if [ ! -n "$1" ] ||  [ ! -n "$2" ] ||  [ ! -n "$3" ]
then
   echo "./sub_corsika_bbftp.sh <ze> <run low> <run high>"
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
#    bbftp -u bbftp -V -S -m -p 12 -e "rm /veritas/upload/CORSIKA_upload/new_array/ATM21/$FIL" gamma1.astro.ucla.edu

done

exit
