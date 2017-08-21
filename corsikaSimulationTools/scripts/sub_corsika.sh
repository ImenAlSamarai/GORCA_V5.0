#!/bin/bash
#
# Revision $Id: sub_corsika.sh,v 1.1.1.1 2011/07/21 20:35:56 gmaier Exp $
#
# submit a large number of corsika jobs
#

RUN=655000
ZE="65"

#FILES=`cat $1`
for ((i=0; i < 2500; i++))
#for RUN in $FILES
do
#   if [ ! -e $VERITAS_DATA_DIR/simulations/V5_FLWO/proton_20deg_750m/corsika/DAT$RUN.telescope.bz2 ]
#   then
    echo $RUN
    ./mm_sub_corsika.sh $RUN VERITAS $ZE 1000 gamma
#   fi

   let "RUN = $RUN + 1"
done

exit
