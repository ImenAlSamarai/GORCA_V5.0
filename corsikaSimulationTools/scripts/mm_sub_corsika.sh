#! /bin/bash
#
# submit corsika simulations 
#
# use CORSIKA 6.900
#
# Gernot Maier 
#

if [ ! -n "$1" ] || [ ! -n "$2" ] || [ ! -n "$3" ] || [ ! -n "$4" ] || [ ! -n "$5" ]
then
   echo "./mm_sub_corsika.sh <run number> <type=AGIS/VERITAS/V5_2009> <ze> <total number of showers simulated> <particle type=gamma/electron/proton/helium> [node]"
   exit
fi

ATM="22"

TYPE=$2

NODE="flor10"
if [ -n "$6" ]
then
  NODE="$6"
fi

#######################################################################
# starting runnumber
#  (seeds will be runnumber + 1)
SRUN=$1
# total number of showers to be simulated
TNEV=$4
# number of showers per run/job
RNEV=$TNEV
# zenit angle 
IZE=$3
# particle type
PTYPE=$5
#######################################################################
#######################################################################
# site specific variables
HOST=`hostname`
#######################################################################
# MCGILL
#######################################################################
if [ $HOST = "kihoskh" ]
then
   HOSTNAME="mcgill"
# directory with corsika executable
   CDIR="/raid/pevray/maierg/veritas/simulations/corsika-6900/run/"
# target directory for input/output files files
   IDIR="/raid/tevray/maierg/simulations/protons/$TYPE/corsika/"
# temporary data directory
   SDIR=/scratch/$NODE/maierg/corsika/
fi
#######################################################################
# UDEL
#######################################################################
if [ $HOST = "asterix.bartol.udel.edu" ]
then
   HOSTNAME="udel"
# directory with corsika executable
   CDIR="/home/gmaier/corsika-6900/run/"
# target directory for input/output files files
   IDIR="/data/veritas2/simulations/$TYPE/corsika/"
# temporary data directory
   SDIR="/state/part1/tmp/gmaier/corsika/"
fi
#######################################################################
# DESY
#######################################################################
if [ $HOST = "blade88.ifh.de" ]
then
   HOSTNAME="desy"
# directory with corsika executable
   CDIR="/afs/ifh.de/group/cta/VERITAS/software/corsika/corsika-6502/run/"
   CDIR="/afs/ifh.de/group/cta/VERITAS/software/corsika/corsika-6960/run/"
# target directory for input/output files files
   IDIR="$VERITAS_USER_DATA_DIR/TEMP/ATM$ATM/"
# temporary data directory
   SDIR="$IDIR/temp/"
#   SDIR="$IDIR/"
fi
#######################################################################
# SLAC
#######################################################################
if [ $HOST = "noric02" ] || [ $HOST = "noric05" ] || [ $HOST = "noric04" ] || [ $HOST = "noric01" ]
then
   HOSTNAME="slac"
# directory with corsika executable
   CDIR="/afs/slac/g/agis/simulations/corsika-6900/run/"
# target directory for input/output files files
   IDIR="/nfs/farm/g/glast/u36/agisSims/scratch/"
# temporary data directory
   SDIR="/scratch/agis/"
fi
echo $HOSTNAME


#######################################################################
########################################################################
mkdir -p $IDIR
mkdir -p $SDIR
#######################################################################
# input file name (DATrunnumber.inp) for first run
DNAM="DAT"
# total number of input files
TRUN=1
let "TRUN = $TNEV / $RNEV"
# total number of showers in the last run
ANEV=0

if [ $RNEV -le 0 ]
then
   echo "error: number of showers per run le 0 ($RNEV)"
   exit
fi
let "ANEV = $TNEV - $TRUN * $RNEV"

echo "Submitting corsika jobs"
echo "    generating input files for $TNEV showers"
echo "    generating $TRUN input files with $RNEV showers"
if [ $ANEV -gt 0 ]
then
  echo "    generating 1 input file with $ANEV showers"
fi

######################
# main loop

for ((i=0; i <= TRUN; i++))    # <= to get last input file with ANEV showers
do
# test if there are any events in last file
   if [ $i -eq $TRUN ] && [ $ANEV -eq 0 ] 
   then
      break
   fi
# test new runnumber
   if [ ${#SRUN} -gt 6 ]
   then
     echo "runnumber to large (<999999) $SRUN"
     exit
   fi
# generate new input file name
   ZFIL=0
   let "ZFIL = 6 - ${#SRUN}"
   INAM=$DNAM
   for ((j=0; j < ZFIL; j++ ))
   do
     INAM="$INAM"0
   done
   INAM=$INAM$SRUN.inp
   INAM=$IDIR$INAM
   echo $INAM
   TNAM=`basename $INAM .inp`
   TNAM=$SDIR/$TNAM.telescope

   echo "RUNNR $SRUN" > $INAM
   ls -l $INAM
   echo "EVTNR   1"  >> $INAM
   if [ $i -ne $TRUN ]
   then
     echo "NSHOW $RNEV" >> $INAM
   else
     echo "NSHOW $ANEV" >> $INAM
   fi
if [ $PTYPE = "gamma" ]
then
   echo PRMPAR  1 >> $INAM
   LENERG="50."
   if [ $IZE = "00" ] || [ $IZE = "20" ]
   then
      LENERG="30."
   fi
   if [ $IZE = "55" ] || [ $IZE = "60" ] || [ $IZE = "65" ]
   then
       LENERG="100."
   fi
   echo ERANGE  $LENERG 200.E3 >> $INAM
fi
if [ $PTYPE = "electron" ]
then
   echo PRMPAR  2 >> $INAM
   echo ERANGE  30. 200.E3 >> $INAM
fi
if [ $PTYPE = "proton" ]
then
   echo PRMPAR  14 >> $INAM
   echo ERANGE  30. 600.E3 >> $INAM
fi
if [ $PTYPE = "helium" ]
then
   echo PRMPAR  402 >> $INAM
   echo ERANGE  30. 600.E3 >> $INAM
fi

   echo ESLOPE  -2. >> $INAM
   echo THETAP  $IZE $IZE >> $INAM
   echo PHIP    0. 360. >> $INAM
   SEED=$SRUN
   echo SEED    $SEED 0   0 >> $INAM
   let "SEED = $SEED + 2"
   echo SEED    $SEED 0   0 >> $INAM
   let "SEED = $SEED + 2"
   echo SEED    $SEED 0   0 >> $INAM
   let "SEED = $SEED + 2"
   echo SEED    $SEED 0   0 >> $INAM
   echo ATMOD   1 >> $INAM
   echo MAGNET  25.2 40.88 >> $INAM
   echo ARRANG  10.4 >> $INAM
   echo ELMFLG  F   T >> $INAM
   echo RADNKG  200.E2 >> $INAM
   echo FIXCHI 0. >> $INAM            # fixed starting altitude in g/cm2
   echo HADFLG  0  0  0  0  0  2 >> $INAM
   echo QGSJET  T  0 >> $INAM
   echo QGSSIG  T >> $INAM
   echo HILOW 100. >> $INAM      # transition energy between low and high energy model [GeV]
   echo ECUTS   0.30  0.05  0.02  0.02 >> $INAM
#   echo ECUTS   0.30  0.05   0.00005   0.00005 >> $INAM
   echo TSTART T >> $INAM
   echo MUADDI  F >> $INAM
   echo MUMULT  T >> $INAM
   echo LONGI   T  20. F F >> $INAM
   echo MAXPRT  50 >> $INAM
   echo PAROUT F F >> $INAM
   echo ECTMAP  1.E5 >> $INAM
   echo DEBUG   F  6  F  1000000 >> $INAM
   echo DIRECT  $SDIR >> $INAM
   echo USER    gm >> $INAM                     
   echo HOST    $HOSTNAME >> $INAM                
#   echo ATMOSPHERE 6 T >> $INAM              # crashes for particles with fixed height
   echo ATMOSPHERE $ATM T >> $INAM              # crashes for particles with fixed height
   if [ $TYPE = "AGIS" ]
   then
     cat telPOS_AGIS.dat >> $INAM
   fi
   if [ $TYPE = "V5_2009" ] || [ $TYPE = "VERITAS" ]
   then
     cat telPOS_VERITAS.dat >> $INAM
#     cat telPOS_VERITAS_HE.dat >> $INAM
   fi

# ##################################################
#   echo TELFIL $TNAM >> $INAM                    # telescope line
   echo TELFIL TELESCOPEFILE >> $INAM                    # telescope line
   echo CERFIL  F >> $INAM
   echo CERSIZ 5. >> $INAM
   echo CWAVLG 200. 700. >> $INAM
   echo EXIT >> $INAM
# end of generating corsika file
#################################################################

##################################################################
#
# now starting corsika/grisu_cors
#
   CSCRIPT="corsikaFLWO7QGSII_urqmd_gamma"

   sed -e "s/999999/$SRUN/" $CSCRIPT.sh > $IDIR/$CSCRIPT-$SRUN-b.sh
   sed -e "s/A12345/$IZE/" $IDIR/$CSCRIPT-$SRUN-b.sh > $IDIR/$CSCRIPT-$SRUN-c.sh
   rm -f $IDIR/$CSCRIPT-$SRUN-b.sh
   sed -e "s/HHHHH/$HOSTNAME/" $IDIR/$CSCRIPT-$SRUN-c.sh  > $IDIR/$CSCRIPT-$SRUN-c1.sh 
   rm -f $IDIR/$CSCRIPT-$SRUN-c.sh
   sed -e "s/DDDD/AGIS/" $IDIR/$CSCRIPT-$SRUN-c1.sh  > $IDIR/$CSCRIPT-$SRUN-d.sh
   rm -f $IDIR/$CSCRIPT-$SRUN-c1.sh
   sed -e "s|IDIR|$IDIR|" $IDIR/$CSCRIPT-$SRUN-d.sh > $IDIR/$CSCRIPT-$SRUN-e.sh
   rm -f $IDIR/$CSCRIPT-$SRUN-d.sh
   sed -e "s|TEMPDIR|$SDIR|" $IDIR/$CSCRIPT-$SRUN-e.sh > $IDIR/$CSCRIPT-$SRUN-f.sh
   rm -f $IDIR/$CSCRIPT-$SRUN-e.sh
   sed -e "s|CORSDIR|$CDIR|" $IDIR/$CSCRIPT-$SRUN-f.sh > $IDIR/$CSCRIPT-$SRUN.sh
   rm -f $IDIR/$CSCRIPT-$SRUN-f.sh

   chmod u+x $IDIR/$CSCRIPT-$SRUN.sh

#######################################################################################################
# no submit all jobs
#######################################################################################################
if [ $HOSTNAME = "mcgill" ]
then
   qsub -l nodes=$NODE -q batch -o $IDIR/DAT$SRUN.out -e $IDIR/DAT$SRUN.error "$IDIR/$CSCRIPT-$SRUN.sh"
fi
if [ $HOSTNAME = "udel" ]
then
   qsub -q low.q -o $IDIR/DAT$SRUN.out -e $IDIR/DAT$SRUN.error "$IDIR/$CSCRIPT-$SRUN.sh"
fi
if [ $HOSTNAME = "slac" ]
then
  bsub -q long -R rhel40 -o $IDIR/DAT$SRUN.out -e $IDIR/DAT$SRUN.error "$IDIR/$CSCRIPT-$SRUN.sh"
fi
if [ $HOSTNAME = "desy" ]
then
#  qsub -V -l h_cpu=46:00:00 -l h_vmem=18G -l tmpdir_size=80G -o $IDIR/DAT$SRUN.out -e $IDIR/DAT$SRUN.error "$IDIR/$CSCRIPT-$SRUN.sh"
# qsub -V -l h_cpu=08:00:00 -p -1 -l h_vmem=4G -l tmpdir_size=30G -o $IDIR/DAT$SRUN.out -e $IDIR/DAT$SRUN.error "$IDIR/$CSCRIPT-$SRUN.sh"
  qsub -V -l h_cpu=08:00:00  -l h_vmem=2G -l tmpdir_size=10G -o $IDIR/DAT$SRUN.out -e $IDIR/DAT$SRUN.error "$IDIR/$CSCRIPT-$SRUN.sh"
#   echo "NOSUB"
fi
#######################################################################################################
#######################################################################################################

# new run number
   let "SRUN = $SRUN + 1"
   if [ ${#SRUN} -gt 6 ]
   then
     echo "runnumber to large (<999999) $SRUN"
     exit
   fi
done

exit
