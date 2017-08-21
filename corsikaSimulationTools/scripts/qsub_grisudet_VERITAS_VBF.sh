#!/bin/tcsh
#
# script to run grisucors, and evndisp on one of the cluster nodes
#
# called from mm_sub_grisudet_VERITAS_VBF.sh
# Gernot Maier
#
# setenv LD_LIBRARY_PATH /usr/local/lib/

######################################################################### 
# host name
set HOST=HHHHHH
# directory with executables, etc.
set RDIR=EXEDIR
###########################################################
# all values here a set by parent script
# zenith angle
set ZEW=123456789
# wobble offset
set WOB=987654321
# run number 
set RUN=999999
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
# set environmental variables
if( $HOST == "udel" ) then
   setenv VBFSYS /home/gmaier/VBF/
   setenv PATH "$PATH":"$VBFSYS"
   setenv LD_LIBRARY_PATH "$VBFSYS/lib":"$LD_LIBRARY_PATH":"$ROOTSYS/lib"
endif
###########################################################
cd $RDIR
echo $HOST

echo "TMPDIR $TMPDIR"

###########################################################
# add temporary directories to pilot files
sed -e "s|TTTTTTTDIR|$TMPDIR|" $LDIR/DAT$RUN.pilot > $LDIR/DAT$RUN.pilot.temp
mv -v $LDIR/DAT$RUN.pilot.temp $LDIR/DAT$RUN.pilot
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
# copy configuration file
cp -f -v $CFGF $TMPDIR/DAT$RUN.cfg
set CFGX=$TMPDIR/DAT$RUN.cfg
ls -l $CFGX
cp $LDIR/DAT$RUN.pilot $TMPDIR/DAT$RUN.pilot


###########################################################
# run corsikaIOreader
if( $PART == "1" ) then
# for gammas: run corsikaIOreader in the background
   ./corsikaIOreader -cors $TMPDIR/DAT$RUN.telescope -abs $EXTFILE -grisu $FIFOF -shorthisto $TMPDIR/DAT$RUN.ioread.root -queff 0.5 -seed $RUN -cfg $CFGX >& $TMPDIR/DAT$RUN.ioread.log  &
else
# otherwise: write corsikaIOreader file to tempdisk
   ./corsikaIOreader -cors $TMPDIR/DAT$RUN.telescope -abs $EXTFILE -grisu $FIFOF -shorthisto $TMPDIR/DAT$RUN.ioread.root -queff 0.5 -seed $RUN -cfg $CFGX >& $TMPDIR/DAT$RUN.ioread.log 
endif
###########################################################


###########################################################
# run grisudet
#./grisudet_VBF $LDIR/DAT$RUN.pilot >& $TMPDIR/DAT$RUN.grisu.error
$GRISUSYS/bin/grisudet_shadow_vbf $TMPDIR/DAT$RUN.pilot >& $TMPDIR/DAT$RUN.grisu.error
###########################################################

###########################################################
# remove corsika file
if(-e $CDIR/DAT$RUN.telescope.bz2) then
   rm -f -v $TMPDIR/DAT$RUN.telescope
endif
###########################################################

###########################################################
# gzip everything
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
