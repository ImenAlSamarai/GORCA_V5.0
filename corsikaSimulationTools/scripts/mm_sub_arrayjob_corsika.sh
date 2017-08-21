#! /bin/bash
#
# submit CORSIKA simulations for VERITAS 
#
# OBSERVE THAT RUNNING THIS SCRIPTS ON HOST OTHER THAN DESY MIGHT BE BROKEN (but easily fixable)
#
#
# /!\ environment variables needed /!\
#
#   $SIMUSYS   pointing to corsikaSimulationTools directory 
#
# Heike Prokoph & Gernot Maier
#

if [ ! -n "$1" ] || [ ! -n "$2" ] || [ ! -n "$3" ] || [ ! -n "$4" ] || [ ! -n "$5" ] || [ ! -n "$6" ]
then
  echo "./mm_sub_arrayjob_corsika.sh <ze> <start runnumber> <end runnumber> <gamma/gamma_cone08/electron/proton/helium 1/2/14/402> <atm=06/20/21/22> <VERITAS>"
  echo 
  exit
fi

########################################################
# read command line parameters
########################################################
ZEW=$1
SRUN=$2
ERUN=$3
PART=$4
ATMO=$5
ARRA=$6

# test new runnumber range
if [ ${#SRUN} -gt 6 ]
then
    echo "start runnumber to large (<999999) $SRUN"
    exit
fi
if [ ${#ERUN} -gt 6 ]
then
    echo "end runnumber to large (<999999) $ERUN"
    exit
fi

# target directory for output files
OUTDIR="$VERITAS_USER_DATA_DIR/TEMP/ATM$ATMO/"
# target directory for log/input files
LDIR="$VERITAS_USER_LOG_DIR/TEMP/ATM$ATMO/"

mkdir -p $OUTDIR
mkdir -p $LDIR

#######################################################################
# write numbers to submission script
#######################################################################

CSCRIPT="qsub_arrayjob_corsika"

sed -e "s|ZZZZ|$ZEW|" $CSCRIPT.sh > $LDIR/$CSCRIPT-a.sh
sed -e "s|AAAA|$ATMO|" $LDIR/$CSCRIPT-a.sh > $LDIR/$CSCRIPT-b.sh
rm -f $LDIR/$CSCRIPT-a.sh
sed -e "s|SSSS|$ARRA|" $LDIR/$CSCRIPT-b.sh > $LDIR/$CSCRIPT-c.sh
rm -f $LDIR/$CSCRIPT-b.sh
sed -e "s|PPPP|$PART|" $LDIR/$CSCRIPT-c.sh > $LDIR/$CSCRIPT-e.sh
rm -f $LDIR/$CSCRIPT-c.sh
sed -e "s|LOGDIR|$LDIR|" $LDIR/$CSCRIPT-e.sh > $LDIR/$CSCRIPT-f.sh
rm -f $LDIR/$CSCRIPT-e.sh
sed -e "s|DATADIR|$OUTDIR|" $LDIR/$CSCRIPT-f.sh > $LDIR/$CSCRIPT.sh
rm -f $LDIR/$CSCRIPT-f.sh

chmod u+x $LDIR/$CSCRIPT.sh


####################################################################
# submit the array job ("-t" option)
qsub -t $SRUN-$ERUN:1 -V -l os="sl*" -l h_cpu=11:45:00 -l h_vmem=3000M -l tmpdir_size=30G -o $LDIR -e $LDIR "$LDIR/$CSCRIPT.sh"

exit
