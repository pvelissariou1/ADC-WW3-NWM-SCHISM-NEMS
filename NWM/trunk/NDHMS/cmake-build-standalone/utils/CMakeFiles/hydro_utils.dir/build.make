# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.16

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:


#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:


# Remove some rules from gmake that .SUFFIXES does not remove.
SUFFIXES =

.SUFFIXES: .hpux_make_needs_suffix_list


# Suppress display of executed commands.
$(VERBOSE).SILENT:


# A target that is always out of date.
cmake_force:

.PHONY : cmake_force

#=============================================================================
# Set environment variables for the build.

# The shell in which to execute make rules.
SHELL = /bin/sh

# The CMake executable.
CMAKE_COMMAND = /apps/spack/linux-centos7-x86_64/gcc-4.8.5/cmake-3.16.1-pujkqsxrn5sipm422gxshrq27d3miagd/bin/cmake

# The command to remove a file.
RM = /apps/spack/linux-centos7-x86_64/gcc-4.8.5/cmake-3.16.1-pujkqsxrn5sipm422gxshrq27d3miagd/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /scratch2/COASTAL/coastal/save/NAMED_STORMS/NEMS_APP/NWM/trunk/NDHMS

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /scratch2/COASTAL/coastal/save/NAMED_STORMS/NEMS_APP/NWM/trunk/NDHMS/cmake-build-standalone

# Include any dependencies generated for this target.
include utils/CMakeFiles/hydro_utils.dir/depend.make

# Include the progress variables for this target.
include utils/CMakeFiles/hydro_utils.dir/progress.make

# Include the compile flags for this target's objects.
include utils/CMakeFiles/hydro_utils.dir/flags.make

utils/CMakeFiles/hydro_utils.dir/module_version.F.o: utils/CMakeFiles/hydro_utils.dir/flags.make
utils/CMakeFiles/hydro_utils.dir/module_version.F.o: ../utils/module_version.F
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/scratch2/COASTAL/coastal/save/NAMED_STORMS/NEMS_APP/NWM/trunk/NDHMS/cmake-build-standalone/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building Fortran object utils/CMakeFiles/hydro_utils.dir/module_version.F.o"
	cd /scratch2/COASTAL/coastal/save/NAMED_STORMS/NEMS_APP/NWM/trunk/NDHMS/cmake-build-standalone/utils && /apps/intel/compilers_and_libraries_2018/linux/mpi/intel64/bin/mpif90 $(Fortran_DEFINES) $(Fortran_INCLUDES) $(Fortran_FLAGS) -c /scratch2/COASTAL/coastal/save/NAMED_STORMS/NEMS_APP/NWM/trunk/NDHMS/utils/module_version.F -o CMakeFiles/hydro_utils.dir/module_version.F.o

utils/CMakeFiles/hydro_utils.dir/module_version.F.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing Fortran source to CMakeFiles/hydro_utils.dir/module_version.F.i"
	cd /scratch2/COASTAL/coastal/save/NAMED_STORMS/NEMS_APP/NWM/trunk/NDHMS/cmake-build-standalone/utils && /apps/intel/compilers_and_libraries_2018/linux/mpi/intel64/bin/mpif90 $(Fortran_DEFINES) $(Fortran_INCLUDES) $(Fortran_FLAGS) -E /scratch2/COASTAL/coastal/save/NAMED_STORMS/NEMS_APP/NWM/trunk/NDHMS/utils/module_version.F > CMakeFiles/hydro_utils.dir/module_version.F.i

utils/CMakeFiles/hydro_utils.dir/module_version.F.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling Fortran source to assembly CMakeFiles/hydro_utils.dir/module_version.F.s"
	cd /scratch2/COASTAL/coastal/save/NAMED_STORMS/NEMS_APP/NWM/trunk/NDHMS/cmake-build-standalone/utils && /apps/intel/compilers_and_libraries_2018/linux/mpi/intel64/bin/mpif90 $(Fortran_DEFINES) $(Fortran_INCLUDES) $(Fortran_FLAGS) -S /scratch2/COASTAL/coastal/save/NAMED_STORMS/NEMS_APP/NWM/trunk/NDHMS/utils/module_version.F -o CMakeFiles/hydro_utils.dir/module_version.F.s

utils/CMakeFiles/hydro_utils.dir/module_hydro_stop.F.o: utils/CMakeFiles/hydro_utils.dir/flags.make
utils/CMakeFiles/hydro_utils.dir/module_hydro_stop.F.o: ../utils/module_hydro_stop.F
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/scratch2/COASTAL/coastal/save/NAMED_STORMS/NEMS_APP/NWM/trunk/NDHMS/cmake-build-standalone/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building Fortran object utils/CMakeFiles/hydro_utils.dir/module_hydro_stop.F.o"
	cd /scratch2/COASTAL/coastal/save/NAMED_STORMS/NEMS_APP/NWM/trunk/NDHMS/cmake-build-standalone/utils && /apps/intel/compilers_and_libraries_2018/linux/mpi/intel64/bin/mpif90 $(Fortran_DEFINES) $(Fortran_INCLUDES) $(Fortran_FLAGS) -c /scratch2/COASTAL/coastal/save/NAMED_STORMS/NEMS_APP/NWM/trunk/NDHMS/utils/module_hydro_stop.F -o CMakeFiles/hydro_utils.dir/module_hydro_stop.F.o

utils/CMakeFiles/hydro_utils.dir/module_hydro_stop.F.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing Fortran source to CMakeFiles/hydro_utils.dir/module_hydro_stop.F.i"
	cd /scratch2/COASTAL/coastal/save/NAMED_STORMS/NEMS_APP/NWM/trunk/NDHMS/cmake-build-standalone/utils && /apps/intel/compilers_and_libraries_2018/linux/mpi/intel64/bin/mpif90 $(Fortran_DEFINES) $(Fortran_INCLUDES) $(Fortran_FLAGS) -E /scratch2/COASTAL/coastal/save/NAMED_STORMS/NEMS_APP/NWM/trunk/NDHMS/utils/module_hydro_stop.F > CMakeFiles/hydro_utils.dir/module_hydro_stop.F.i

utils/CMakeFiles/hydro_utils.dir/module_hydro_stop.F.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling Fortran source to assembly CMakeFiles/hydro_utils.dir/module_hydro_stop.F.s"
	cd /scratch2/COASTAL/coastal/save/NAMED_STORMS/NEMS_APP/NWM/trunk/NDHMS/cmake-build-standalone/utils && /apps/intel/compilers_and_libraries_2018/linux/mpi/intel64/bin/mpif90 $(Fortran_DEFINES) $(Fortran_INCLUDES) $(Fortran_FLAGS) -S /scratch2/COASTAL/coastal/save/NAMED_STORMS/NEMS_APP/NWM/trunk/NDHMS/utils/module_hydro_stop.F -o CMakeFiles/hydro_utils.dir/module_hydro_stop.F.s

# Object files for target hydro_utils
hydro_utils_OBJECTS = \
"CMakeFiles/hydro_utils.dir/module_version.F.o" \
"CMakeFiles/hydro_utils.dir/module_hydro_stop.F.o"

# External object files for target hydro_utils
hydro_utils_EXTERNAL_OBJECTS =

lib/libhydro_utils.a: utils/CMakeFiles/hydro_utils.dir/module_version.F.o
lib/libhydro_utils.a: utils/CMakeFiles/hydro_utils.dir/module_hydro_stop.F.o
lib/libhydro_utils.a: utils/CMakeFiles/hydro_utils.dir/build.make
lib/libhydro_utils.a: utils/CMakeFiles/hydro_utils.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/scratch2/COASTAL/coastal/save/NAMED_STORMS/NEMS_APP/NWM/trunk/NDHMS/cmake-build-standalone/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Linking Fortran static library ../lib/libhydro_utils.a"
	cd /scratch2/COASTAL/coastal/save/NAMED_STORMS/NEMS_APP/NWM/trunk/NDHMS/cmake-build-standalone/utils && $(CMAKE_COMMAND) -P CMakeFiles/hydro_utils.dir/cmake_clean_target.cmake
	cd /scratch2/COASTAL/coastal/save/NAMED_STORMS/NEMS_APP/NWM/trunk/NDHMS/cmake-build-standalone/utils && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/hydro_utils.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
utils/CMakeFiles/hydro_utils.dir/build: lib/libhydro_utils.a

.PHONY : utils/CMakeFiles/hydro_utils.dir/build

utils/CMakeFiles/hydro_utils.dir/clean:
	cd /scratch2/COASTAL/coastal/save/NAMED_STORMS/NEMS_APP/NWM/trunk/NDHMS/cmake-build-standalone/utils && $(CMAKE_COMMAND) -P CMakeFiles/hydro_utils.dir/cmake_clean.cmake
.PHONY : utils/CMakeFiles/hydro_utils.dir/clean

utils/CMakeFiles/hydro_utils.dir/depend:
	cd /scratch2/COASTAL/coastal/save/NAMED_STORMS/NEMS_APP/NWM/trunk/NDHMS/cmake-build-standalone && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /scratch2/COASTAL/coastal/save/NAMED_STORMS/NEMS_APP/NWM/trunk/NDHMS /scratch2/COASTAL/coastal/save/NAMED_STORMS/NEMS_APP/NWM/trunk/NDHMS/utils /scratch2/COASTAL/coastal/save/NAMED_STORMS/NEMS_APP/NWM/trunk/NDHMS/cmake-build-standalone /scratch2/COASTAL/coastal/save/NAMED_STORMS/NEMS_APP/NWM/trunk/NDHMS/cmake-build-standalone/utils /scratch2/COASTAL/coastal/save/NAMED_STORMS/NEMS_APP/NWM/trunk/NDHMS/cmake-build-standalone/utils/CMakeFiles/hydro_utils.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : utils/CMakeFiles/hydro_utils.dir/depend

