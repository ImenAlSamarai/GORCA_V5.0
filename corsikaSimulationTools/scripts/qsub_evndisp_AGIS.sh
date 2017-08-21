#!/bin/tcsh
#
# script to run evndisp on one of the cluster nodes
#
# AGIS
#
# Revision $Id: qsub_evndisp_AGIS.sh,v 1.1.1.1 2011/07/21 20:35:56 gmaier Exp $
#
# Gernot Maier
#
# randazzo only

# constant set by parent script
set RUN=999999
set FLOR=ABCDEFGH
set FDIR=DIRDIRDIRDIR

# location of noise file
set XDIR="/raid/pevray/maierg/EVNDISP-340/AGIS/"
# total number of telescopes simulated
set NTEL=302

# BEOWULF
setenv LD_LIBRARY_PATH /usr/local/lib/
#run directory
set RDIR="/raid/pevray/maierg/EVNDISP-340/eventdisplay/"
cd $RDIR

# write eventdisplay files to this director
#set  PDIR="/raid/pevray/maierg/AGIS/"
set  PDIR="/scratch/flor10/maierg/AGIS/N36/"

###########################################################################
## loop over all detector configurations
set n = 0;
#foreach CFG ( AGIS_singleTelescope_015deg_F10_ AGIS_singleTelescope_010deg_F30_ AGIS_singleTelescope_020deg_F30_ AGIS_singleTelescope_005deg_F30_ )
foreach CFG ( AGIS_singleTelescope_015deg_F10_ AGIS_singleTelescope_010deg_F30_ AGIS_singleTelescope_020deg_F30_ )

# grisucors data source directory
   set CDIR="$FDIR/$RUN/CFG$n"
   set CDIR="/scratch/flor08/maierg/temp/AGIS/$RUN/CFG$n"

# temporary data directory
   set DDIR=$FDIR/$RUN/$CFG/scratch/
   mkdir -p $DDIR

# unzip grisu files
   set f = 1;
   while ($f < $NTEL )

      echo $CDIR/DAT$RUN.grisu.T$f.bz2
      if (-e $CDIR/DAT$RUN.grisu.T$f.bz2 ) then
	 bunzip2 -f -c $CDIR/DAT$RUN.grisu.T$f.bz2 > $DDIR/DAT$RUN.grisu.T$f
      endif
      if (-e $CDIR/DAT$RUN.grisu.T$f ) then
         cp -f $CDIR/DAT$RUN.grisu.T$f $DDIR/DAT$RUN.grisu.T$f
      endif
     
      @ f = ($f + 1)

  end
# noise file
  set NOISEFILE="$XDIR/$CFG"NOISE150.grisu

# number of telescope per array
#  set NATEL=( 49 49 49 49 49 49 49 4 )
  set NATEL=( 36 36 36 36 36 36 36 4 )

###########################################################################
## loop over all array distances
  set t = 1;
  foreach DIST ( 050m 075m 100m 125m 150m 175m 200m VERITAS )

# eventdisplay output directory
     set ODIR=$FDIR/"$CFG"/"$DIST"
     mkdir -p $ODIR
     setenv EVNDISPOUTPUT $ODIR

# configuration file
     set CFGF=$CFG$DIST"_EVNDISP.cfg"

# array_analysis_cuts.txt
      set ATXT="array_analysis_cuts_short_AGIS.txt"
# dead channel definition
      set DEAD="deadChannelDefinition_AGIS_MC.dat"

# run eventdisplay
      ./evndisp -display=0 -donotusedbinfo -borderthresh=4.5 -imagethresh=5. -ntelescopes=$NATEL[$t] -sourcetype=5 -camera=$CFGF -teleNoff=1 -sumfirst=3 -sumwindow=18 -tracewindowshift=-1 -nodoublepass  -arraycuts $ATXT -pedestalfile $NOISEFILE -pedestalnoiselevel=150 -pedestalseed=$RUN -deadchannelfile $DEAD -l2timecorrect=0 -l2setspecialchannels nofile -sourcefile $DDIR/DAT$RUN.grisu > $ODIR/DAT$RUN.log

# move eventdisplay file to final destination
      set QDIR=$PDIR/"$CFG"/"$DIST"
      mkdir -p $QDIR
      mv -f -v $ODIR/$RUN.root $QDIR
      mv -f -v $ODIR/DAT$RUN.log $QDIR

      @ t = ($t + 1)
  end

# remove grisu file
  rm -f -v $DDIR/DAT$RUN.grisu.T*

  @ n = ($n + 1)   
end

rm -rfv $DDIR

# tell that we are done
touch $FDIR/EVDAT$RUN.done

exit
