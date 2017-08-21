#!/bin/tcsh
#
# create a temporary corsika directory with all executables, etc.
#
#

set CORSDIR=$1
set TEMPDIR=$2

# remove any exisiting directories
rm -rfv $TEMPDIR

# create a temporary directory
mkdir -p $TEMPDIR

# create links

ln -s $CORSDIR/atmprof1.dat $TEMPDIR/atmprof1.dat
ln -s $CORSDIR/atmprof2.dat $TEMPDIR/atmprof2.dat
ln -s $CORSDIR/atmprof3.dat $TEMPDIR/atmprof3.dat
ln -s $CORSDIR/atmprof4.dat $TEMPDIR/atmprof4.dat
ln -s $CORSDIR/atmprof5.dat $TEMPDIR/atmprof5.dat
ln -s $CORSDIR/atmprof6.dat $TEMPDIR/atmprof6.dat
ln -s $CORSDIR/atmprof7.dat $TEMPDIR/atmprof7.dat
ln -s $CORSDIR/atmprof8.dat $TEMPDIR/atmprof8.dat
ln -s $CORSDIR/atmprof9.dat $TEMPDIR/atmprof9.dat
ln -s $CORSDIR/atmprof20.dat $TEMPDIR/atmprof20.dat
ln -s $CORSDIR/atmprof21.dat $TEMPDIR/atmprof21.dat
ln -s $CORSDIR/atmprof22.dat $TEMPDIR/atmprof22.dat

cp -v $CORSDIR/corsika6900Linux_QGSII_urqmd $TEMPDIR/
cp -v $CORSDIR/corsika6900Linux_QGSII_fluka $TEMPDIR/
cp -v $CORSDIR/corsika6900Linux_QGSII_gheisha $TEMPDIR/
cp -v $CORSDIR/corsika6960Linux_QGSII_urqmd_SLANT $TEMPDIR/
#ln -s $CORSDIR/corsika6900Linux_QGSII_urqmd $TEMPDIR/corsika6900Linux_QGSII_urqmd
ln -s $CORSDIR/EGSDAT5_.05 $TEMPDIR/EGSDAT5_.05
ln -s $CORSDIR/EGSDAT5_1. $TEMPDIR/EGSDAT5_1
ln -s $CORSDIR/EGSDAT5_.15 $TEMPDIR/EGSDAT5_.15
ln -s $CORSDIR/EGSDAT5_.25 $TEMPDIR/EGSDAT5_.25
ln -s $CORSDIR/EGSDAT5_3. $TEMPDIR/EGSDAT5_3.
ln -s $CORSDIR/EGSDAT5_.4  $TEMPDIR/EGSDAT5_.4
ln -s $CORSDIR/GLAUBTAR.DAT $TEMPDIR/GLAUBTAR.DAT
ln -s $CORSDIR/NUCLEAR.BIN $TEMPDIR/NUCLEAR.BIN
ln -s $CORSDIR/NUCNUCCS $TEMPDIR/NUCNUCCS
ln -s $CORSDIR/QGSDAT01 $TEMPDIR/QGSDAT01
ln -s $CORSDIR/qgsdat-II-03 $TEMPDIR/qgsdat-II-03
ln -s $CORSDIR/SECTNU $TEMPDIR/SECTNU
ln -s $CORSDIR/sectnu-II-03 $TEMPDIR/sectnu-II-03
ln -s $CORSDIR/tables.dat $TEMPDIR/tables.dat
ln -s $CORSDIR/UrQMD-1.3.1-xs.dat $TEMPDIR/UrQMD-1.3.1-xs.dat
ln -s $CORSDIR/VENUSDAT $TEMPDIR/VENUSDAT

exit
