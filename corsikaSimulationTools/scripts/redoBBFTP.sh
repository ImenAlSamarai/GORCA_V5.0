#!/bin/sh
#
# 

if [ ! -n "$1" ] 
then
 echo "./redoSimulations.sh  <file list>"
exit
fi

echo $1

FILES=`cat $1`


#for AFIL in $FILES
for ((i=200000; i < 203339; i++))
do
     AFIL=$i
     FFIL=/lustre/fs2/group/cta/maierg/simulations/V5_FLWO/gamma_20deg_750m/corsika/DAT$i.telescope.bz2
     if [ ! -e "$FFIL" ]
     then
         echo $FFIL
#     echo $AFIL
#      bbftp -u bbftp  -V -S -p 12 -e "get /raid/tevray/maierg/simulations/V5_FLWO/proton_20deg_750m/corsika/$AFIL /lustre/fs2/group/cta/maierg/simulations/V5_FLWO/proton_20deg_750m/corsika/$AFIL" www.hep.physics.mcgill.ca
         bbftp -u bbftp  -V -S -p 12 -e "get /raid/tevray/maierg/simulations/V5_FLWO/gamma_20deg_750m/corsika/DAT$AFIL.telescope.bz2 /lustre/fs2/group/cta/maierg/simulations/V5_FLWO/gamma_20deg_750m/corsika/DAT$AFIL.telescope.bz2" www.hep.physics.mcgill.ca

      fi

#      bbftp -u bbftp -V -S -m -e "rm $AFIL" gamma1.astro.ucla.edu
#     bbftp -u bbftp -V -S -m -e "get /veritas/upload/CORSIKA_upload/new_array/20deg/DAT$AFIL.telescope.bz2 /scratch/flor10/maierg/simulations/V5_FLWO/gamma_20deg_750m/corsika/DAT$AFIL.telescope.bz2" gamma1.astro.ucla.edu
#     bbftp -u bbftp -V -S -m -e "rm /veritas/upload/CORSIKA_upload/new_array/20deg/DAT$AFIL.telescope.bz2" gamma1.astro.ucla.edu
#     bbftp -u bbftp -V -S -m -e "rm /veritas/upload/CORSIKA_upload/new_array/20deg/DAT$AFIL.log.bz2" gamma1.astro.ucla.edu
#     bbftp -u bbftp -V -S -m -e "rm /veritas/upload/CORSIKA_upload/new_array/20deg/DAT$AFIL.inp" gamma1.astro.ucla.edu
done

exit


