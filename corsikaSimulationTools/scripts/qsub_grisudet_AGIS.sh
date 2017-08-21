#!/bin/tcsh
#
# script to run grisucors, and evndisp on one of the cluster nodes
#
# called from mm_sub_grisudet_AGIS.sh
#
# Gernot Maier
#
hostname
######################################################################### 
# environment
set HOST=HHHHHHH
######################################################################### 
# directory with executables, etc.
set RDIR=EXEDIR
######################################################################### 
######################################################################### 
# number of telescopes
set NTEL=248
###########################################################
# all values here a set by parent script
# node name
set FLOR=ABCDEFG
# run number 
set RUN=999999
# all input and log files are located here, all output is written to this directory (+subdirectories with different configurations)
set FDIR=INPUTLOGDIR
# corsika data source directory
set CDIR=CORSIKADIR
# temporary corsika data directory
set DDIR=TEMPDIR
# configuration file
set CFGF=CONFIGCONFIG
# number of configuration files
set NCFG=NCFGF

###########################################################
# set environmental variables 
if( $HOST == "udel" ) then
   setenv VBFSYS /home/gmaier/VBF/
   setenv PATH "$PATH":"$VBFSYS"
   setenv LD_LIBRARY_PATH "$VBFSYS/lib":"$LD_LIBRARY_PATH":"$ROOTSYS/lib"
endif
###########################################################
# make directories
cd $RDIR
mkdir -p $DDIR
###########################################################
###########################################################
# check amount of memory available on scratch disk (want at least 5 Gbyte)
df -h $DDIR
df $DDIR
df  $DDIR | tail -n 1 | awk '{ print $4 }'
set DISKSPACE=`df  $DDIR | tail -n 1 | awk '{ print $4 }'`
if ( $DISKSPACE < 5087136 ) then
   echo "FULL DISK " > $FDIR/DAT$RUN.DISK
   echo "DISKSPACE " $DISKSPACE >> $FDIR/DAT$RUN.DISK 
   hostname >> $FDIR/DAT$RUN.DISK
   df -h $DDIR >> $FDIR/DAT$RUN.DISK
   ls -lh -R $DDIR/../ >> $FDIR/DAT$RUN.DISK
   du -h -c $DDIR/../ >> $FDIR/DAT$RUN.DISK
#   exit
endif
###########################################################
# unzip corsika file
echo "looking for $CDIR/DAT$RUN.telescope" >> $FDIR/DAT$RUN.DISK
if (-e $CDIR/DAT$RUN.telescope ) then
   echo "copying  $CDIR/DAT$RUN.telescope"
   cp -f -v $CDIR/DAT$RUN.telescope $DDIR/DAT$RUN.telescope
else if (-e $CDIR/DAT$RUN.telescope.bz2) then
   echo "unzipping  $CDIR/DAT$RUN.telescope.bz2"
   bunzip2 -v -f -c $CDIR/DAT$RUN.telescope.bz2 > $DDIR/DAT$RUN.telescope
else 
   echo "ERROR: file $DDIR/DAT$RUN.telescope(.bz2) not found" > $FDIR/DAT$RUN.FAILED
   ls -l $CDIR/DAT$RUN.telescope* >> $FDIR/DAT$RUN.FAILED
   exit
endif
ls $DDIR/ >> $FDIR/DAT$RUN.DISK

###########################################################
# run corsikaIOreader once
if( $HOST == "slac" ) then
    set FIFOF="$DDIR/DAT$RUN.cph"
   ./corsikaIOreader -cors $DDIR/DAT$RUN.telescope -grisu $FIFOF -queff 0.5 -seed $RUN -shorthisto $DDIR/DAT$RUN.ioread.root -verbose  -cfg $CFGF >& $DDIR/DAT$RUN.ioread.log 
   bzip2 -f -v $DDIR/DAT$RUN*ioread.log
   mv -v -f $DDIR/DAT$RUN*ioread.log.bz2 $CDIR/
   mv -v -f $DDIR/DAT$RUN.ioread.root $CDIR/
endif

###########################################################
# loop over all configuration files
set c = 0;
while ($c < $NCFG )

   set XDIR="$DDIR/CFG$c"
   mkdir -p $XDIR

   echo "temporary directory: $XDIR"
###########################################################
# make fifo output file
   if( $HOST != "slac" ) then
      set FIFOF="$XDIR/DAT$RUN.grisu.fifo"
      rm -f $FIFOF
      mkfifo $FIFOF
   endif
   echo "$FIFOF"
###########################################################
# run corsikaIOreader
   if( $HOST != "slac" ) then
      ./corsikaIOreader -cors $DDIR/DAT$RUN.telescope -grisu $FIFOF -queff 0.5 -seed $RUN -shorthisto $XDIR/DAT$RUN.ioread.root -verbose  -cfg $CFGF >& $XDIR/DAT$RUN.ioread.log &
   endif
###########################################################
# run grisudet
   ./grisudet_AGIS $FDIR/CFG$c/DAT$RUN.pilot >& $XDIR/DAT$RUN.grisu.error
###########################################################
# fill pe trees
  if (-e $XDIR/DAT$RUN.grisu.pe ) then
      cat $XDIR/DAT$RUN.grisu.pe | ./writeGRISU_pe_to_root $NTEL $XDIR/DAT$RUN.grisu.pe.root >& $XDIR/DAT$RUN.grisu.pe.log
  else
      echo "PE output not found: $XDIR/DAT$RUN.grisu.pe"
  endif
###########################################################
# gzip everything and move to right location
   bzip2 -v -f $XDIR/DAT$RUN*grisu.log
   mv -v -f $XDIR/DAT$RUN*grisu.log.bz2 $FDIR/CFG$c/
   bzip2 -f -v $XDIR/DAT$RUN*grisu.error
   mv -v -f $XDIR/DAT$RUN*grisu.error.bz2 $FDIR/CFG$c/
   bzip2 -f -v $XDIR/DAT$RUN*grisu.pe
   mv -f $XDIR/DAT$RUN.grisu.pe.bz2 $FDIR/CFG$c/
   rm -f -v $XDIR/CFG$c/DAT$RUN*grisu
   bzip2 -f -v $XDIR/DAT$RUN.grisu.pe.log
   mv -v -f $XDIR/DAT$RUN.grisu.pe.log.bz2 $FDIR/CFG$c/
   mv -v -f $XDIR/DAT$RUN.grisu.pe.root $FDIR/CFG$c/
   if( $c == "0" ) then
      mv -v -f $XDIR/DAT$RUN.ioread.root $DDIR/
      bzip2 -f -v $XDIR/DAT$RUN*ioread.log
      mv -v -f $XDIR/DAT$RUN*ioread.log.bz2 $DDIR/
   else
      rm -f -v $XDIR/DAT$RUN.ioread.root
      rm -f -v $XDIR/DAT$RUN*ioread.log
   endif

   if( $HOST != "slac" ) then
      rm -f -v $FIFOF
   endif
###########################################################

   @ c = ($c + 1)

end

###########################################################
# remove corsika file
if (-e $CDIR/DAT$RUN.telescope.bz2) then
   rm -f -v $DDIR/DAT$RUN.telescope
endif
rm -f -v $FIFOF
###########################################################

touch $FDIR/DAT$RUN.done

exit
