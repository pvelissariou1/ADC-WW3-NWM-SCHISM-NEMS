# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.16

# Note that incremental build could trigger a call to cmake_copy_f90_mod on each re-build

OrchestratorLayer/CMakeFiles/hydro_orchestrator.dir/config.f90.o: utils/CMakeFiles/hydro_utils.dir/module_hydro_stop.mod.stamp
OrchestratorLayer/CMakeFiles/hydro_orchestrator.dir/config.f90.o.provides.build: OrchestratorLayer/CMakeFiles/hydro_orchestrator.dir/config_base.mod.stamp
OrchestratorLayer/CMakeFiles/hydro_orchestrator.dir/config_base.mod.stamp: OrchestratorLayer/CMakeFiles/hydro_orchestrator.dir/config.f90.o
	$(CMAKE_COMMAND) -E cmake_copy_f90_mod mods/config_base.mod OrchestratorLayer/CMakeFiles/hydro_orchestrator.dir/config_base.mod.stamp Intel
OrchestratorLayer/CMakeFiles/hydro_orchestrator.dir/config.f90.o.provides.build:
	$(CMAKE_COMMAND) -E touch OrchestratorLayer/CMakeFiles/hydro_orchestrator.dir/config.f90.o.provides.build
OrchestratorLayer/CMakeFiles/hydro_orchestrator.dir/build: OrchestratorLayer/CMakeFiles/hydro_orchestrator.dir/config.f90.o.provides.build

OrchestratorLayer/CMakeFiles/hydro_orchestrator.dir/io_manager.f90.o: IO/CMakeFiles/hydro_netcdf_layer.dir/netcdf_layer_base.mod.stamp
OrchestratorLayer/CMakeFiles/hydro_orchestrator.dir/io_manager.f90.o.provides.build: OrchestratorLayer/CMakeFiles/hydro_orchestrator.dir/io_manager_base.mod.stamp
OrchestratorLayer/CMakeFiles/hydro_orchestrator.dir/io_manager_base.mod.stamp: OrchestratorLayer/CMakeFiles/hydro_orchestrator.dir/io_manager.f90.o
	$(CMAKE_COMMAND) -E cmake_copy_f90_mod mods/io_manager_base.mod OrchestratorLayer/CMakeFiles/hydro_orchestrator.dir/io_manager_base.mod.stamp Intel
OrchestratorLayer/CMakeFiles/hydro_orchestrator.dir/io_manager.f90.o.provides.build:
	$(CMAKE_COMMAND) -E touch OrchestratorLayer/CMakeFiles/hydro_orchestrator.dir/io_manager.f90.o.provides.build
OrchestratorLayer/CMakeFiles/hydro_orchestrator.dir/build: OrchestratorLayer/CMakeFiles/hydro_orchestrator.dir/io_manager.f90.o.provides.build

OrchestratorLayer/CMakeFiles/hydro_orchestrator.dir/orchestrator.f90.o: OrchestratorLayer/CMakeFiles/hydro_orchestrator.dir/config_base.mod.stamp
OrchestratorLayer/CMakeFiles/hydro_orchestrator.dir/orchestrator.f90.o: OrchestratorLayer/CMakeFiles/hydro_orchestrator.dir/io_manager_base.mod.stamp
OrchestratorLayer/CMakeFiles/hydro_orchestrator.dir/orchestrator.f90.o.provides.build: OrchestratorLayer/CMakeFiles/hydro_orchestrator.dir/orchestrator_base.mod.stamp
OrchestratorLayer/CMakeFiles/hydro_orchestrator.dir/orchestrator_base.mod.stamp: OrchestratorLayer/CMakeFiles/hydro_orchestrator.dir/orchestrator.f90.o
	$(CMAKE_COMMAND) -E cmake_copy_f90_mod mods/orchestrator_base.mod OrchestratorLayer/CMakeFiles/hydro_orchestrator.dir/orchestrator_base.mod.stamp Intel
OrchestratorLayer/CMakeFiles/hydro_orchestrator.dir/orchestrator.f90.o.provides.build:
	$(CMAKE_COMMAND) -E touch OrchestratorLayer/CMakeFiles/hydro_orchestrator.dir/orchestrator.f90.o.provides.build
OrchestratorLayer/CMakeFiles/hydro_orchestrator.dir/build: OrchestratorLayer/CMakeFiles/hydro_orchestrator.dir/orchestrator.f90.o.provides.build
