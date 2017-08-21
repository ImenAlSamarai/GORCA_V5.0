#!/bin/tcsh
#
# script to run grisucors, and evndisp on one of the cluster nodes
#
# called from mm_sub_grisudet_VERITAS_VBF.sh
# Heike Prokoph & Gernot Maier
#
# setenv LD_LIBRARY_PATH /usr/local/lib/

# only needed for DESY hosts
source $EVNDISPSYS/setObservatory.tcsh VTS

######################################################################### 
# directory with executables, etc.
set RDIR=EXEDIR
###########################################################
# all values here a set by parent script
set ARRA=AAAAARRR
# wobble offset
set WOB=987654321
# run number 
set RUN=$SGE_TASK_ID
# particle
set PART=PARTICLE
# all input and log files are located here, all output log files are written to this directory (+subdirectories with different configurations)
set LDIR=INPUTLOGDIR
# corsika data source directory
set CDIR=CORSIKADIR
# final directory where all data products from grisudet are moved to
set ODIR=FINALDIR
# configuration file (is copied temporarily here)
set CFGF=CONFIGCONFIG
# extinction file
set EXTFILE=AAAAAAA
###########################################################
cd $RDIR

set HOST=`hostname`
set MCD=`date`

echo "HOST $HOST"
echo "USER $USER"
echo "DATE $MCD"
echo "RUN $RUN"
echo "TMPDIR $TMPDIR"

###########################################################
# copy configuration file
cp -f -v $CFGF $TMPDIR/DAT$RUN.cfg
set CFGX=$TMPDIR/DAT$RUN.cfg
ls -l $CFGX

###########################################################
#  generating the pilot file 
###############################################################

set PNAM=$LDIR/DAT$RUN.pilot

echo "writing pilot file $PNAM"

@ SEED = $RUN * 10 
echo "SEED $SEED"

echo "Pilot file generated $HOST at $MCD" > $PNAM
echo "   (cfg file $CFGF)" > $PNAM
echo "* ARRAY $TMPDIR/DAT$RUN.cfg" >> $PNAM
echo "* CHERP $TMPDIR/DAT$RUN.grisu.fifo" >> $PNAM
echo "* LOGBK $TMPDIR/DAT$RUN.grisu.log"  >> $PNAM
echo "* VBFOF $TMPDIR/DAT$RUN.vbf"  >> $PNAM
if ( $ARRA == "V4" ) then
  echo "* VBFST 2009-07-22   02:00:00.000000000   GPS"  >> $PNAM 
endif
if ( $ARRA == "V5" ) then
  echo "* VBFST 2009-09-10 02:00:00.000000000  GPS"  >> $PNAM
endif
if ( $ARRA == "V6" ) then
  echo "* VBFST 2012-10-10 02:00:00.000000000  GPS"  >> $PNAM
endif
echo "* VBFTM 200.0 1 1"  >> $PNAM
echo "* VBFPR $USER $RUN 1275 1 1" >> $PNAM
echo "* RANDM -$SEED" >> $PNAM
if ( $PART == "1" ) then
  echo "* NBREV 0 1" >> $PNAM
  echo "* NBRPR 0 1" >> $PNAM
  echo "* SOURC 0.0 $WOB 0.0 31.675" >> $PNAM
endif
if ( $PART == "2" ) then
  echo "* NBREV 0 10" >> $PNAM
  echo "* NBRPR 0 1" >> $PNAM
  echo "* SOURC 0.0 $WOB 2.0 31.675" >> $PNAM
endif
if ( $PART == "14" ) then
  echo "* NBREV 0 10" >> $PNAM
  echo "* NBRPR 0 1" >> $PNAM
  echo "* SOURC 0.0 0.0 4.0 31.675" >> $PNAM
endif
if ( $PART == "402" ) then
  echo "* NBREV 0 15" >> $PNAM
  echo "* NBRPR 0 1" >> $PNAM
  echo "* SOURC 0.0 0.0 4.0 31.675" >> $PNAM
endif
# echo "* NOISE 0. 0. 200." >> $PNAM
# no noise simulation (as standard):
echo "* NOISE 0. 0. 0." >> $PNAM
echo "* NUMPE 0" >> $PNAM
echo "* FRECP 0" >> $PNAM
echo "* NOPIX 0" >> $PNAM
echo "* USEOC 0" >> $PNAM
echo "* GRIDF 1 15 15" >> $PNAM
echo "* GRIDP 1 19 19" >> $PNAM


#ls -l $PNAM
cp -f -v $PNAM $TMPDIR/DAT$RUN.pilot
   
###########################################################
# unzip corsika file
if (! -e $TMPDIR/DAT$RUN.telescope ) then
  if (-e $CDIR/DAT$RUN.telescope ) then
      cp -f -v $CDIR/DAT$RUN.telescope $TMPDIR/DAT$RUN.telescope
  else if (-e $CDIR/DAT$RUN.telescope.bz2) then
      echo "unzipping into $TMPDIR/DAT$RUN.telescope"
      cp -f -v $CDIR/DAT$RUN.telescope.bz2 $TMPDIR/DAT$RUN.telescope.bz2
      bunzip2 -v -f $TMPDIR/DAT$RUN.telescope.bz2
  else
      echo "ERROR: corsika file not found: $CDIR/DAT$RUN.telescope.bz2"
      exit
  endif
endif
ls -l $TMPDIR/DAT$RUN.telescope

###########################################################

###########################################################
# make fifo output file (not for protons)
set FIFOF="$TMPDIR/DAT$RUN.grisu.fifo"
if( $PART == "1" ) then
   mkfifo $FIFOF
endif
###########################################################

###########################################################
# run corsikaIOreader
if( $PART == "1" ) then
# for gammas: run corsikaIOreader in the background
   ./corsikaIOreader -cors $TMPDIR/DAT$RUN.telescope -abs $EXTFILE -grisu $FIFOF -shorthisto $TMPDIR/DAT$RUN.ioread.root -queff 0.5 -seed $RUN -cfg $CFGX >& $TMPDIR/DAT$RUN.ioread.log &
else
# otherwise: write corsikaIOreader file to tempdisk
   echo "starting corsikaIOreader to tempdisk"
   ./corsikaIOreader -cors $TMPDIR/DAT$RUN.telescope -abs $EXTFILE -grisu $FIFOF -shorthisto $TMPDIR/DAT$RUN.ioread.root -queff 0.5 -seed $RUN -cfg $CFGX >& $TMPDIR/DAT$RUN.ioread.log 
endif
###########################################################


###########################################################
# run grisudet

echo "starting grisudet"
# $GRISUSYS/bin_nov2010/grisudet_shadow_vbf $TMPDIR/DAT$RUN.pilot >& $TMPDIR/DAT$RUN.grisu.error
$GRISUSYS/bin_apr2012/grisudet_shadow_vbf $TMPDIR/DAT$RUN.pilot >& $TMPDIR/DAT$RUN.grisu.error

###########################################################

###########################################################
# remove corsika file
if(-e $CDIR/DAT$RUN.telescope.bz2) then
   rm -f -v $TMPDIR/DAT$RUN.telescope
endif
###########################################################

###########################################################
# bzip everything 
bzip2 -f -v $TMPDIR/DAT$RUN.grisu.log
bzip2 -f -v $TMPDIR/DAT$RUN.grisu.error
bzip2 -f -v $TMPDIR/DAT$RUN.ioread.log

###########################################################
# mv things into the right place
mv -v -f $TMPDIR/DAT$RUN.vbf $ODIR/
mv -v -f $TMPDIR/DAT$RUN.grisu.log.bz2 $LDIR/
mv -v -f $TMPDIR/DAT$RUN.grisu.error.bz2 $LDIR/
if( $PART != "1" ) then
   mkdir -p $LDIR/ioread
   mv -v -f $TMPDIR/DAT$RUN.ioread.log.bz2 $LDIR/ioread/
   mv -v -f $TMPDIR/DAT$RUN.ioread.root $LDIR/ioread/
endif
#rm -f -f $TMPDIR/DAT$RUN.ioread.log.bz2 
#rm  -f $TMPDIR/DAT$RUN.ioread.root 
rm -f $FIFOF
rm -f $CFGX

exit
