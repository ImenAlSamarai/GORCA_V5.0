#! /bin/sh
#
# merge vbf files to one single file
#
# Revision $Id: mm_mergeVBF.sh,v 1.1.1.1 2011/07/21 20:35:56 gmaier Exp $
#
# Author: Gernot Maier
#
#

IDIR=$VERITAS_USER_DATA_DIR//simulations/V5_FLWO/
GDIR="grisu_ATM21"
IDIR=$VERITAS_USER_DATA_DIR//simulations/V4_FLWO/
GDIR="grisu_ATM21"

ODIR=$VERITAS_USER_DATA_DIR//simulations/V5_FLWO/$ODIR/
ODIR=$VERITAS_USER_DATA_DIR//simulations/V4_FLWO/$ODIR/
mkdir -p $ODIR


IZE=( 00 20 30 35 40 45 50 )
IID=( 10 20 30 35 40 45 50 )
IZE=( 20 )
IID=( 20 )
NZE=${#IZE[@]}
WOF=( 0.5 )
WON=( 050 )
NWO=${#WOF[@]}

XDIR="$EVNDISPSYS/queue/"

for ((z=0; z< $NZE; z++))
do
    ZE=${IZE[$z]}
    ID=${IID[$z]}

    for ((w=0; w< $NWO; w++))
    do
      WO=${WOF[$w]}
 
      TFIL=merge"$ZE"deg"$WO"woff

      OSUB="$XDIR/$TFIL.sh"
      rm -f $OSUB
      touch $OSUB

      OFIL=Nov10_oa_ATM21_"$ZE"deg_"${WON[$w]}"wobb
      echo $OFIL

      sed -e "s|OOOO|$OFIL|" qsub_mergeVBF.sh > $XDIR/$TFIL-1.sh
      sed -e "s|IIII|$IDIR/"gamma_"$ZE"deg_750m/wobble_"$WO/$GDIR|" $XDIR/$TFIL-1.sh > $XDIR/$TFIL-2.sh
      rm -f $XDIR/$TFIL-1.sh
      sed -e "s|UUUUU|$ID|" $XDIR/$TFIL-2.sh > $XDIR/$TFIL.sh
      rm -f $XDIR/$TFIL-2.sh

      chmod u+x $XDIR/$TFIL.sh

      qsub -V -l h_cpu=10:20:00 -l h_vmem=4000M -l tmpdir_size=50G -o $XDIR  -e $XDIR/ "$XDIR/$TFIL.sh"

    done
done

exit
