#! /bin/bash
#
# submit evndisp for simulations (analyse all noise levels at the same time)
#
#
# Revision $ID$
#
MACHINE=`hostname`
MCD=`date`

####################################
# default values (replaced by command line parameters)
# zenith angle
ZEW="00"
# starting runnumber
SRUN=800000
# total number of runs
TNEV=1
# node
NODE="flor06"
####################################
####################################
####################################
# number of files per job
NJOB=10

MACHINE=`hostname`
MCD=`date`

if [ ! -n "$1" ] 
then
  echo "./mm_sub_evndisp.sh <flor node> <start runnumber> <number of jobs>" 
  exit
fi

NODE=$1

if [ -n "$2" ] 
then
  SRUN=$2
fi

if [ -n "$3" ] 
then
  NJOB=$3
fi

echo $NODE $SRUN $NJOB

######################
# main loop
QRUN=$SRUN

for ((i=0; i < $NJOB; i++))    # <= to get last input file with ANEV showers
do
##################################################################
# run scripts are written to this directory
     FDIR="/scratch/$NODE/maierg/temp/AGIS/$QRUN/"
     mkdir -p $FDIR
# output directories
     ODIR="/scratch/$NODE/maierg/temp/AGIS/EVNDISP/"
#     ODIR="/scratch/flor10/maierg/temp/AGIS/EVNDISP/"
     mkdir -p $ODIR
##################################################################
#
      CSCRIPT="qsub_evndisp_AGIS"
      OSCRIPT="qsub_evndisp_AGIS-$QRUN"

      sed -e "s/999999/$QRUN/" $CSCRIPT.sh > $FDIR/$OSCRIPT-a.sh

# add flor number
      sed -e "s/ABCDEFGH/$NODE/" $FDIR/$OSCRIPT-a.sh > $FDIR/$OSCRIPT-q.sh
      rm -f $FDIR/$OSCRIPT-a.sh

# set grisu directory
      sed -e "s|DIRDIRDIRDIR|$ODIR|" $FDIR/$OSCRIPT-q.sh > $FDIR/$OSCRIPT.sh
      rm -f $FDIR/$OSCRIPT-q.sh
      chmod u+x $FDIR/$OSCRIPT.sh

# submit the job
      qsub -l nodes=$NODE -q batch -o $FDIR/ -e $FDIR/ "$FDIR/$OSCRIPT.sh"

# new run number
      let "QRUN = $QRUN + 1"
      if [ ${#QRUN} -gt 6 ]
      then
	echo "runnumber to large (<999999) $QRUN"
	exit
      fi
done

exit
