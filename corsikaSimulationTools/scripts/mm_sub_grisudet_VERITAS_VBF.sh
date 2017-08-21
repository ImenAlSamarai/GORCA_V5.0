#! /bin/bash
#
# submit grisudet simulations for VERITAS (VBF output)
#
# Revision $Id: mm_sub_grisudet_VERITAS_VBF.sh,v 1.2 2011/12/01 10:26:44 prokoph Exp $
#
#
# OBSERVE THAT RUNNING THIS SCRIPTS ON HOST OTHER THAN DESY MIGHT BE BROKEN (but easily fixable)
#
# /!\ environment variables needed:
#  $SIMUSYS   pointing to corsikaSimulationTools directory 
#  $GRISUSYS  pointing to GrISU with subdirectory /configuration and /bin
#
# Gernot Maier 
#

HOST=`hostname`
MCD=`date`
NODE="flor10"

if [ ! -n "$1" ] || [ ! -n "$2" ] || [ ! -n "$3" ] || [ ! -n "$4" ] || [ ! -n "$5" ] || [ ! -n "$6" ]
then
  echo "./mm_sub_grisudet_VERITAS_VBF.sh <ze> <wob> <start runnumber> <V4/V5> <gamma/gamma-iso/proton/helium 1/2/14/402> <atm=06/20/21/22>"
  echo 
  exit
fi

########################################################
# read command line parameters
########################################################
ZEW=$1
WOB=$2
SRUN=$3
ARRA=$4
PART=$5
ATMO=$6

########################################################
# set extinction files for summer/winter
########################################################
if [ $ATMO = "21" ]
then
  EXTFILE="data/V.Winter.US76.50km.profile.ext"
fi
if [ $ATMO = "22" ]
then
  EXTFILE="data/V.Summer.US76.50km.profile.ext"
fi
if [ $ATMO = "20" ]
then
  EXTFILE="data/V.Crab.US76.50km.profile.ext"
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
   GDIR="/raid/pevray/maierg/EVNDISP-346/tools/simulation/showerSimulation/"
# directory with corsika files
   CDIR="/raid/pevray/maierg/simulations/V4_FLWO_0706/gamma_"$ZEW"deg_750m/corsika/"
# target directory for grisu output
   IDIR="/raid/pevray/maierg/simulations/V4_FLWO_0706/gamma_"$ZEW"deg_750m/wobble_$WOB/"
   if [ $ARRA = "V5" ]
   then
      if [ $PART = "1" ]
      then
	 IDIR="/raid/pevray/maierg/simulations/V5_FLWO/gamma_"$ZEW"deg_750m/wobble_$WOB/"
	 CDIR="/raid/pevray/maierg/simulations/V5_FLWO/gamma_"$ZEW"deg_750m/corsikaATM22/"
      fi
      if [ $PART = "14" ]
      then
	 IDIR="/raid/pevray/maierg/simulations/V5_FLWO/proton_"$ZEW"deg_750m/wobble_$WOB/"
	 CDIR="/raid/pevray/maierg/simulations/V5_FLWO/proton_"$ZEW"deg_750m/corsika/"
      fi
   fi
# target directory for corsika simulation files (unzipped, temporary)
   DDIR=/scratch/$NODE/maierg/temp/woff_"$WOB"/
# target directory for grisu simulation output files (temporary)
   SDIR=/scratch/$NODE/maierg/temp/woff_"$WOB"/
# detector configuration file
   CFDFDIR="/raid/pevray/maierg/EVNDISP-346/data/detector_geometry/"
fi
#######################################################################
# DESY
#######################################################################
if [ $HOST = "blade88.ifh.de" -o $HOST = "wgs01.ifh.de" ]
then
   HOSTNAME="desy"
# directory with executables
   GDIR=$EVNDISPSYS"/tools/simulation/showerSimulation/" # outdated (v400)
   GDIR=$SIMUSYS"/"
# INPUT  directories with corsika files (CDIR)
# OUTPUT directories with log files (LDIR), and grisu output file (ODIR)
   if [ $PART = "1" ]
   then
      CDIR=$VERITAS_DATA_DIR"/simulations/V5_FLWO/gamma_"$ZEW"deg_750m/corsikaATM"$ATMO"/"
      OFFDIR="/simulations/"$ARRA"_FLWO/gamma_"$ZEW"deg_750m/wobble_"$WOB"/grisu_ATM"$ATMO"/"
#      OFFDIR="/simulations/"$ARRA"_FLWO/gamma_"$ZEW"deg_750m/wobble_$WOB/grisu_UG11_ATM$ATMO/"
   fi
   if [ $PART = "2" ]
   then
      CDIR=$VERITAS_DATA_DIR"/simulations/V5_FLWO/gamma_"$ZEW"deg_750m/corsika/"
      OFFDIR="/simulations/"$ARRA"_FLWO/gamma_"$ZEW"deg_750m/iso/grisu/"
   fi
   if [ $PART = "14" ]
   then
      CDIR=$VERITAS_DATA_DIR"/simulations/V5_FLWO/proton_"$ZEW"deg_750m/corsikaATM20/"
      OFFDIR="/simulations/"$ARRA"_FLWO/proton_"$ZEW"deg_750m/wobble_$WOB/grisu_atm20/"
   fi
   if [ $PART = "402" ]
   then
      CDIR=$VERITAS_DATA_DIR"/simulations/V5_FLWO/helium_"$ZEW"deg_750m/corsika/"
      OFFDIR="/simulations/"$ARRA"_FLWO/helium_"$ZEW"deg_750m/wobble_$WOB/grisu/"
   fi
# log file directory
   LDIR=$VERITAS_USER_LOG_DIR"/"$OFFDIR
# grisudet data output directory
   ODIR=$VERITAS_USER_DATA_DIR"/"$OFFDIR
# detector configuration file directory
   CFDFDIR=$GRISUSYS"/configuration/"
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
   CDIR="/data/veritas2/simulations/V5_2009/gamma_"$ZEW"deg_750m/corsika/"
# target directory for corsika simulation output files, scratch disk
   DDIR="/state/part1/tmp/gmaier/grisudet/gamma_"$ZEW"deg_750m/"
# target directory for grisu simulation output files
   ODIR="/data/veritas2/simulations/V5_2009/grisudet/gamma_"$ZEW"deg_750m/wobble_$WOB/"
# directory for detector configuration file
   CFDFDIR="/home/gmaier/CFG/"
fi
#######################################################################
# make target directories
mkdir -p $ODIR
mkdir -p $LDIR
#######################################################################
# detector configuration files
echo "grisu simulation files are written to: $ODIR"
CFGF="$CFDFDIR/BestGuess_oldArrayConfig_20101106_v416.cfg"
if [ $ARRA = "V5" ]
then
   CFGF="$CFDFDIR/BestGuess_newArrayConfig_20101106_v416.cfg"
#   CFGF="$CFDFDIR/BestGuess_newArrayConfig_FILTERUG11_20101106_v416.cfg"
fi
echo "detector configuration: $CFGF"
######################################

###############################################################
#  generating the pilot file 
PNAM=$LDIR/DAT$SRUN.pilot

echo "writing pilot file $PNAM"

let "SEED = $SRUN * 10"

echo "Pilot file generated on $HOST at $MCD" > $PNAM
echo "   (cfg file $CFGF)" > $PNAM
echo "* ARRAY TTTTTTTDIR/DAT$SRUN.cfg" >> $PNAM
echo "* CHERP TTTTTTTDIR/DAT$SRUN.grisu.fifo" >> $PNAM
echo "* LOGBK TTTTTTTDIR/DAT$SRUN.grisu.log"  >> $PNAM
echo "* VBFOF TTTTTTTDIR/DAT$SRUN.vbf"  >> $PNAM
if [ $ARRA = "V4" ]
then
  echo "* VBFST 2009-07-22   02:00:00.000000000   GPS"  >> $PNAM 
fi
if [ $ARRA = "V5" ]
then
  echo "* VBFST 2009-09-10 02:00:00.000000000  GPS"  >> $PNAM
fi
echo "* VBFTM 200.0 1 1"  >> $PNAM
echo "* VBFPR $USER $SRUN 1275 1 1" >> $PNAM
echo "* RANDM -$SEED" >> $PNAM
if [ $PART = "1" ]
then
  echo "* NBREV 0 1" >> $PNAM
  echo "* NBRPR 0 1" >> $PNAM
  echo "* SOURC 0.0 $WOB 0.0 31.675" >> $PNAM
fi
if [ $PART = "2" ]
then
  echo "* NBREV 0 10" >> $PNAM
  echo "* NBRPR 0 1" >> $PNAM
  echo "* SOURC 0.0 $WOB 2.0 31.675" >> $PNAM
fi
if [ $PART = "14" ]
then
  echo "* NBREV 0 10" >> $PNAM
  echo "* NBRPR 0 1" >> $PNAM
  echo "* SOURC 0.0 0.0 4.0 31.675" >> $PNAM
fi
if [ $PART = "402" ]
then
  echo "* NBREV 0 15" >> $PNAM
  echo "* NBRPR 0 1" >> $PNAM
  echo "* SOURC 0.0 0.0 4.0 31.675" >> $PNAM
fi
echo "* NOISE 0. 0. 200." >> $PNAM
#echo "* NOISE 0. 0. 0." >> $PNAM
echo "* NUMPE 0" >> $PNAM
echo "* FRECP 0" >> $PNAM
echo "* NOPIX 0" >> $PNAM
echo "* USEOC 0" >> $PNAM
echo "* GRIDF 1 15 15" >> $PNAM
echo "* GRIDP 1 19 19" >> $PNAM

####################################################################
# create a new job script
CSCRIPT="qsub_grisudet_VERITAS_VBF"

sed -e "s/999999/$SRUN/" $CSCRIPT.sh > $LDIR/$CSCRIPT-$SRUN-a1.sh
sed -e "s|CONFIGCONFIG|$CFGF|" $LDIR/$CSCRIPT-$SRUN-a1.sh > $LDIR/$CSCRIPT-$SRUN-a.sh
rm -f $LDIR/$CSCRIPT-$SRUN-a1.sh
sed -e "s/123456789/$ZEW/" $LDIR/$CSCRIPT-$SRUN-a.sh > $LDIR/$CSCRIPT-$SRUN-a2.sh
rm -f $LDIR/$CSCRIPT-$SRUN-a.sh
sed -e "s/987654321/$WOB/" $LDIR/$CSCRIPT-$SRUN-a2.sh > $LDIR/$CSCRIPT-$SRUN-b.sh
rm -f $LDIR/$CSCRIPT-$SRUN-a2.sh
sed -e "s/XXXXXXXX/$NJOB/" $LDIR/$CSCRIPT-$SRUN-b.sh > $LDIR/$CSCRIPT-$SRUN-d.sh
rm -f $LDIR/$CSCRIPT-$SRUN-b.sh
sed -e "s|INPUTLOGDIR|$LDIR|" $LDIR/$CSCRIPT-$SRUN-d.sh > $LDIR/$CSCRIPT-$SRUN-e.sh
rm -f $LDIR/$CSCRIPT-$SRUN-d.sh
sed -e "s|CORSIKADIR|$CDIR|" $LDIR/$CSCRIPT-$SRUN-e.sh > $LDIR/$CSCRIPT-$SRUN-f.sh
rm -f $LDIR/$CSCRIPT-$SRUN-e.sh
sed -e "s|EXEDIR|$GDIR|" $LDIR/$CSCRIPT-$SRUN-f.sh > $LDIR/$CSCRIPT-$SRUN-h.sh
rm -f $LDIR/$CSCRIPT-$SRUN-f.sh
sed -e "s|HHHHHH|$HOSTNAME|" $LDIR/$CSCRIPT-$SRUN-h.sh > $LDIR/$CSCRIPT-$SRUN-i.sh
rm -f $LDIR/$CSCRIPT-$SRUN-h.sh
sed -e "s|FINALDIR|$ODIR|" $LDIR/$CSCRIPT-$SRUN-i.sh > $LDIR/$CSCRIPT-$SRUN-ja.sh
rm -f $LDIR/$CSCRIPT-$SRUN-i.sh
sed -e "s|AAAAAAA|$EXTFILE|" $LDIR/$CSCRIPT-$SRUN-ja.sh > $LDIR/$CSCRIPT-$SRUN-j.sh
rm -f $LDIR/$CSCRIPT-$SRUN-ja.sh
sed -e "s/PARTICLE/$PART/" $LDIR/$CSCRIPT-$SRUN-j.sh > $LDIR/$CSCRIPT-$SRUN.sh
rm -f $LDIR/$CSCRIPT-$SRUN-j.sh

chmod u+x $LDIR/$CSCRIPT-$SRUN.sh

echo "qsub script " $LDIR/$CSCRIPT-$SRUN.sh 

####################################################################
# submit the job
if [ $HOSTNAME = "mcgill" ]
then
   qsub -l nodes=$NODE -q batch -o $LDIR/DAT$SRUN.out -e $LDIR/DAT$SRUN.error "$LDIR/$CSCRIPT-$SRUN.sh" 
fi
if [ $HOSTNAME = "desy" ]
then
   if [ $PART = "1" ]
   then
       qsub -V -l h_cpu=00:40:00 -l h_vmem=3000M -l tmpdir_size=20G -o /dev/null -e /dev/null "$LDIR/$CSCRIPT-$SRUN.sh"
#       qsub -V -l h_cpu=00:40:00 -l h_vmem=3000M -l tmpdir_size=20G -o $LDIR -e $LDIR "$LDIR/$CSCRIPT-$SRUN.sh"
#      qsub -V -l h_cpu=03:30:00 -l h_vmem=8000M -l tmpdir_size=60G -o /dev/null -e /dev/null "$LDIR/$CSCRIPT-$SRUN.sh"
   fi
   if [ $PART = "14" ] || [ $PART = "2" ] || [ $PART = "402" ]
   then
      qsub -V -l h_cpu=03:30:00 -l h_vmem=3000M -l tmpdir_size=30G -o /dev/null -e /dev/null "$LDIR/$CSCRIPT-$SRUN.sh"
   fi
fi
if [ $HOSTNAME = "udel" ]
then
   qsub -q low.q -pe openmp 2 -o $LDIR/ -e $LDIR/ "$LDIR/$CSCRIPT-$SRUN.sh"
fi


exit
