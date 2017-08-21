#!/bin/bash

# MAIN
export main_dir="/home/ialsamar/GORCA_V5.0"
export out_dir="/work/se3/imen/GORCA_V5.0_out"
export tmp_dir="/work/se3/imen/GORCA_V5.0_tmp"
#export corsika_version="6990"

# execution dirs
export runPath="${main_dir}/Run"
#export corsikaPath="${simteldir}/corsika-${corsika_version}/run"
export corsikaIOreaderPath="${main_dir}/corsikaSimulationTools"
export gropticsPath="${main_dir}/groptics_SST1M_geo"
export carePath="${main_dir}/CARE_SST1M"

# configs
export gropticsCfg="${main_dir}/groptics_cfg"
export careCfg="${main_dir}/care_cfg"

# CORISKA stuff in sim_telarray
#export sim_bin="${simteldir}/runMC"
#export config_file="${sim_bin}/config.sh"
#export cka_exe="corsika${corsika_version}Linux_QGSII_urqmd"

# local qsub definition
export queue="qlong" # a queue available on ui.cta.camk.edu.pl server.
export mem="6000mb" # max memory available on ui.cta.camk.edu.pl server
export vmem="6000mb" # max virtual memory available on ui.cta.camk.edu.pl server
export walltime="168:00:00" # max walltime!

# rc
export ROOTSYS="/work/app/root_v5.34.36"
export PATHTOVBFLIBDIRECTORY=$carePath/VBF-0.3.4/lib
export PATH="$PATHTOVBFLIBDIRECTORY/bin:$ROOTSYS/bin:$PATH"
export LD_LIBRARY_PATH="$HOME/lib:$ROOTSYS/lib:/lib:/usr/lib64:/usr/lib:$LD_LIBRARY_PATH:$PATHTOVBFLIBDIRECTORY/lib"
export DYLD_LIBRARY_PATH="$HOME/lib:$ROOTSYS/lib:/lib:/usr/lib64:/usr/lib:$DYLD_LIBRARY_PATH:$PATHTOVBFLIBDIRECTORY/lib"
