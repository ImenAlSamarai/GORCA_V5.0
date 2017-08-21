#!/bin/tcsh
#
# script to run corsika
#
# CORSIKA 6.900
#
# Gernot Maier
#
#
# run number is set by parent script mm_run.sh
set RUN=999999
# simulation type
set TYPE=DDDD
# data directory (where data will be copied after the simulations)
# all input and log files are located here
set FDIR=IDIR
mkdir -p $FDIR
# temporary data directory
set DDIR=TEMPDIR
mkdir -p $DDIR
# corsika  directory
set RDIR=CORSDIR
# host name
set HOST=HHHHH

# BEOWULF
setenv LD_LIBRARY_PATH /usr/local/lib/

# set temporary corsika directory
set CDIR=$DDIR/tempCors_run$RUN

# remove temporary corsika directory
rm -rf $CDIR
# remove scratch from previous runs
rm -f -v $DDIR/DAT$RUN.*

# setup run directory with links to corsika stuff
$RDIR/makeTemporaryCorsikaDirectory.sh $RDIR $CDIR
cd $CDIR

#CORSIKA executable
#set CORS=corsika6960Linux_QGSII_urqmd_SLANT
#set CORS=corsika6960Linux_QGSII_gheisha
#set CORS=corsika6502Linux_QGSII_urqmd 

#set CORS=corsika6960Linux_QGSII_fluka
#set CORS=corsika6960Linux_QGSII_fluka_VOLUMEDET

set CORS=corsika6960Linux_QGSII_urqmd

echo "TELFILE DOING"
echo $RUN
echo $TMPDIR
set TELFIL=$TMPDIR/DAT$RUN.telescope
echo "TELFIL $TELFIL"
sed -e "s|TELESCOPEFILE|$TELFIL|" $FDIR/DAT$RUN.inp > $TMPDIR/DAT$RUN.inp

./$CORS < $TMPDIR/DAT$RUN.inp >& $FDIR/DAT$RUN.log

# check if this run was successfull
set WERR=`grep -i "end of run" $FDIR/DAT$RUN.log | wc -l`
if ( $WERR != 1 ) then
   touch $FDIR/DAT$RUN.FAILED
   grep -i "end of run" $FDIR/DAT$RUN.log >> touch $FDIR/DAT$RUN.FAILED
endif

echo bzip2 everything and move to final destination
bzip2 -f -v $FDIR/DAT$RUN.log
bzip2 -f -v $TMPDIR/DAT$RUN.telescope
mv -v -f $TMPDIR/DAT$RUN.telescope.bz2 $FDIR/
cp -v -f $TMPDIR/DAT$RUN.inp $FDIR

# remove temporary corsika directory
rm -rf $CDIR

exit
