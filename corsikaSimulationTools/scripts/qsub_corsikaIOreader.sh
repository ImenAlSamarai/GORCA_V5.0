#!/bin/tcsh
#
# script to run corsikaIOreader on cluster nodes
#
# Revision $Id: qsub_corsikaIOreader.sh,v 1.1.1.1 2011/07/21 20:35:56 gmaier Exp $
#
# Gernot Maier
#
# randazzo only

# constant set by parent script
# input file (without .telescope...)
set FFIL=DIRDIRDIRDIR
set FDIR=`basename $FFIL.telescope.bz2 .telescope.bz2`
set RDIR=CCCCCCC

#run directory
cd $RDIR

# unzipping source file
echo $FDIR
bunzip2 -c $FFIL.telescope.bz2 > $TMPDIR/$FDIR.telescope
#cp $FFIL.telescope $TMPDIR/$FDIR.telescope

# run corsikaIOreader
./corsikaIOreader -cors $TMPDIR/$FDIR.telescope -smallfile -histo $FFIL.ioread.root > $FFIL.ioread.log


exit
