#! /bin/bash
#
# make noise files from grisudet simulations for a given cfg file
#
# Revision $Id: make_noiseFiles.sh,v 1.1.1.1 2011/07/21 20:35:56 gmaier Exp $
#
#
#  Author: Gernot Maier
#

HOST=`hostname`
MCD=`date`

if [ ! -n "$1" ] || [ ! -n "$2" ]
then
   echo "make_noiseFiles.sh <cfg file> <output directory>"
   exit
fi

CFGF=$1
ODIR=$2
mkdir -p $ODIR

# dummy input file
DUMMYIOREAD="$VERITAS_DATA_DIR/simulations/NOISE/dummy_ioread/DAT200000.ioread"

# copy exectutable into output directory
cp -f $GRISUSYS/bin/grisudet_shadow_v_4_1_gb $ODIR


NOISE=( 200 250 075 100 150 325 425 550 750 1000 )
NNOIS=${#NOISE[@]}

# loop over all noise levels
for (( n = 0; n < $NNOIS; n++ ))
do
   NOIS=${NOISE[$n]}

   echo "calculating noise files for $NOIS and configuration file $CFGF"

# copy configuration file into output directory
   cp -f $CFGF $ODIR
   CFGW=`basename $CFGF`

# creating pilot file
   OFIL=$ODIR/NOISE$NOIS
   rm -f $OFIL.pilot
   touch $OFIL.pilot

   echo "Pilot file generated on $HOST at $MCD" >> $OFIL.pilot
   echo "* ARRAY $CFGW" >> $OFIL.pilot
   echo "* CHERP $DUMMYIOREAD" >> $OFIL.pilot
   echo "* LOGBK $OFIL.grisu.log" >> $OFIL.pilot
   echo "* OUTPU $OFIL.grisu" >> $OFIL.pilot
   echo "* RANDM -"$NOIS"0000" >> $OFIL.pilot
   echo "* NBREV 0 1" >> $OFIL.pilot
   echo "* NBRPR 0 1" >> $OFIL.pilot
   echo "* SOURC 0.0 0.5 0.0 31.675" >> $OFIL.pilot
   echo "* NOISE 0. 0. $NOIS" >> $OFIL.pilot
   echo "* NUMPE 0" >> $OFIL.pilot
   echo "* FRECP 0" >> $OFIL.pilot
   echo "* NOPIX 0" >> $OFIL.pilot
   echo "* USEOC 0" >> $OFIL.pilot
   echo "* GRIDF 2 15 15" >> $OFIL.pilot
   echo "* GRIDP 2 19 19" >> $OFIL.pilot

# run grisudet
   cd $ODIR
   ./grisudet_shadow_v_4_1_gb  $OFIL.pilot >& $OFIL.grisu.error
done

# fix 075 noise file
cp -f $ODIR/NOISE075.grisu $ODIR/NOISE75.grisu

# rm executable in output directory and grisu clutter
rm -f $GRISUSYS/bin/grisudet_NN $ODIR
rm -f facetgrid.out
rm -f pixelgrid.out

exit

