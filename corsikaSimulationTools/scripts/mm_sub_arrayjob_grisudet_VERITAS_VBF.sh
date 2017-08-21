#! /bin/bash
#
# submit grisudet simulations for VERITAS (VBF output)
#
# Revision $Id: mm_sub_arrayjob_grisudet_VERITAS_VBF.sh,v 1.2 2012/10/23 13:22:33 gmaier Exp $
#
#
# /!\ environment variables needed:
#  $SIMUSYS   pointing to corsikaSimulationTools directory 
#  $GRISUSYS  pointing to GrISU with subdirectory /configuration and /bin
#
# Heike Prokoph & Gernot Maier
#

if [ ! -n "$1" ] || [ ! -n "$2" ] || [ ! -n "$3" ] || [ ! -n "$4" ] || [ ! -n "$5" ] || [ ! -n "$6" ]
then
  echo
  echo "./mm_sub_arrayjob_grisudet_VERITAS_VBF.sh <ze> <wob> <start runnumber> <end runnumber> <V4/V5/V6> <gamma/gamma-iso/proton/helium 1/2/14/402> <atm=06/20/21/22>"
  echo
  echo " <ze> 		zenith angle [deg]"
  echo " <wob>	 	wobble offset [deg]"	 
  echo " <start runnumber> <end runnumber>"
  echo "		start and stop run numbers of corsika data files"
  echo " <V4/V5/V6>"
  echo "		V4 = before T1 move"
  echo "		V5 = after T1 move (Sep 2009)"
  echo "		V6 = camera upgrade (Sep 2012)"
  echo "<1/2/14/402>   	particle number"
  echo "atm=06/20/21/22>"
  echo "		atmosphere (default: winter=21, summer = 22)"
  echo 
  exit
fi

########################################################
# hardwired parameter and directory values
########################################################
SCATTER="1000"        # scatter area for cores
GRISUVERS="apr12"     # grisu config version

########################################################
# read command line parameters
########################################################
ZEW=$1
WOB=$2
SRUN=$3
ERUN=$4
ARRA=$5
PART=$6
ATMO=$7

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
# directory with executables
GDIR=$SIMUSYS"/"
# INPUT  directories with corsika files (CDIR)
# OUTPUT directories with log files (LDIR), and grisu output file (ODIR)
if [ $PART = "1" ]
then
   CDIR=$VERITAS_DATA_DIR"/simulations/V5_FLWO/gamma_"$ZEW"deg_"$SCATTER"m/corsikaATM"$ATMO"/"
   OFFDIR="/simulations/"$ARRA"_FLWO/gamma_"$ZEW"deg_"$SCATTER"m/wobble_"$WOB"/grisu_"$GRISUVERS"_ATM"$ATMO"/"
fi
if [ $PART = "2" ]
then
   CDIR=$VERITAS_DATA_DIR"/simulations/V5_FLWO/gamma_"$ZEW"deg_"$SCATTER"m/corsikaATM"$ATMO"/"
   OFFDIR="/simulations/"$ARRA"_FLWO/gamma_"$ZEW"deg_"$SCATTER"m/iso/grisu_"$GRISUVERS"_ATM"$ATMO"/"
fi
if [ $PART = "14" ]
then
   CDIR=$VERITAS_DATA_DIR"/simulations/V5_FLWO/proton_"$ZEW"deg_"$SCATTER"m/corsikaATM"$ATMO"/"
   OFFDIR="/simulations/"$ARRA"_FLWO/proton_"$ZEW"deg_"$SCATTER"m/wobble_$WOB/grisu_"$GRISUVERS"_ATM"$ATMO"/"
fi
if [ $PART = "402" ]
then
   CDIR=$VERITAS_DATA_DIR"/simulations/V5_FLWO/helium_"$ZEW"deg_"$SCATTER"m/corsikaATM"$ATMO"/"
   OFFDIR="/simulations/"$ARRA"_FLWO/helium_"$ZEW"deg_"$SCATTER"m/wobble_$WOB/grisu_"$GRISUVERS"_ATM"$ATMO"/"
fi
# log file directory
LDIR=$VERITAS_USER_LOG_DIR"/"$OFFDIR
# qsub files directory (or /dev/null)
QDIR="/dev/null"
QDIR=$LDIR
# grisudet data output directory
ODIR=$VERITAS_USER_DATA_DIR"/"$OFFDIR
# detector configuration file directory
CFDFDIR=$GRISUSYS"/configuration/"
#######################################################################
# make target directories
mkdir -p $ODIR
mkdir -p $LDIR
#######################################################################
# detector configuration files
echo "grisu simulation files are written to: $ODIR"
echo "log files are written to: $LDIR"

CFGF="$CFDFDIR/BestGuess_oldArrayConfig_20101106_v416.cfg" # (Nov10)

if [ $ARRA = "V5" ]
then
    CFGF="$CFDFDIR/BestGuess_newArrayConfig_20101106_v416.cfg"  # (Nov10)
    if [ $GRISUVERS = "apr12" ]
    then
       CFGF="$CFDFDIR/Apr2012_newArrayConfig_20120412_v420.txt"    # (April2012)
    fi
#    CFGF="$CFDFDIR/Apr2012_newArrayConfig_reducedHV_CrabSept2010_20120605_v420.txt"
#    CFGF="$CFDFDIR/Apr2012_newArrayConfig_reducedHV_Mkn421March2012_20120523_v420.txt"
fi
if [ $ARRA = "V6" ]
then
  CFGF="$CFDFDIR/Upgrade_20120827_v420.txt"
fi
echo "detector configuration: $CFGF"
######################################


####################################################################
# create a new job script
CSCRIPT="qsub_arrayjob_grisudet_VERITAS_VBF"

sed -e "s|CONFIGCONFIG|$CFGF|" $CSCRIPT.sh > $LDIR/$CSCRIPT-a.sh
sed -e "s/AAAAARRR/$ARRA/" $LDIR/$CSCRIPT-a.sh > $LDIR/$CSCRIPT-a2.sh
rm -f $LDIR/$CSCRIPT-a.sh
sed -e "s/987654321/$WOB/" $LDIR/$CSCRIPT-a2.sh > $LDIR/$CSCRIPT-b.sh
rm -f $LDIR/$CSCRIPT-a2.sh
sed -e "s/XXXXXXXX/$NJOB/" $LDIR/$CSCRIPT-b.sh > $LDIR/$CSCRIPT-d.sh
rm -f $LDIR/$CSCRIPT-b.sh
sed -e "s|INPUTLOGDIR|$LDIR|" $LDIR/$CSCRIPT-d.sh > $LDIR/$CSCRIPT-e.sh
rm -f $LDIR/$CSCRIPT-d.sh
sed -e "s|CORSIKADIR|$CDIR|" $LDIR/$CSCRIPT-e.sh > $LDIR/$CSCRIPT-f.sh
rm -f $LDIR/$CSCRIPT-e.sh
sed -e "s|EXEDIR|$GDIR|" $LDIR/$CSCRIPT-f.sh > $LDIR/$CSCRIPT-h.sh
rm -f $LDIR/$CSCRIPT-f.sh
sed -e "s|HHHHHH|$HOSTNAME|" $LDIR/$CSCRIPT-h.sh > $LDIR/$CSCRIPT-i.sh
rm -f $LDIR/$CSCRIPT-h.sh
sed -e "s|FINALDIR|$ODIR|" $LDIR/$CSCRIPT-i.sh > $LDIR/$CSCRIPT-ja.sh
rm -f $LDIR/$CSCRIPT-i.sh
sed -e "s|AAAAAAA|$EXTFILE|" $LDIR/$CSCRIPT-ja.sh > $LDIR/$CSCRIPT-j.sh
rm -f $LDIR/$CSCRIPT-ja.sh
sed -e "s/PARTICLE/$PART/" $LDIR/$CSCRIPT-j.sh > $LDIR/$CSCRIPT.sh
rm -f $LDIR/$CSCRIPT-j.sh
mv -f $LDIR/$CSCRIPT.sh $LDIR/GR-$CSCRIPT.sh
CSCRIPT=GR-$CSCRIPT

chmod u+x $LDIR/$CSCRIPT.sh

echo "qsub script: $LDIR/$CSCRIPT.sh"

####################################################################
# submit the array job ("-t" option)
if [ $PART = "1" ]
then
    qsub -t $SRUN-$ERUN:1 -V -l os="sl*" -l h_cpu=00:29:00 -l h_vmem=3000M -l tmpdir_size=20G -o $QDIR -e $QDIR "$LDIR/$CSCRIPT.sh"
fi
if [ $PART = "14" ] || [ $PART = "2" ] || [ $PART = "402" ] 
then
    qsub -t $SRUN-$ERUN:1 -V -l os="sl*" -l h_cpu=11:59:00 -l h_vmem=3000M -l tmpdir_size=30G -o $QDIR -e $QDIR "$LDIR/$CSCRIPT.sh"
fi


exit
