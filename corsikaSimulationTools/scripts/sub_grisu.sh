#!/bin/bash
#
# Revision $Id: sub_grisu.sh,v 1.1.1.1 2011/07/21 20:35:56 gmaier Exp $
#
# submit a large number of grisudet jobs
#


# gammas
#ZE=( 00 20 30 35 40 45 50 )
#RST=( 105000 205000 305000 350000 405000 455000 505000 )
#NFI=( 2500 5000 5000 5000 5000 5000 5000 )
ZE=( 20 )
RST=( 205000 )
NFI=( 500 )
ARRAY="V5"
ATMO="21"

# protons
#ZE=( 20 )
#RST=( 904999 )
#NFI=( 1 )
#ARRAY="V5"
#ATMO="20"

# loop over all zenith angle
NZE=${#ZE[@]}
for (( z = 0; z < $NZE; z++ ))
do
   ZEB=${ZE[$z]}
   NNN=${NFI[$z]}
   RUN=${RST[$z]}

#   FILES=`cat $1`
   for ((i=0; i< $NNN; i++))
#   for RUN in $FILES
   do
      echo "./mm_sub_grisudet_VERITAS_VBF.sh $ZEB 0.5 $RUN $ARRAY 1 $ATMO"
# gammas      
      ./mm_sub_grisudet_VERITAS_VBF.sh $ZEB 0.5 $RUN $ARRAY 1 $ATMO
# proton line
#      ./mm_sub_grisudet_VERITAS_VBF.sh $ZEB 0.0 $RUN $ARRAY 14 $ATMO
      let "RUN = $RUN + 1"
   done
done

exit
