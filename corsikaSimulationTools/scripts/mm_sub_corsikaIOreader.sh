#! /bin/bash
#
# submit corsikaIOreader runs
#
#
# Revision $ID$
#

# number of files per job
NJOB=40

# directory with corsikaIOreader executable
RDIR=$SIMUDIR"/"

# data directory
DDIR="$VERITAS_USER_DATA_DIR//simulations/simulation_tests/CORSIKA_WW_MODELS/"


######################
# main loop
#for F in FLUKA_005_100 FLUKA_005_500 FLUKA_030_100 FLUKA_030_500 GHEISHA_005_100 GHEISHA_030_100 URQMD_030_100 URQMD_030_100SL URQMD_030_500 URQMD_030_500SL
for F in V4_2007
do
# loop over all runs
   RUN=840000
   for ((i=0; i < $NJOB; i++))    
   do

      FDIR=$DDIR/$F/
      FFIL=$FDIR/DAT$RUN

      CSCRIPT="qsub_corsikaIOreader"
      OSCRIPT="qsub_corsikaIOreader-$F-$RUN"

      sed -e "s|CCCCCCC|$RDIR|" $CSCRIPT.sh > $FDIR/$OSCRIPT-a.sh
      sed -e "s|DIRDIRDIRDIR|$FFIL|" $FDIR/$OSCRIPT-a.sh > $FDIR/$OSCRIPT.sh
      rm -f $FDIR/$OSCRIPT-a.sh
      chmod u+x $FDIR/$OSCRIPT.sh

# submit the job
      qsub -V -l h_cpu=01:00:00 -l h_vmem=4500M -l tmpdir_size=20G -o $FDIR/ -e $FDIR/ "$FDIR/$OSCRIPT.sh"

# new run number
      let "RUN = $RUN + 1"
   done
done

exit
