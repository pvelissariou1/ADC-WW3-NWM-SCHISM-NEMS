#!/bin/bash

# Panagiotis Velissariou <panagiotis.velissariou@noaa.gov> - 12/05/2020
# This script compiles ADCIRC model with a pre-selected sets of
# functionalities in NUOPC coupled mode.
# Run it as: ./compile_nuopc_NoahMP.sh some-env-file some-env-module-file some-compiler-name   or,
#            ./compile_nuopc_NoahMP.sh some-env-file some-compiler-name                        or,
#            ./compile_nuopc_NoahMP.sh some-compiler-name                                      or,
#            ./compile_nuopc_NoahMP.sh
# Note: some-env-module-file will be sourced from this bash script
#       so it should be bash compatible file.

###====================
### The following lines are from the original script
###====================
# This script takes three optional arguments:
# a file which sets the environment variables
# to use in the compile (clearning any inherited
# from the calling envionrment first). The template
# for this file is trunk/NDHMS/setEnvar.sh. Please
# copy that file to trunk/NDHMS, make copies
# for your favorite compile configurations, and
# pass the appropriate file name to this script
# as desired.
# a file, esmf-impi-env.sh, which sets the esmf 
# environment variables.
# copy this file from CPL/NUOPC_cpl if doesn't exists
# in NDHMS directory.
# compiler option for configure script
#
# TO RUN:
# ./compile_nuopc_NoahMP.sh setEnvar.sh esmf-impi-env.sh 3
###====================


# Get the directory where the script is located
if [[ $(uname -s) == Darwin ]]; then
  readonly scrNAME="$( basename "$(grealpath -s "${BASH_SOURCE[0]}")" )"
  readonly scrDIR="$(cd "$(dirname "$(grealpath -s "${BASH_SOURCE[0]}")" )" && pwd -P)"
else
  readonly scrNAME="$( basename "$(realpath -s "${BASH_SOURCE[0]}")" )"
  readonly scrDIR="$(cd "$(dirname "$(realpath -s "${BASH_SOURCE[0]}")" )" && pwd -P)"
fi


###====================
### Get the script arguments: env. file and compiler
###====================
num_opt=${#}
if [ ${num_opt} -ge 3 ]; then
  env_file="${1}"
  esmf_env="${2}"
  comp_opt="${3}
elif [ ${num_opt} -eq 2 ]; then
  env_file="${1}"
  comp_opt="${2}
elif [ ${num_opt} -eq 1 ]; then
  comp_opt="${1}"
else
  if [ -n ${NEMS_COMPILER:+1} ]; then
    case "${comp_opt}" in
                pgi ) comp_opt=1 ;;
          gnu|gfort ) comp_opt=2 ;;
        intel|ifort ) comp_opt=3 ;;
               luna ) comp_opt=4 ;;
          ifort_omp ) comp_opt=5 ;;
      intel.cray_xc ) comp_opt=6 ;;
                  0 ) ;;
                  * ) echo "${scrNAME} :: Compiler option \"${comp_opt}\" is not supported"
                      echo "   Exiting ..."
                      exit 1
                      ;;
    esac
    comp_opt="$( echo "${NEMS_COMPILER}" | tr '[:upper:]' '[:lower:]' )"
  fi
fi

# Compiler to use (REQUIRED)
case "${comp_opt}" in
    0|1|2|3|4|5|6 ) ./configure ${comp_opt} ;;
      * ) echo "${scrNAME} :: Compiler option \"${comp_opt}\" is not supported"
          echo "   You must configure with compiler option! Run ./configure to see options"
          echo "   You must use compiler option 3 - if running impi"
          echo "   Exiting ..."
          exit 1
          ;;
esac

# Environment required by ESMF/NUOPC (OPTIONAL)
if [ -n "${esmf_env:+1}" ]; then
  if [ -f "${esmf_env}" ]; then
    echo "${scrNAME} :: Sourcing the environment file: \"${esmf_env}\""
    source ${esmf_env}
  else
    echo "${scrNAME} :: Using the compile options in the calling environment for ESMFMKFILE"
  fi
fi

# Environment required by WRF_HYDRO/NoahMP (OPTIONAL)
if [ -n "${env_file:+1}" ]; then
  if [ -f "${env_file}" ]; then
    # Unset these in the env so we are not mixing
    # and matching env vars and the sourced file
    unset WRF_HYDRO
    unset HYDRO_D
    unset RESERVOIR_D
    unset SPATIAL_SOIL
    unset WRF_HYDRO_RAPID
    unset WRFIO_NCD_LARGE_FILE_SUPPORT
    unset NCEP_WCOSS
    unset NWM_META
    unset WRF_HYDRO_NUDGING

    echo "${scrNAME} :: Sourcing the environment file: \"${env_file}\""
    source ${env_file}
  else
    echo "${scrNAME} :: Using the compile options in the calling environment"
  fi
fi
if [[ "${WRF_HYDRO:-0}" -ne 1 ]]; then
    echo
    echo "${scrNAME} :: The WRF_HYDRO compile option is required to be 1 for compile_nuopc_NoahMP.sh"
    echo "   Exiting ..."
    exit 1
fi
###====================


[ -e "Makefile.nuopc"           ] && rm -f  Makefile.nuopc
[ -e "Makefile.nuopc_with_main" ] && rm -f Makefile.nuopc_with_main

if [ "${MAINAPP}" == "ON" ]; then
    echo
    echo "${scrNAME} :: Compiling NUOPC with mainApp ......"
    cp CPL/NUOPC_cpl/Makefile.nuopc_with_main Makefile.nuopc
else
    cp CPL/NUOPC_cpl/Makefile.nuopc Makefile.nuopc
fi

pushd Land_models/NoahMP >/dev/null 2>&1
  [ -e "Makefile.NoahMP.nuopc" ] && rm -f Makefile.NoahMP.nuopc
  cp ../../CPL/NUOPC_cpl/Makefile.NoahMP.nuopc Makefile.NoahMP.nuopc

  [ -e "IO_code/Makefile.IO_code.nuopc" ] && rm -f IO_code/Makefile.IO_code.nuopc
  cp ../../CPL/NUOPC_cpl/Makefile.IO_code.nuopc IO_code/Makefile.IO_code.nuopc

  [ -e "MPP" ] && rm -rf  MPP
  ln -sf ../../MPP .
popd >/dev/null 2>&1

cat macros Land_models/NoahMP/hydro/user_build_options.bak  > Land_models/NoahMP/user_build_options

# make -f Makefile.nuopc nuopcclean
make -f Makefile.nuopc

if [ $? -eq 0 ]; then
  echo
  echo "${scrNAME} :: Make failed for Makefile.nuopc"
  echo "   Exiting ..."
  exit 1
fi

cp template/NoahMP/namelist.hrldas .
cp template/HYDRO/hydro.namelist .

if [ "$(cat .nwm_version)" == "none" ]; then
  # if it is not an nwm version, use the stock ones.
  cp Land_models/NoahMP/run/*TBL .
  cp template/HYDRO/HYDRO.TBL .
  cp template/HYDRO/CHANPARM.TBL .
else
    # If it's an nwm version (nwm release branch), then use these
    cp template/WCOSS/TBLS/* .
fi

echo
echo "${scrNAME} :: The environment variables used in the compile:"
grepStr="(WRF_HYDRO)|(HYDRO_D)|(SPATIAL_SOIL)|(WRF_HYDRO_RAPID)|(WRFIO_NCD_LARGE_FILE_SUPPORT)|(HYDRO_REALTIME)|(NCEP_WCOSS)|(WRF_HYDRO_NUDGING)|(NETCDF)"
printenv | egrep -w "${grepStr}" | sort

exit 0
