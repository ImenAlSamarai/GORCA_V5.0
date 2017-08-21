#! /bin/bash
#
# submit grisudet simulations for AGIS
#
# Revision $Id: mm_sub_grisudet_AGIS.sh,v 1.1.1.1 2011/07/21 20:35:56 gmaier Exp $
#
#  ./mm_sub_grisu_cors <flor node> <ze> <wob> <start runnumber> <number of files> <njob>
#
# Gernot Maier 
#

HOST=`hostname`
MCD=`date`

if [ ! -n "$1" ] 
then
  echo "./mm_sub_grisudet_AGIS.sh <flor node> <start runnumber> <number of files>"
  exit
fi

########################################################
# read command line parameters
########################################################
NODE=$1
if [ -n "$2" ] 
then
  SRUN=$2
fi

if [ -n "$3" ] 
then
  TNEV=$3
fi

#######################################################################
# site specific variables
#######################################################################
# MCGILL
#######################################################################
if [ $HOST = "kihoskh" ]
then
   HOSTNAME="mcgill"
# directory with executables
   GDIR="/raid/pevray/maierg/EVNDISP-340/tools/simulation/showerSimulation/"
# directory with corsika files
   CDIR="/scratch/flor10/maierg/simulations/AGIS/corsika/"
# target directory for corsika simulation output files, scratch disk
   DDIR="/scratch/$NODE/maierg/temp/AGIS/scratch/"
# target directory for grisu simulation output files
   SDIR="/scratch/flor10/maierg/simulations/AGIS/grisu/"
# directory for detector configuration file
   CFDFDIR="/raid/pevray/maierg/EVNDISP-340/data/detector_geometry/AGIS/"
fi
#######################################################################
# UDEL
#######################################################################
if [ $HOST = "asterix.bartol.udel.edu" ]
then
   HOSTNAME="udel"
# directory with executables
   GDIR="/home/gmaier/showerSimulation/"
# directory with corsika files
   CDIR="/data/veritas2/simulations/AGIS/corsika/"
# target directory for corsika simulation output files, scratch disk
   DDIR="/state/part1/tmp/gmaier/grisudet/"
# target directory for grisu simulation output files
   SDIR="/data/veritas2/simulations/AGIS/grisudet/"
# directory for detector configuration file
   CFDFDIR="/home/gmaier/CFG/AGIS/"
fi
#######################################################################
# SLAC
#######################################################################
if [ $HOST = "noric02" ] || [ $HOST = "noric05" ] || [ $HOST = "noric07" ]
then
   HOSTNAME="slac"
# directory with executables
   GDIR="/afs/slac.stanford.edu/g/agis/simulations/bartol/showerSimulation/" 
# directory with corsika files
   CDIR="/nfs/farm/g/glast/u36/agisSims/Set-Ib1/corsika/"
   CDIR="/nfs/farm/g/glast/u36/agisSims/scratch/"
# target directory for corsika simulation output files, scratch disk
   DDIR="/scratch/agis/grisudet/"
# target directory for grisu simulation output files
   SDIR="/nfs/farm/g/glast/u36/agisSims/Set-Ib/grisudet/"
# directory for detector configuration file
   CFDFDIR="/afs/slac.stanford.edu/g/agis/simulations/bartol/CFG/AGIS/"
fi
#######################################################################
echo "grisu simulation files are written to: $SDIR"
# make target directories
mkdir -p $SDIR
######################################

######################################
# detector configuration file
######################################
CFGF=( AGIS_36tel_SQ_020deg_global.cfg AGIS_36tel_SQ_015deg_global.cfg AGIS_36tel_SQ_010deg_global.cfg AGIS_36tel_SQ_005deg_global.cfg )
# number of configurations
NCFG=${#CFGF[@]}
# grid parameter
GRID=( "GRIDP 1 30 30" "GRIDP 1 50 50" "GRIDP 1 75 75" "GRIDP 1 138 138" )
# 020 camera GRIDP 1 30 30
# 015 camera GRIDP 1 50 50
# 010 camera GRIDP 1 75 75
# 005 camera GRIDP 1 138 138
######################################

###############################################################
# loop over all runs
###############################################################
SRRUN="$SRUN"
echo $SRRUN
for ((i=0; i < TNEV; i++))    # <= to get last input file with ANEV showers
do
###############################################################
# loop over detector configurations
###############################################################
  for ((t=0; t < $NCFG; t++))
  do

     CFGN=$CFDFDIR/${CFGF[$t]}

     JDIR=$SDIR/CFG$t
     mkdir -p $JDIR
     echo "JDIR $JDIR"
     XDIR="$DDIR/CFG$t"
     echo "XDIR $XDIR"

#  generating the pilot file 
     PNAM=$JDIR/DAT$SRUN.pilot

     echo "writing pilot file $PNAM"

     let "SEED = $SRUN * 10"

     echo "Pilot file generated with mm_sub.sh, generated on $HOST ($NODE) at $MCD" > $PNAM
     echo "* ARRAY $CFGN" >> $PNAM
     if [ $HOSTNAME = "slac" ]
     then
#        echo "* CHERP $XDIR/DAT$SRUN.cph" >> $PNAM
        echo "* CHERP $DDIR/DAT$SRUN.cph" >> $PNAM
     fi
     if [ $HOSTNAME = "mcgill" ] || [ $HOSTNAME = "udel" ]
     then
        echo "* CHERP $XDIR/DAT$SRUN.grisu.fifo" >> $PNAM
     fi
     echo "* OUTPU $JDIR/DAT$SRUN.grisu"  >> $PNAM
     echo "* PEOUT $XDIR/DAT$SRUN.grisu.pe" >> $PNAM
     echo "* LOGBK $XDIR/DAT$SRUN.grisu.log"  >> $PNAM
     echo "* RANDM -$SEED" >> $PNAM
     echo "* NBREV 0 1" >> $PNAM
     echo "* NBRPR 0 0" >> $PNAM
     echo "* SOURC 0.0 0.5 0.0 30.0" >> $PNAM
     echo "* NOISE 0. 0. 0." >> $PNAM
     echo "* NUMPE 0" >> $PNAM
     echo "* FRECP 0" >> $PNAM
     echo "* NOPIX 0" >> $PNAM
     echo "* USEOC 0" >> $PNAM
     echo "* ${GRID[$t]}" >> $PNAM
   done


####################################################################
# create a new job script
   CSCRIPT="qsub_grisudet_AGIS"

   sed -e "s/999999/$SRUN/" $CSCRIPT.sh > $SDIR/$CSCRIPT-$SRUN-a1.sh
# configuration file needed for telescope positions only. Must be the same in all CFGF files
   CFGN=$CFDFDIR/${CFGF[0]}
   sed -e "s|CONFIGCONFIG|$CFGN|" $SDIR/$CSCRIPT-$SRUN-a1.sh > $SDIR/$CSCRIPT-$SRUN-a.sh
   rm -f $SDIR/$CSCRIPT-$SRUN-a1.sh
   sed -e "s/ABCDEFG/$NODE/" $SDIR/$CSCRIPT-$SRUN-a.sh > $SDIR/$CSCRIPT-$SRUN-c.sh
   rm -f $SDIR/$CSCRIPT-$SRUN-a.sh
   sed -e "s|INPUTLOGDIR|$SDIR|" $SDIR/$CSCRIPT-$SRUN-c.sh > $SDIR/$CSCRIPT-$SRUN-e.sh
   rm -f $SDIR/$CSCRIPT-$SRUN-c.sh
   sed -e "s|CORSIKADIR|$CDIR|" $SDIR/$CSCRIPT-$SRUN-e.sh > $SDIR/$CSCRIPT-$SRUN-f.sh
   rm -f $SDIR/$CSCRIPT-$SRUN-e.sh
   sed -e "s|TEMPDIR|$DDIR|" $SDIR/$CSCRIPT-$SRUN-f.sh > $SDIR/$CSCRIPT-$SRUN-g.sh
   rm -f $SDIR/$CSCRIPT-$SRUN-f.sh
   sed -e "s|NCFGF|$NCFG|" $SDIR/$CSCRIPT-$SRUN-g.sh > $SDIR/$CSCRIPT-$SRUN-h.sh
   rm -f $SDIR/$CSCRIPT-$SRUN-g.sh
   sed -e "s|HHHHHHH|$HOSTNAME|" $SDIR/$CSCRIPT-$SRUN-h.sh > $SDIR/$CSCRIPT-$SRUN-i.sh
   rm -f $SDIR/$CSCRIPT-$SRUN-h.sh
   sed -e "s|EXEDIR|$GDIR|" $SDIR/$CSCRIPT-$SRUN-i.sh > $SDIR/$CSCRIPT-$SRUN-$NODE.sh
   rm -f $SDIR/$CSCRIPT-$SRUN-i.sh

   chmod u+x $SDIR/$CSCRIPT-$SRUN-$NODE.sh

####################################################################
# submit the job
####################################################################
if [ $HOSTNAME = "mcgill" ]
then
   qsub -l nodes=$NODE -q batch -o $SDIR/ -e $SDIR/ "$SDIR/$CSCRIPT-$SRUN-$NODE.sh" 
fi
if [ $HOSTNAME = "udel" ]
then
   qsub -q low.q -pe openmp 2 -l vf=4G -o $SDIR/ -e $SDIR/ "$SDIR/$CSCRIPT-$SRUN-$NODE.sh" 
fi
if [ $HOSTNAME = "slac" ]
then
  bsub -q long -R rhel40 -o $SDIR/DAT$SRUN.out -e $SDIR/DAT$SRUN.error "$SDIR/$CSCRIPT-$SRUN-$NODE.sh"
fi 

# new run number
   let "SRUN = $SRUN + 1"
   if [ ${#SRUN} -gt 6 ]
   then 
     echo "runnumber to large (<999999) $SRUN"
     exit
   fi
done


exit
