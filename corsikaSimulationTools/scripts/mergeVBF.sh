#! /bin/sh
#
# merge vbf files to one single file
#
if [ ! -n "$1" ] || [ ! -n "$2" ] 
then
  echo "./mergeVBF.sh <directory with vbf files> <merged vbf file>"
  exit
fi


for ZE in 00 20 30 35 40 
do
   echo "running zenith angle $ZE"

   DDIR="$VERITAS_USER_DATA_DIR/simulations/V5_FLWO/gamma_"$ZE"deg_750m/wobble_0.5/$1/"

   mkdir -p "$VERITAS_USER_DATA_DIR/simulations/V5_FLWO/grisu/$2"

   OFIL="$VERITAS_USER_DATA_DIR/simulations/V5_FLWO/grisu/$2/gamma_Sept10_"$2"_newArray_"$ZE"deg_050wobb.vbf"

   ls -1 $DDIR/*.vbf > tmp.$ZE.list
   wc -l tmp.$ZE.list

   RUN=$ZE"5000"
   if [ $ZE -eq "00" ]
   then
     RUN="105000"
   fi

   ../mergeVBF tmp.$ZE.list $OFIL $RUN >> tmp.$ZE.log &
done

exit
