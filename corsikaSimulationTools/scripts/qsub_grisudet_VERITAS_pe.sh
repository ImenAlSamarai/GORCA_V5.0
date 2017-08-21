#!/bin/tcsh
#
# script to run grisucors, and evndisp on one of the cluster nodes
#
# called from mm_sub_grisudet_VERITAS_VBF.sh
# Gernot Maier
#
# setenv LD_LIBRARY_PATH /usr/local/lib/

#set TMPDIR=/afs/ifh.de/group/cta/scratch/prokoph/tmp3;
#echo "Temporary directory ${TMPDIR}";

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
mkdir -p $LDIR/ioread

cd $RDIR
echo $HOST

echo "TMPDIR $TMPDIR"

###########################################################
# add temporary directories to pilot files
sed -e "s|TTTTTTTDIR|$TMPDIR|" $LDIR/DAT$RUN.pilot > $LDIR/DAT$RUN.pilot.temp
mv -v $LDIR/DAT$RUN.pilot.temp $LDIR/DAT$RUN.pilot

echo " TIME `date`"

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
#if( $PART == "1" ) then
#   mkfifo $FIFOF
#endif
###########################################################

###########################################################
# copy configuration file
cp -f -v $CFGF $TMPDIR/DAT$RUN.cfg
set CFGX=$TMPDIR/DAT$RUN.cfg
ls -l $CFGX
cp $LDIR/DAT$RUN.pilot $TMPDIR/DAT$RUN.pilot

echo "=== copying things done ==="
echo `ls $TMPDIR`
echo `date`

###########################################################
# run corsikaIOreader
if( $PART == "1" ) then
# for gammas: run corsikaIOreader in the background
# /!\ if ATM != 06 use extinction file!!!
#   $SIMUSYS/corsikaIOreader -cors $TMPDIR/DAT$RUN.telescope -abs $EXTFILE -grisu $FIFOF -shorthisto $TMPDIR/DAT$RUN.ioread.root -queff 0.5 -seed $RUN -cfg $CFGX >& $TMPDIR/DAT$RUN.ioread.log 
   $SIMUSYS/corsikaIOreader -cors $TMPDIR/DAT$RUN.telescope -grisu $FIFOF -shorthisto $TMPDIR/DAT$RUN.ioread.root -queff 0.5 -seed $RUN -cfg $CFGX >& $TMPDIR/DAT$RUN.ioread.log 
else
# otherwise: write corsikaIOreader file to tempdisk
   $SIMUSYS/corsikaIOreader -cors $TMPDIR/DAT$RUN.telescope -grisu $FIFOF -shorthisto $TMPDIR/DAT$RUN.ioread.root -queff 0.5 -seed $RUN -cfg $CFGX >& $TMPDIR/DAT$RUN.ioread.log 
endif
###########################################################

echo "=== corsika reader done ==="

###########################################################
# gzip everything
bzip2 -f -v $TMPDIR/DAT$RUN.ioread.log
mv -v -f $TMPDIR/DAT$RUN.ioread.log.bz2 $LDIR/ioread/

echo "=== corsika reader done ==="
echo `date`
echo `ls $TMPDIR`

###########################################################
# run grisudet
cd $GRISUSYS/bin/
./grisudet_shadow $TMPDIR/DAT$RUN.pilot >& $TMPDIR/DAT$RUN.grisu.error
###########################################################

echo "=== grisudet optics done ==="
echo `date`

###########################################################
# write PEs to root file
set NTEL=4
if ( -e $TMPDIR/DAT$RUN.grisu.cmout ) then
    cat $TMPDIR/DAT$RUN.grisu.cmout | ./writeGRISU_pe_to_root $NTEL $TMPDIR/DAT$RUN.pixelID.root 1 >& $TMPDIR/DAT$RUN.pixelID.log
#    cat $TMPDIR/DAT$RUN.grisu.cmout | ./writeGRISU_pe_to_root $NTEL $TMPDIR/DAT$RUN.pixelXY.root 0 >& $TMPDIR/DAT$RUN.pixelXY.log
else
   echo "WARNING: PE output not found"
endif

###########################################################
# gzip everything
bzip2 -f -v $TMPDIR/DAT$RUN.grisu.log
bzip2 -f -v $TMPDIR/DAT$RUN.grisu.error
bzip2 -f -v $TMPDIR/DAT$RUN.pixelID.log
mv -v -f $TMPDIR/DAT$RUN.pixelID.log.bz2 $LDIR/ioread/
#bzip2 -f -v $TMPDIR/DAT$RUN.pixelXY.log
#mv -v -f $TMPDIR/DAT$RUN.pixelXY.log.bz2 $LDIR/ioread/

###########################################################
# mv things into the right place
mv -v -f $TMPDIR/DAT$RUN.grisu.log.bz2 $LDIR/
mv -v -f $TMPDIR/DAT$RUN.grisu.error.bz2 $LDIR/
#mv -v -f $TMPDIR/DAT$RUN.ioread.log.bz2 $LDIR/ioread/
#mv -v -f $TMPDIR/DAT$RUN.grisu.cmout $ODIR/
mv -v -f $TMPDIR/DAT$RUN.pixelID.root $ODIR/
#mv -v -f $TMPDIR/DAT$RUN.pixelXY.root $ODIR/
#mv -v -f $TMPDIR/photon.rec $ODIR/

echo "=== copying things done ==="
echo `date`

exit
