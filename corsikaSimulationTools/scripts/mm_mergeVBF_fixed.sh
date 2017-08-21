#! /bin/sh
#
# merge vbf files to one single file
#
#
# Author: Gernot Maier
#
#


FLIST="$1"
ODIR="$2"
OFIL="$3"
RUN="$4"

SDIR="$VERITAS_USER_LOG_DIR/merge"
mkdir -p $SDIR

OSUB=$SDIR/$OFIL.sh
rm -f $OSUB
touch $OSUB

echo "QSUBDIR $OSUB"

sed -e "s|IIII|$FLIST|" \
    -e "s|OOOO|$OFIL|" \
    -e "s|UUUUU|$RUN|" \
    -e "s|DDDD|$ODIR|" qsub_mergeVBF.sh > $OSUB

chmod u+x $OSUB

qsub -V -l h_cpu=10:20:00 -l h_vmem=4000M -l tmpdir_size=100G -o $SDIR -e $SDIR/ "$OSUB"

exit
