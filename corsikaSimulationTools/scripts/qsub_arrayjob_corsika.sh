#!/bin/zsh
#
# script to run corsika
#
# CORSIKA 6.990
#
# Heike Prokoph & Gernot Maier
#
#

# run number is set by parent script (TASKID in arrayjobs)
RUN=$SGE_TASK_ID
# simulation type (e.g. VERITAS)
STYPE=SSSS
# primary particle type (e.g. gamma, electron, ...)
PTYPE=PPPP
# zenith angle
IZE=ZZZZ
# atmospheric profile
ATM=AAAA

# data directory (where data will be copied after the simulations)
# log directory (all input and log files are located here)
ODIR=DATADIR
LDIR=LOGDIR
mkdir -p $ODIR
mkdir -p $LDIR

#######################################################################
# input file name (DATrunnumber.inp) 
INAM=$TMPDIR/DAT$RUN.inp

echo "writing corsika input file: $INAM"

### basic stuff ###
echo "RUNNR   $RUN" > $INAM          # number of run
echo "EVTNR   1"  >> $INAM           # number of first shower event
echo "NSHOW   5000" >> $INAM         # number of showers to generate
# echo "NSHOW   15000" >> $INAM         # number of showers to generate
echo "DATBAS  no" >> $INAM           # write a file with parameters used
echo "USER    $USER" >> $INAM                     
echo "HOST    $HOSTNAME" >> $INAM                

### random numbers (4 lines needed for IACT mode) ###
SEED=$RUN
echo "SEED    $SEED 0   0" >> $INAM
let "SEED = $SEED + 2"
echo "SEED    $SEED 0   0" >> $INAM
let "SEED = $SEED + 2"
echo "SEED    $SEED 0   0" >> $INAM
let "SEED = $SEED + 2"
echo "SEED    $SEED 0   0" >> $INAM

### primary particle ###
if [ $PTYPE = "gamma" ] || [ $PTYPE = "1" ]
    then
    LENERG="50."
    if [ $IZE = "00" ] || [ $IZE = "20" ]
	then
	LENERG="30."
    fi
    if [ $IZE = "55" ] || [ $IZE = "60" ] || [ $IZE = "65" ]
	then
	LENERG="100."
    fi
    echo "PRMPAR  1" >> $INAM                # particle type of prim. particle    
    echo "ERANGE  $LENERG 200.E3" >> $INAM   # energy range of primary particle (in GeV)
    echo "VIEWCONE 0. 0." >> $INAM           # can be a cone around fixed THETAP/PHIP
    echo "CSCAT 5 1000E2 0." >> $INAM        # use shower several times (gammas)
fi
if [ $PTYPE = "gamma_cone08" ]
    then
    LENERG="50."
    if [ $IZE = "00" ] || [ $IZE = "20" ]
	then
	LENERG="30."
    fi
    if [ $IZE = "55" ] || [ $IZE = "60" ] || [ $IZE = "65" ]
	then
	LENERG="100."
    fi
    echo "PRMPAR  1" >> $INAM                # particle type of prim. particle    
    echo "ERANGE  $LENERG 200.E3" >> $INAM   # energy range of primary particle (in GeV)
    echo "VIEWCONE 0. 4." >> $INAM           # can be a cone around fixed THETAP/PHIP
    echo "CSCAT 5 1000E2 0." >> $INAM        # use shower several times (gammas)
fi
if [ $PTYPE = "electron" ] || [ $PTYPE = "2" ]
    then
    echo "PRMPAR  2" >> $INAM
    echo "ERANGE  30. 200.E3" >> $INAM
    echo "VIEWCONE 4. 0." >> $INAM   
    echo "CSCAT 5 1000E2 0." >> $INAM
fi
if [ $PTYPE = "proton" ] || [ $PTYPE = "14" ]
    then
    echo "PRMPAR  14" >> $INAM
    echo "ERANGE  30. 600.E3" >> $INAM
#    echo "VIEWCONE 4. 0." >> $INAM   
    echo "VIEWCONE 0. 0." >> $INAM   
    echo "CSCAT 5 1000E2 0." >> $INAM
fi
if [ $PTYPE = "helium" ] || [ $PTYPE = "402" ]
    then
    echo "PRMPAR  402" >> $INAM
    echo "ERANGE  30. 600.E3" >> $INAM
    echo "VIEWCONE 4. 0." >> $INAM   
    echo "CSCAT 5 1000E2 0." >> $INAM
fi
echo "ESLOPE  -2.0" >> $INAM         # slope of primary energy spectrum
echo "THETAP  $IZE $IZE" >> $INAM    # range of zenith angles (degree)
echo "PHIP    0. 360." >> $INAM      # range of azimuth angles (degree)

### site specific variables ###
#echo "ATMOD   1" >> $INAM             # use atmospheric MODTRAN models 
echo "ATMOSPHERE $ATM T" >> $INAM     # atmosphere, with refraction
echo "OBSLEV  1270.E2" >> $INAM       # observation level (in cm)
echo "MAGNET  25.2  40.88" >> $INAM   # magnetic field at equator, l=0 [H, Z] (muT)
echo "ARRANG  10.4" >> $INAM          # rotation of array to magnetic north [D] (degree)

### telescope positions on ground ###
if [ $STYPE = "VERITAS" ] || [ $STYPE = "VTS" ]
    then
    cat $SIMUSYS/scripts/telPOS_VERITAS.dat >> $INAM
fi

### interaction model parameters ###
echo "FIXHEI  0. 0" >> $INAM         # first interaction height & target (new)
echo "FIXCHI 0." >> $INAM            # fixed starting altitude in g/cm2
echo "TSTART  T" >> $INAM            # needed for emission and scattering of primary

echo "ECUTS   0.30 0.1 0.020 0.020" >> $INAM 
#    echo ECUTS   0.30  0.05   0.00005   0.00005 >> $INAM
echo "MUADDI  F" >> $INAM            # additional info for muons
echo "MUMULT  T" >> $INAM            # muon multiple scattering angle
echo "LONGI   T  20. F F" >> $INAM   # longit.distr. & step size & fit
echo "MAXPRT  50" >> $INAM           # max. number of printed events (old: set to 0, cause we are not storing particles anyway?)
echo "ECTMAP  1.E5" >> $INAM         # cut on gamma factor for printout 
echo "ELMFLG  F   T" >> $INAM
echo "RADNKG  200.E2" >> $INAM
echo "HADFLG  0  0  0  0  0  2" >> $INAM
echo "QGSJET  T  0" >> $INAM
echo "QGSSIG  T" >> $INAM
echo "HILOW 100." >> $INAM      # transition energy between low and high energy model [GeV]
echo "PAROUT F F" >> $INAM

### cherenkov output ###
echo "CERFIL  F" >> $INAM            
echo "CERSIZ  5." >> $INAM           
echo "CWAVLG  200. 700." >> $INAM    

echo "DEBUG   F  6  F  1000000" >> $INAM           # debug flag and logical unit for output
#echo "DIRECT  $SDIR" >> $INAM                      # CORSIKA data: /dev/null means nothing is written
echo "DIRECT /dev/null" >> $INAM                   # CORSIKA data: /dev/null means nothing is written
echo "TELFIL $TMPDIR/DAT$RUN.telescope" >> $INAM   # telescope photon bunch output (eventio format)
echo "EXIT" >> $INAM
# end of generating corsika file
#################################################################

cp -v -f $INAM $LDIR/

#################################################################
# do the real stuff
#################################################################
cd $TMPDIR

# link CORSIKA run directory
CDIR="/afs/ifh.de/group/cta/VERITAS/software/corsika/corsika-6990/run/"
CORS=corsika6990Linux_QGSII_urqmd

ln -sf $CDIR/* .   # needed to get atmospheric profiles within TMPDIR environment

# run CORSIKA executable
./$CORS < $INAM >& $TMPDIR/DAT$RUN.log

# check if this run was successfull
WERR=`grep -i "end of run" $TMPDIR/DAT$RUN.log | wc -l`
if [ $WERR != 1 ] 
    then
    touch $LDIR/DAT$RUN.FAILED
    grep -i "end of run" $TMPDIR/DAT$RUN.log >> touch $LDIR/DAT$RUN.FAILED
fi
    
# finalize 
echo bzip2 everything and move to final destination
bzip2 -f -v $TMPDIR/DAT$RUN.log
mv -v -f $TMPDIR/DAT$RUN.log.bz2 $LDIR/

bzip2 -f -v $TMPDIR/DAT$RUN.telescope
mv -v -f $TMPDIR/DAT$RUN.telescope.bz2 $ODIR/

exit






