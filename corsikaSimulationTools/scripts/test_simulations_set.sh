#!/bin/bash
#
#

if [ ! -n "$1" ] || [ ! -n "$2" ] || [ ! -n "$3" ] || [ ! -n "$4" ]
then
   echo "./test_simulations_set.sh <grisu/grisulog/corsika/corsikaerror/failed/evndisp> <run start> <number of files> <directory>"
   exit
fi

SIM=$1
RUN=$2
NRUN=$3
DIR=$4

for ((i=0; i < $NRUN; i++))
do
# check if grisu vbf file exists
   if [ $SIM = "grisu" ]
   then
      FFIL=$DIR/DAT$RUN.vbf
      if [ ! -e $FFIL ]
      then
        echo $RUN
      fi
   fi
# check grisu log
   if [ $SIM = "grisulog" ]
   then
      FFIL=$DIR/DAT$RUN.grisu.log.bz2
      bunzip2 -c $FFIL > /tmp/DAT$RUN.log
      grep -H "10000 Cherenkov Fronts were read" /tmp/DAT$RUN.log
      rm -f /tmp/DAT$RUN.log
   fi
# check if corsika telescope file exists
   if [ $SIM = "corsika" ]
   then
      FFIL=$DIR/DAT$RUN.telescope.bz2
      if [ ! -e $FFIL ]
      then
        echo $RUN
      fi
   fi
# check if FAILED file exisits
   if [ $SIM = "failed" ]
   then
      FFIL=$DIR/DAT$RUN.FAILED
      if [ -e $FFIL ]
      then
       echo $RUN
      fi
   fi
# check corsika log files for errors
   if [ $SIM = "corsikaerror" ]
   then
      FFIL=$DIR/DAT$RUN.log.bz2
      bunzip2 -c $FFIL > /tmp/DAT$RUN.log
#      WERR=`grep -i error /tmp/DAT$RUN.log | wc -l`
#      if [ $WERR = 1 ]
#      then
#	 echo "* error message for " $RUN
#	 grep -i error /tmp/DAT$RUN.log
#         echo $RUN
#      fi
      WERR=`grep -i "end of run" /tmp/DAT$RUN.log | wc -l`
      if [ $WERR != 1 ]
      then
	 echo "* no end of run for " $RUN
         echo $RUN
      fi
#      WERR=`grep -H "THETAP  20 20" /tmp/DAT$RUN.log | wc -l`
      WERR=`grep -H "NSHOW   15000" /tmp/DAT$RUN.log | wc -l`
      if [ $WERR == 1 ]
      then
         echo $RUN
      fi
      rm -f /tmp/DAT$RUN.log
   fi
# check if evendisp file exists
   if [ $SIM = "evndisp" ]
   then
      FFIL=$DIR/$RUN.root
      if [ ! -e $FFIL ]
      then
        echo $RUN
      fi
   fi

   let "RUN = $RUN + 1"
done

exit
