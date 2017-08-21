#!/bin/tcsh
#
#  merge VBFs
#
# Revision "$Id"
#
# Author: Gernot Maier
#
set OFIL=OOOO
set IFIL=IIII
set RUN=UUUUU

#cd $EVNDISPSYS/tools/simulation/showerSimulation/ # outdated (v400)
cd $SIMUSYS/

mkdir -p $TMPDIR/vbf

cp -f $IFIL/DAT$RUN*.vbf $TMPDIR/vbf/

ls -1 $TMPDIR/vbf/*.vbf > $VERITAS_USER_DATA_DIR/TEMP/$OFIL.dat

./mergeVBF $VERITAS_USER_DATA_DIR/TEMP/$OFIL.dat $VERITAS_USER_DATA_DIR/TEMP/$OFIL.vbf  "$RUN"5000 > $VERITAS_USER_DATA_DIR/TEMP/$OFIL.log

gzip $VERITAS_USER_DATA_DIR/TEMP/$OFIL.vbf

mv $VERITAS_USER_DATA_DIR/TEMP/$OFIL.vbf.gz $VERITAS_USER_DATA_DIR/TEMP/


exit

