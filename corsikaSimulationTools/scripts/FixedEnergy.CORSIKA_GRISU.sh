# start corsika/grisudet for fixed energies
# 
#

ATMO="61"
# energy binning
# 100 GeV
EFIX="100."
ESTART="4"
NSHOW="399"
# 300 GeV
# EFIX="300."
#ESTART="1"
# NSHOW="399"
# 1000 GeV
#EFIX="1000."
#ESTART="0"
#NSHOW="399"
# 3000 GeV
#EFIX="3000."
#ESTART="2"
#NSHOW="399"
# 1000 GeV
#EFIX="10000."
#ESTART="3"
#NSHOW="999"
#NSHOW="499"

# for Ze in 00 35 40 45
for Ze in 20 30 35 40 45 00
do
   if [[ $Ze == "00" ]]
   then
       RUNSTART="10${ESTART}500"
       RUNSTOPP="10${ESTART}${NSHOW}"
       FF="10"
   else
       RUNSTART="${Ze}${ESTART}500"
       RUNSTOPP="${Ze}${ESTART}${NSHOW}"
       FF="$Ze"
   fi

   echo $RUNSTART $RUNSTOPP

# CORSIKA
#   ./mm_sub_arrayjob_corsika.sh $Ze $RUNSTART $RUNSTOPP 1 $ATMO VERITAS $EFIX


# GRISUDET and merging
#   for E in V6 V5 V4
   for E in V6 V5
   do
      for W in 0.5 0.0 0.25 0.75 1.0 1.25 1.5 1.75 2.0
      do
# GRISUDET
#           ./mm_sub_arrayjob_grisudet_VERITAS_VBF.sh $Ze $W $RUNSTART $RUNSTOPP $E 1 ${ATMO}
#           continue
          
# merge VBF
            ODIR="/lustre/fs16/group/cta/VTS/ATM${ATMO}/${E}_FLWO/"
            FINAME="gamma_${Ze}deg_1000m_${E}_${W}wobb_${EFIX}GeV"
            IDIR="/lustre/fs16/group/cta/VTS/ATM${ATMO}/${E}_FLWO/gamma_${Ze}deg_1000m/wobble_${W}"
            FLIST="$ODIR/$FINAME.list"
# upload vbf files to UCLA
            bbftp -u bbftp -V -S -m -p 12 -e "put $ODIR/$FINAME.vbf.bz2 /veritas/upload/CORSIKAFixedEnergySimulations/$FINAME.vbf.bz2" gamma1.astro.ucla.edu
            continue
            rm -f $FLIST
            echo "filling list..."
            ls -1 $IDIR/DAT${FF}${ESTART}*.vbf > $FLIST
            echo "...done"
            rm -f $ODIR/$FINAME.cvbf
            rm -f $ODIR/$FINAME.cvbf.bz2
            rm -f $ODIR/$FINAME.log
            ./mm_mergeVBF_fixed.sh $FLIST $ODIR $FINAME $RUNSTART
#            $SIMUSYS/mergeVBF $FLIST $ODIR/$FINAME.cvbf $RUNSTART > $ODIR/$FINAME.log
#            bzip2 -v $ODIR/$FINAME.cvbf
      done
   done
done

exit
