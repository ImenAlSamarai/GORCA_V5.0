#!/bin/bash
#
# Revision $Id: sub_grisudet_bbftp.sh,v 1.1.1.1 2011/07/21 20:35:56 gmaier Exp $
#
# upload grisu file to UCLA
#

DDIR="$VERITAS_USER_DATA_DIR/simulations/V4_FLWO/grisu/ATM22/"
mkdir -p $DDIR

for ZE in 00 20 30 35 40 45 50
do
#   FIL=Nov10_oa_ATM21_"$ZE"deg_050wobb.vbf.bz2
   FIL=Nov10_oa_ATM22_"$ZE"deg_050wobb.vbf.gz
   FILD=Nov2010_oa_ATM22_"$ZE"deg_050wobb.vbf.gz
   echo $FIL
   echo $DDIR/$FIL

#   bbftp -u bbftp -V -S -m -p 12 -e "get /veritas/upload/CORSIKA_grisudet/Nov2010_OldArray_ATM21/$FILD $DDIR/$FIL" gamma1.astro.ucla.edu
   if [ -e $DDIR/$FIL ]
   then
       bbftp -u bbftp -V -S -m -p 12 -e "put $DDIR/$FIL /veritas/upload/CORSIKA_grisudet/Nov2010_OldArray_ATM22/$FIL" gamma1.astro.ucla.edu
   fi

done

exit
